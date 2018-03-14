/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "gpio_api.h"   // mbed
#include "gpio_irq_api.h"   // mbed
#include "gpio_irq_ex_api.h"   // mbed
#include "sleep_ex_api.h"
#include "sys_api.h"
#include "diag.h"
#include "main.h"
#include "rtc_api.h"
//#include "send_count.h"

#define GPIO_WAKE_PIN		PA_5
#define SEND_PERIOD		(30*60) // sec

gpio_t gpio_wake;
volatile uint32_t pulse_count = 0;
volatile uint32_t rtc_count = 0;
time_t send_seconds = 1510433552;  // Set RTC 11.11.2017 @ 20:52:32

VOID RTC_Handler(u32 Data)
{
	 /*clear alarm flag*/
	 RTC_AlarmClear();
	 rtc_count++;
}

void rtc_irq_en(time_t sec)
{
	RTC_AlarmTypeDef RTC_AlarmStruct_temp;
    struct tm *timeinfo;
	time_t seconds = rtc_read() + sec;
  	timeinfo = localtime(&seconds);
	
	RTC_AlarmStructInit(&RTC_AlarmStruct_temp);
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Days = timeinfo->tm_mday;
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Hours = timeinfo->tm_hour;
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Minutes = timeinfo->tm_min;
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Seconds = timeinfo->tm_sec;
	RTC_AlarmStruct_temp.RTC_AlarmMask = RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;
	RTC_AlarmStruct_temp.RTC_Alarm2Mask = RTC_Alarm2Mask_Days;

	RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStruct_temp);
	RTC_AlarmCmd(ENABLE);
	
	InterruptRegister((IRQ_FUN)RTC_Handler, RTC_IRQ, NULL, 4);
	InterruptEn(RTC_IRQ, 4);
    DBG_8195A("Time set = %d-%d-%d %d:%d:%d\r\n", 
    	timeinfo->tm_year + 1900, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour,
        timeinfo->tm_min,timeinfo->tm_sec);
	
}

gpio_irq_t gpio_level;
int current_level = IRQ_LOW;

void gpio_level_irq_handler (uint32_t id, gpio_irq_event event)
{
    uint32_t *level = (uint32_t *) id;

    // Disable level irq because the irq will keep triggered when it keeps in same level.
    gpio_irq_disable(&gpio_level);

    // make some software de-bounce here if the signal source is not stable.

    if (*level == IRQ_LOW )
    {
        DBG_8195A("low level event\r\n");

        // Change to listen to high level event
        *level = IRQ_HIGH;
        gpio_irq_set(&gpio_level, IRQ_HIGH, 1);
        gpio_irq_enable(&gpio_level);
		pulse_count++;
    }
    else if (*level == IRQ_HIGH)
    {
        DBG_8195A("high level event\r\n");

        // Change to listen to low level event
        *level = IRQ_LOW;
        gpio_irq_set(&gpio_level, IRQ_LOW, 1);
        gpio_irq_enable(&gpio_level);
    }
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void psm_sleep(void)
{
    //int IsDramOn = 1;

    rtc_init();
    rtc_write(send_seconds);  // Set RTC time to Wed, 28 Oct 2009 11:35:37
	send_seconds += SEND_PERIOD;
  	rtc_irq_en(SEND_PERIOD);

	DBG_INFO_MSG_OFF(_DBG_GPIO_);

	// Initial pin as interrupt source
	gpio_init(&gpio_wake, GPIO_WAKE_PIN);
    gpio_dir(&gpio_wake, PIN_INPUT);    // Direction: Input
    gpio_mode(&gpio_wake, PullUp);     // Pull up
//    sys_log_uart_off();

	while(1) {
			
            sleep_ex(SLEEP_WAKEUP_BY_GPIO_INT, 0); // sleep_ex can't be put in irq handler  | SLEEP_WAKEUP_BY_RTC
//			if(!gpio_read(&gpio_wake)) 
			  pulse_count++;
			if(rtc_count) {
			  	rtc_count = 0;
//	            sys_log_uart_on();
		        time_t seconds = rtc_read();
	 			DBG_8195A("RTC time: %d, Count = %d\n\r", seconds, pulse_count);  
				if(send_seconds < seconds) {
					send_seconds += SEND_PERIOD;
					if(gpio_read(&gpio_wake)) {
						current_level = IRQ_LOW;
						gpio_irq_init(&gpio_level, GPIO_WAKE_PIN, gpio_level_irq_handler, (uint32_t)(&current_level));
						gpio_irq_set(&gpio_level, IRQ_LOW, 1);
					} else {
						current_level = IRQ_HIGH;
						gpio_irq_init(&gpio_level, GPIO_WAKE_PIN, gpio_level_irq_handler, (uint32_t)(&current_level));
			        	gpio_irq_set(&gpio_level, IRQ_HIGH, 1);
					}
//					send_count();
					DBG_8195A("New TimeSend = %d\n\r", send_seconds);
				    gpio_irq_disable(&gpio_level);
    			}
//				sys_log_uart_off();
			}
	}
	//vTaskDelete(NULL);
}

void main(void)
{
	// create demo Task
	if(xTaskCreate( (TaskFunction_t)psm_sleep, "WRK", (2048/4), 0, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
		DBG_8195A("Cannot create uart wakeup demo task\n\r");
		goto end_demo;
	}
#if defined(CONFIG_KERNEL) && !TASK_SCHEDULER_DISABLED
	#ifdef PLATFORM_FREERTOS
	vTaskStartScheduler();
	#endif
#else
	#error !!!Need FREERTOS!!!
#endif
end_demo:	
//	while(1);
}


