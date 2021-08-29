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


int main()
{
	BOOL ret;

	/* 初期化 */
	ret = I2cCtl_Init();
	ret = Bme280_Init();
	ret = Ssd1306_Init();

	if (ret == NG)
	{
		printf("[BME280]Init Error.\n");
		return -1;
	}

	for (;;)
	{
		Bme280_Update();

		F32 t, h, p;
		Bme280_Get_TempHumidPress(&t, &h, &p);
		printf("Temperature:%f\n", t);
		printf("Humidity:%f\n", h);
		printf("Pressure:%f\n", p);

		Paint_Draw_ThermoMeter(t, h, p);

		// sleep(3);
	}

	return 0;
}