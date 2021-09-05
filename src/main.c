/* -----------------------------------------------------------------------------
 Include
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include "common.h"
#include "Bme280.h"
#include "I2cCtl.h"
#include "Ssd1306.h"
#include "Paint.h"
#include "Thermo.h"


int main()
{
	BOOL ret;

	/* 初期化 */
	ret = I2cCtl_Init();
	ret = Bme280_Init();
	ret = Ssd1306_Init();
	ret = Paint_Init();
	ret = Thermo_Init();

	if (ret == NG)
	{
		printf("[BME280]Init Error.\n");
		return -1;
	}

	for (;;)
	{
		Thermo_Measure();

		F32 t = Thermo_Get_Temperature();
		F32 h = Thermo_Get_Humidity();
		F32 p = Thermo_Get_Pressure();
		printf("Temperature:%f\n", t);
		printf("Humidity:%f\n", h);
		printf("Pressure:%f\n", p);

		Paint_Draw_ThermoMeter(t, h, p);
		Paint_Flush_Canvas();

		// sleep(3);
	}

	return 0;
}