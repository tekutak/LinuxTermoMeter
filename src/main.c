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

/* -----------------------------------------------------------------------------
 Prototype
------------------------------------------------------------------------------*/
BOOL Init();

/* -----------------------------------------------------------------------------
 Function   : main
 Memo       : 
 Date       : 2021.09.12
------------------------------------------------------------------------------*/
int main()
{
	BOOL ret;

	/* 初期化 */
	ret = Init();
	if (ret == NG)
	{
		printf("Init Error.\n");
		return -1;
	}

	/* 温度計 */
	for (;;)
	{
		Thermo_Measure();

		F32 t = Thermo_Get_Temperature();
		F32 h = Thermo_Get_Humidity();
		F32 p = Thermo_Get_Pressure();
		printf("Temperature:%.1f\n", t);
		printf("Humidity:%.1f\n", h);
		printf("Pressure:%.1f\n", p);

		Paint_Draw_ThermoMeter(t, h, p);
		Paint_Flush_Canvas();

	}

	return 0;
}

/* -----------------------------------------------------------------------------
 Function   : Init
 Memo       : 
 Date       : 2021.09.12
------------------------------------------------------------------------------*/
BOOL Init()
{
	BOOL ret;

	/* 初期化 */
	ret = I2cCtl_Init();
	if (ret == NG)
	{
		printf("I2cCtl_Init Error.\n");
		return NG;
	}
	ret = Bme280_Init();
	if (ret == NG)
	{
		printf("Bme280_Init Error.\n");
		return NG;
	}
	ret = Ssd1306_Init();
	if (ret == NG)
	{
		printf("Ssd1306_Init Error.\n");
		return NG;
	}
	ret = Thermo_Init();
	if (ret == NG)
	{
		printf("Thermo_Init Error.\n");
		return NG;
	}
	ret = Paint_Init();
	if (ret == NG)
	{
		printf("Paint_Init Error.\n");
		return NG;
	}

	return ret;
}