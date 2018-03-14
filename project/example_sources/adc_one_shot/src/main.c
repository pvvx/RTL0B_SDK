/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "analogin_api.h"
#include <sys_api.h>


void adc_isr(void *Data)
{
  union x {
	u32 buf[2];
	u16 adc[4];
  } x;
	u32 isr = 0;
	u32 i = 0;

	isr = ADC_GetISR();
	if (isr & BIT_ADC_FIFO_THRESHOLD) {
		for(i = 0; i < sizeof(x.buf)/sizeof(u32); i++) {
			x.buf[i] = (u32)ADC_Read();
		}
	}

	ADC_INTClear();
	DBG_8195A("ADC0..3: 0x%04x, 0x%04x, 0x%04x, 0x%04x\n", x.adc[0], x.adc[1], x.adc[2], x.adc[3]);
}


VOID adc_one_shot (VOID)
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
	ADCInitStruct.ADC_OneShotTD = 8; /* means 4 times */
	ADC_Init(&ADCInitStruct);
	ADC_SetOneShot(ENABLE, 100, ADCInitStruct.ADC_OneShotTD); /* 100 will task 200ms */

	ADC_INTClear();
//	ADC_INTConfig(BIT_ADC_FIFO_FULL_EN|BIT_ADC_FIFO_RD_REQ_EN, ENABLE);
	ADC_Cmd(ENABLE);
	
	vTaskDelete(NULL);
}	


void main(void)
{
	if(xTaskCreate( (TaskFunction_t)adc_one_shot, "ADC ONE SHOT DEMO", (2048/4), NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
			DBG_8195A("Cannot create ADC one shot demo task\n\r");
	}

	vTaskStartScheduler();

	while(1);
}



