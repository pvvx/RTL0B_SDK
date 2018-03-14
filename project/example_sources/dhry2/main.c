//------------------------------------------------------------

#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"
#include "dhry2/dhry.h"

#define delay(ms) vTaskDelay(ms) 

uint32_t millis( void ) {
    return (__get_IPSR() == 0) ? xTaskGetTickCount() : xTaskGetTickCountFromISR();
}

int c_printf(const char *fmt, ...);

//------------------------------------------------------------------------

void test(void)
{
	// https://www.eevblog.com/forum/microcontrollers/dhrystone-2-1-on-mcus/
	int Number_Of_Runs = 1000000;
	float CpuClkMhz = (float) PLATFORM_CLOCK / 1000000.0;
	while(1) {
        delay(1000);
		uint32_t User_Time = dhry_main(Number_Of_Runs);
		c_printf ("RunTime %u ms, CPU CLK %f MHz, Run Cycles %u\n", User_Time, CpuClkMhz, Number_Of_Runs);
		
        if (User_Time < Too_Small_Time) {
          c_printf ("Measured time too small to obtain meaningful results\n");
          c_printf ("Please increase number of runs\n");
          c_printf ("\n");
        } else {
      	  float Microseconds = (float) User_Time * 1000.0 / (float) Number_Of_Runs;
      	  float Dhrystones_Per_Second = (float) Number_Of_Runs / (float) User_Time * 1000.0;
      	  c_printf ("Microseconds for one run through Dhrystone: ");
      	  c_printf ("%f \n", Microseconds);
      	  c_printf ("Dhrystones per Second:                      ");
      	  c_printf ("%f \n", Dhrystones_Per_Second);
      	  c_printf ("Dhrystones per MHz:                         ");
      	  c_printf ("%f \n", Dhrystones_Per_Second / CpuClkMhz);
      	  c_printf ("%f DMIPS, %f DMIPS/MHz ?\n", Dhrystones_Per_Second / 1757.0, Dhrystones_Per_Second / 1757.0 / CpuClkMhz);
      	  c_printf ("\n");
        };
	}
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	if(xTaskCreate( (TaskFunction_t)test, "Test", (2048), NULL, (tskIDLE_PRIORITY + 3), NULL)!= pdPASS) {
			DBG_8195A("Cannot create Test task\n\r");
	}

	vTaskStartScheduler();

	while(1);
}
