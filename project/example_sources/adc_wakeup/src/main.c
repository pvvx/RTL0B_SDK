/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 */

#include "device.h"
#include "analogin_api.h"
#include <sys_api.h>

#define ADC_BUF_SIZE 8
union x {
	u32 buf[ADC_BUF_SIZE];
	u16 adc[ADC_BUF_SIZE*2];
  } adc;

u32 adc_ints = 0;

void adc_isr(void *Data)
{
	u32 isr = 0;
	u32 i = 0;

	isr = ADC_GetISR();
	if (isr & BIT_ADC_FIFO_THRESHOLD) {
	  	adc_ints++;
		for(i = 0; i < sizeof(adc.buf)/sizeof(u32); i++) {
			adc.buf[i] = (u32)ADC_Read();
		}
	}
	ADC_INTClear();
//	DBG_8195A("ADC0..3: 0x%04x 0x%04x 0x%04x 0x%04x\n", adc.adc[0], adc.adc[1], adc.adc[2], adc.adc[3]);
}

static u32 adc_suspend(u32 expected_idle_time )
{
	ADC_INTClear();
	ADC_Cmd(ENABLE);

	return TRUE;
}

static u32 adc_resume(u32 expected_idle_time)
{	
	int i;
//	ADC_Cmd(DISABLE);
//	DBG_8195A("ADC0..3 (%d):\n", adc_ints);
	DBG_8195A("%d - ADC0..3: 0x%04x 0x%04x 0x%04x 0x%04x\n", adc_ints, adc.adc[0], adc.adc[1], adc.adc[2], adc.adc[3]);
//	for (i = 0; i < sizeof(adc.adc)/sizeof(u16); i += 4) {
//		DBG_8195A("0x%04x 0x%04x 0x%04x 0x%04x\n", adc.adc[i], adc.adc[i+1], adc.adc[i+2], adc.adc[i+3]);
//		DBG_8195A("%08x, %08x\n", adc_buf[i], adc_buf[i + 1]);
//		adc_buf[i] = 0;
//		adc_buf[i+1] = 0;
//	}
	
	return TRUE;
}

VOID adc_wakeup (VOID)
{
	ADC_InitTypeDef ADCInitStruct;
	
	/* ADC Interrupt Initialization */
	InterruptRegister((IRQ_FUN)&adc_isr, ADC_IRQ, (u32)NULL, 5);
	InterruptEn(ADC_IRQ, 5);

	/* To release ADC delta sigma clock gating */
	PLL2_Set(BIT_SYS_SYSPLL_CK_ADC_EN, ENABLE);

	/* Turn on ADC active clock */
	RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);

	ADC_InitStruct(&ADCInitStruct);
	ADCInitStruct.ADC_BurstSz = 8;
	
	ADCInitStruct.ADC_OneShotTD = 2; //8 /* means 4 times */

	ADC_Init(&ADCInitStruct);
	ADC_SetOneShot(ENABLE, 100, ADCInitStruct.ADC_OneShotTD); /* 100 will task 200ms */
	
//	DBG_8195A("ADCPAR: %08x %08x\n", ADC_AnaparAd[0], ADC_AnaparAd[1]);

	ADC_INTClear();
	ADC_Cmd(ENABLE);


	pmu_register_sleep_callback(PMU_ADC_DEVICE, (PSM_HOOK_FUN)adc_suspend, (void*)NULL, (PSM_HOOK_FUN)adc_resume, (void*)NULL);

	pmu_sysactive_timer_init();
	pmu_set_sysactive_time(PMU_ADC_DEVICE, 5);
	pmu_release_wakelock(PMU_OS);

	vTaskDelete(NULL);
}

void main(void)
{
	if(xTaskCreate( (TaskFunction_t)adc_wakeup, "ADC WAKEUP DEMO", (2048/4), NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
			DBG_8195A("Cannot create ADC wakeup demo task\n\r");
	}

	vTaskStartScheduler();

	while(1);
}



