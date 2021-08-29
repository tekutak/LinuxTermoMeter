/* -----------------------------------------------------------------------------
 Include
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <time.h>

#include "common.h"
#include "I2cCtl.h"

/* -----------------------------------------------------------------------------
 Define
------------------------------------------------------------------------------*/
 /* デバイスアドレス */
#define BME280_DEV_ADR (0x76)

/* BME280 レジスタマップ */
#define BME280_REG_ADDRESS_CALIB_00	(0x88)
#define BME280_REG_ADDRESS_CALIB_01	(0x89)
#define BME280_REG_ADDRESS_CALIB_02	(0x8A)
#define BME280_REG_ADDRESS_CALIB_03	(0x8B)
#define BME280_REG_ADDRESS_CALIB_04	(0x8C)
#define BME280_REG_ADDRESS_CALIB_05	(0x8D)
#define BME280_REG_ADDRESS_CALIB_06	(0x8E)
#define BME280_REG_ADDRESS_CALIB_07	(0x8F)
#define BME280_REG_ADDRESS_CALIB_08	(0x90)
#define BME280_REG_ADDRESS_CALIB_09	(0x91)
#define BME280_REG_ADDRESS_CALIB_10	(0x92)
#define BME280_REG_ADDRESS_CALIB_11	(0x93)
#define BME280_REG_ADDRESS_CALIB_12	(0x94)
#define BME280_REG_ADDRESS_CALIB_13	(0x95)
#define BME280_REG_ADDRESS_CALIB_14	(0x96)
#define BME280_REG_ADDRESS_CALIB_15	(0x97)
#define BME280_REG_ADDRESS_CALIB_16	(0x98)
#define BME280_REG_ADDRESS_CALIB_17	(0x99)
#define BME280_REG_ADDRESS_CALIB_18	(0x9A)
#define BME280_REG_ADDRESS_CALIB_19	(0x9B)
#define BME280_REG_ADDRESS_CALIB_20	(0x9C)
#define BME280_REG_ADDRESS_CALIB_21	(0x9D)
#define BME280_REG_ADDRESS_CALIB_22	(0x9E)
#define BME280_REG_ADDRESS_CALIB_23	(0x9F)
#define BME280_REG_ADDRESS_CALIB_24	(0xA0)
#define BME280_REG_ADDRESS_CALIB_25	(0xA1)
#define BME280_REG_ADDRESS_ID	(0xD0)
#define BME280_REG_ADDRESS_RESET	(0xE0)
#define BME280_REG_ADDRESS_CALIB_26	(0xE1)
#define BME280_REG_ADDRESS_CALIB_27	(0xE2)
#define BME280_REG_ADDRESS_CALIB_28	(0xE3)
#define BME280_REG_ADDRESS_CALIB_29	(0xE4)
#define BME280_REG_ADDRESS_CALIB_30	(0xE5)
#define BME280_REG_ADDRESS_CALIB_31	(0xE6)
#define BME280_REG_ADDRESS_CALIB_32	(0xE7)
#define BME280_REG_ADDRESS_CALIB_33	(0xE8)
#define BME280_REG_ADDRESS_CALIB_34	(0xE9)
#define BME280_REG_ADDRESS_CALIB_35	(0xEA)
#define BME280_REG_ADDRESS_CALIB_36	(0xEB)
#define BME280_REG_ADDRESS_CALIB_37	(0xEC)
#define BME280_REG_ADDRESS_CALIB_38	(0xED)
#define BME280_REG_ADDRESS_CALIB_39	(0xEE)
#define BME280_REG_ADDRESS_CALIB_40	(0xEF)
#define BME280_REG_ADDRESS_CALIB_41	(0xF0)
#define BME280_REG_ADDRESS_CTRL_HUM	(0xF2)
#define BME280_REG_ADDRESS_STATUS	(0xF3)
#define BME280_REG_ADDRESS_CTRL_MEAS	(0xF4)
#define BME280_REG_ADDRESS_CONFIG	(0xF5)
#define BME280_REG_ADDRESS_PRESS_MSB	(0xF7)
#define BME280_REG_ADDRESS_PRESS_LSB	(0xF8)
#define BME280_REG_ADDRESS_PRESS_XLSB	(0xF9)
#define BME280_REG_ADDRESS_TEMP_MSB	(0xFA)
#define BME280_REG_ADDRESS_TEMP_LSB	(0xFB)
#define BME280_REG_ADDRESS_TEMP_XLSB	(0xFC)
#define BME280_REG_ADDRESS_HUM_MSB	(0xFD)
#define BME280_REG_ADDRESS_HUM_LSB	(0xFE)



/* 設定 */
#define BME280_SETTING_OSRS_H (0b101) //16
#define BME280_SETTING_OSRS_P (0b101) //16
#define BME280_SETTING_OSRS_T (0b101) //16

#define BME280_SETTING_MODE (0b01)	  //Forced mode
#define BME280_SETTING_T_SB (0b000)	  //This relates Normal mode only
#define BME280_SETTING_FILTER (0b100) //IIR Filter 16
#define BME280_SETTING_SPI3_EN (0b0)  //SPI3 wire Off

#define BME280_SETTING_REG_VAL_CTRL_HUM (BME280_SETTING_OSRS_H)
#define BME280_SETTING_REG_VAL_CTRL_MEAS ((BME280_SETTING_OSRS_T << 5) | (BME280_SETTING_OSRS_P << 2) | BME280_SETTING_MODE)
#define BME280_SETTING_REG_VAL_CONFIG ((BME280_SETTING_T_SB << 5) | (BME280_SETTING_FILTER << 5) | BME280_SETTING_SPI3_EN)

#define BME280_MEASURE_TIME (113) //1.25+(2.3*16)+(2.3*16+0.575)+(2.3*16+0.575)=112.8[ms]

/* -----------------------------------------------------------------------------
 Typedef
------------------------------------------------------------------------------*/
typedef enum
{
	e_BME280_CALIB_00_DIG_T1_LSB,
	e_BME280_CALIB_01_DIG_T1_MSB,
	e_BME280_CALIB_02_DIG_T2_LSB,
	e_BME280_CALIB_03_DIG_T2_MSB,
	e_BME280_CALIB_04_DIG_T3_LSB,
	e_BME280_CALIB_05_DIG_T3_MSB,
	e_BME280_CALIB_06_DIG_P1_LSB,
	e_BME280_CALIB_07_DIG_P1_MSB,
	e_BME280_CALIB_08_DIG_P2_LSB,
	e_BME280_CALIB_09_DIG_P2_MSB,
	e_BME280_CALIB_10_DIG_P3_LSB,
	e_BME280_CALIB_11_DIG_P3_MSB,
	e_BME280_CALIB_12_DIG_P4_LSB,
	e_BME280_CALIB_13_DIG_P4_MSB,
	e_BME280_CALIB_14_DIG_P5_LSB,
	e_BME280_CALIB_15_DIG_P5_MSB,
	e_BME280_CALIB_16_DIG_P6_LSB,
	e_BME280_CALIB_17_DIG_P6_MSB,
	e_BME280_CALIB_18_DIG_P7_LSB,
	e_BME280_CALIB_19_DIG_P7_MSB,
	e_BME280_CALIB_20_DIG_P8_LSB,
	e_BME280_CALIB_21_DIG_P8_MSB,
	e_BME280_CALIB_22_DIG_P9_LSB,
	e_BME280_CALIB_23_DIG_P9_MSB,
	e_BME280_CALIB_24,
	e_BME280_CALIB_25_DIG_H1,
	e_BME280_CALIB_26_DIG_H2_LSB,
	e_BME280_CALIB_27_DIG_H2_MSB,
	e_BME280_CALIB_28_DIG_H3,
	e_BME280_CALIB_29_DIG_H4_MSB,
	e_BME280_CALIB_30_DIG_H4_LSB_H5_LSB,
	e_BME280_CALIB_31_DIG_H5_MSB,
	e_BME280_CALIB_32_DIG_H6,
	e_BME280_CALIB_33,
	e_BME280_CALIB_34,
	e_BME280_CALIB_35,
	e_BME280_CALIB_36,
	e_BME280_CALIB_37,
	e_BME280_CALIB_38,
	e_BME280_CALIB_39,
	e_BME280_CALIB_40,
	e_BME280_CALIB_41,

	e_END_OF_BME280_CALIB
} ET_BME280_CALIB;
#define BME280_CALIB_DATA_NUM (e_END_OF_BME280_CALIB)

typedef enum
{
	e_BME280_READ_DATA_PRESS_MSB,
	e_BME280_READ_DATA_PRESS_LSB,
	e_BME280_READ_DATA_PRESS_XLSB,
	e_BME280_READ_DATA_TEMP_MSB,
	e_BME280_READ_DATA_TEMP_LSB,
	e_BME280_READ_DATA_TEMP_XLSB,
	e_BME280_READ_DATA_HUM_MSB,
	e_BME280_READ_DATA_HUM_LSB,

	e_END_OF_BME280_READ_DATA
} ET_BME280_READ_DATA;
#define BME280_READ_DATA_NUM (e_END_OF_BME280_READ_DATA)

/* -----------------------------------------------------------------------------
 Global
------------------------------------------------------------------------------*/
/* BME280から読みだしたキャリブレーションデータ */
static U08 U08_CalibData[BME280_CALIB_DATA_NUM] = {0};

/* 温度湿度気圧の補正係数 */
static U16 dig_T1 = 0;
static S16 dig_T2 = 0;
static S16 dig_T3 = 0;
static U16 dig_P1 = 0;
static S16 dig_P2 = 0;
static S16 dig_P3 = 0;
static S16 dig_P4 = 0;
static S16 dig_P5 = 0;
static S16 dig_P6 = 0;
static S16 dig_P7 = 0;
static S16 dig_P8 = 0;
static S16 dig_P9 = 0;
static U08 dig_H1 = 0;
static S16 dig_H2 = 0;
static U08 dig_H3 = 0;
static S16 dig_H4 = 0;
static S16 dig_H5 = 0;
static S08 dig_H6 = 0;

/* BME280から取得した温度湿度気圧 */
static F32 F32_Temperature = 0.0;
static F32 F32_Humidity = 0.0;
static F32 F32_Pressure = 0.0;

/* 換算用温度値 */
static S32 t_fine = 0;

/* -----------------------------------------------------------------------------
 Prototype
------------------------------------------------------------------------------*/
static void Bme280_Calibrate();
static BOOL Bme280_Write1byte(U08 reg_adr, U08 data);
static BOOL Bme280_Read(U08 reg_adr, void *buf, U32 length);
static S32 Bme280_CompensateT(S32 adc_T);
static U32 Bme280_CompensateP(S32 adc_P);
static U32 Bme280_CompensateH(S32 adc_H);

/* -----------------------------------------------------------------------------
 Function	: Bme280_Init
 Memo		: BME280のキャリブレーションと動作モードの設定
 Date		: 2021.08.28
------------------------------------------------------------------------------*/
BOOL Bme280_Init()
{
	BOOL status = OK;

	/* デバイスID取得 */
	U08 dev_id;
	status = Bme280_Read(BME280_REG_ADDRESS_ID, &dev_id, 1);
	if (status == NG)
	{
		printf("[BME280]id read failed.\n");
		return NG;
	}
	printf("[BME280]device id : 0x%08X\n", dev_id);

	/* キャリブレーションデータ読み出し */
	status = Bme280_Read(BME280_REG_ADDRESS_CALIB_00, U08_CalibData, BME280_CALIB_DATA_NUM);
	if (status == NG)
	{
		printf("[BME280]Calibration data read failed.\n");
		return NG;
	}
	/* キャリブレーション係数計算 */
	Bme280_Calibrate();

	/* 動作モード設定 */
	status = Bme280_Write1byte(BME280_REG_ADDRESS_CTRL_HUM, BME280_SETTING_REG_VAL_CTRL_HUM);
	if (status == NG)
	{
		printf("[BME280]CTRL_HUM set failed.\n");
		return NG;
	}
	status = Bme280_Write1byte(BME280_REG_ADDRESS_CTRL_MEAS, BME280_SETTING_REG_VAL_CTRL_MEAS);
	if (status == NG)
	{
		printf("[BME280]CTRL_MEAS set failed.\n");
		return NG;
	}
	status = Bme280_Write1byte(BME280_REG_ADDRESS_CONFIG, BME280_SETTING_REG_VAL_CONFIG);
	if (status == NG)
	{
		printf("[BME280]CONFIG set failed.\n");
		return NG;
	}

	return status;
}

/* -----------------------------------------------------------------------------
 Function	: Bme280_Calibrate
 Memo		: 温度・湿度・気圧計算時の補償係数算出
 Date		: 2021.08.28
------------------------------------------------------------------------------*/
static void Bme280_Calibrate()
{
	dig_T1 = ((U16)U08_CalibData[e_BME280_CALIB_01_DIG_T1_MSB] << 8) | (U16)U08_CalibData[e_BME280_CALIB_00_DIG_T1_LSB];
	dig_T2 = ((S16)U08_CalibData[e_BME280_CALIB_03_DIG_T2_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_02_DIG_T2_LSB];
	dig_T3 = ((S16)U08_CalibData[e_BME280_CALIB_05_DIG_T3_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_04_DIG_T3_LSB];
	dig_P1 = ((U16)U08_CalibData[e_BME280_CALIB_07_DIG_P1_MSB] << 8) | (U16)U08_CalibData[e_BME280_CALIB_06_DIG_P1_LSB];
	dig_P2 = ((S16)U08_CalibData[e_BME280_CALIB_09_DIG_P2_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_08_DIG_P2_LSB];
	dig_P3 = ((S16)U08_CalibData[e_BME280_CALIB_11_DIG_P3_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_10_DIG_P3_LSB];
	dig_P4 = ((S16)U08_CalibData[e_BME280_CALIB_13_DIG_P4_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_12_DIG_P4_LSB];
	dig_P5 = ((S16)U08_CalibData[e_BME280_CALIB_15_DIG_P5_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_14_DIG_P5_LSB];
	dig_P6 = ((S16)U08_CalibData[e_BME280_CALIB_17_DIG_P6_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_16_DIG_P6_LSB];
	dig_P7 = ((S16)U08_CalibData[e_BME280_CALIB_19_DIG_P7_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_18_DIG_P7_LSB];
	dig_P8 = ((S16)U08_CalibData[e_BME280_CALIB_21_DIG_P8_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_20_DIG_P8_LSB];
	dig_P9 = ((S16)U08_CalibData[e_BME280_CALIB_23_DIG_P9_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_22_DIG_P9_LSB];
	dig_H1 = U08_CalibData[e_BME280_CALIB_25_DIG_H1];
	dig_H2 = ((S16)U08_CalibData[e_BME280_CALIB_27_DIG_H2_MSB] << 8) | (S16)U08_CalibData[e_BME280_CALIB_26_DIG_H2_LSB];
	dig_H3 = U08_CalibData[e_BME280_CALIB_28_DIG_H3];
	dig_H4 = ((S16)U08_CalibData[e_BME280_CALIB_29_DIG_H4_MSB] << 4) | (S16)(U08_CalibData[e_BME280_CALIB_30_DIG_H4_LSB_H5_LSB] & 0x0F);
	dig_H5 = ((S16)U08_CalibData[e_BME280_CALIB_31_DIG_H5_MSB] << 4) | (S16)((U08_CalibData[e_BME280_CALIB_30_DIG_H4_LSB_H5_LSB] & 0xF0) >> 4);
	dig_H6 = (S16)U08_CalibData[e_BME280_CALIB_32_DIG_H6];
}

/* -----------------------------------------------------------------------------
 Function	: Bme280_Write1byte
 Memo		: BME280レジスタ書き込み(1byte)ラッパー
 Date		: 2021.08.28
------------------------------------------------------------------------------*/
static BOOL Bme280_Write1byte(U08 reg_adr, U08 data)
{
	BOOL status = OK;
	U08 buf[2];

	buf[0] = reg_adr;
	buf[1] = data;
	status = I2cCtl_Write(BME280_DEV_ADR, buf, 2);

	return status;
}

/* -----------------------------------------------------------------------------
 Function	: Bme280_Read
 Memo		: BME280レジスタ読み出しラッパー
 Date		: 2021.08.28
------------------------------------------------------------------------------*/
static BOOL Bme280_Read(U08 reg_adr, void *buf, const U32 length)
{
	BOOL status = OK;

	status = I2cCtl_Write(BME280_DEV_ADR, &reg_adr, 1);
	if(status == NG)
	{
		printf("[BME280]read address set failed.\n");
		return NG;
	}
	status = I2cCtl_Read(BME280_DEV_ADR, buf, length);
	if(status == NG)
	{
		printf("[BME280]read failed.\n");
		return NG;
	}

	return status;
}

/* -----------------------------------------------------------------------------
 Function	: Bme280_Compensate
 Memo		: レジスタ読み出し値から温度・湿度・気圧に各種変換＆補償
 Date		: 2021.08.28
------------------------------------------------------------------------------*/
static S32 Bme280_CompensateT(S32 adc_T)
{
	S32 var1;
	S32 var2;
	static S32 t_fine;
	S32 T;
	var1 = ((((adc_T >> 3) - ((S32)dig_T1 << 1))) * ((S32)dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((S32)dig_T1)) * ((adc_T >> 4) - ((S32)dig_T1))) >> 12) * ((S32)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}
static U32 Bme280_CompensateP(S32 adc_P)
{
	S64 var1, var2, p;
	var1 = ((S64)t_fine) - 128000;
	var2 = var1 * var1 * (S64)dig_P6;
	var2 = var2 + ((var1 * (S64)dig_P5) << 17);
	var2 = var2 + (((S64)dig_P4) << 35);
	var1 = ((var1 * var1 * (S64)dig_P3) >> 8) + ((var1 * (S64)dig_P2) << 12);
	var1 = (((((S64)1) << 47) + var1)) * ((S64)dig_P1) >> 33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}

	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((S64)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((S64)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((S64)dig_P7) << 4);
	return (U32)p;
}
static U32 Bme280_CompensateH(S32 adc_H)
{
	S32 v_x1_u32r;
	v_x1_u32r = (t_fine - ((S32)76800));
	v_x1_u32r = (((((adc_H << 14) - (((S32)dig_H4) << 20) - (((S32)dig_H5) * v_x1_u32r)) + ((S32)16384)) >> 15) * (((((((v_x1_u32r * ((S32)dig_H6)) >> 10) * (((v_x1_u32r * ((S32)dig_H3)) >> 11) + ((S32)32768))) >> 10) + ((S32)2097152)) * ((S32)dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((S32)dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (U32)(v_x1_u32r >> 12);
}

/* -----------------------------------------------------------------------------
 Function	: Bme280_Update
 Memo		: 温度・湿度・気圧の計測実施
 Date		: 2021.08.28
------------------------------------------------------------------------------*/
void Bme280_Update()
{
	U08 buf;
	BOOL status = OK;
	U08 read_data[BME280_READ_DATA_NUM] = {0};

	/* 測定開始 */
	buf = BME280_SETTING_REG_VAL_CTRL_MEAS;
	status = Bme280_Write1byte(BME280_REG_ADDRESS_CTRL_MEAS, buf);
	if (status == NG)
	{
		printf("[BME280]Forced Measure failed.\n");
		return;
	}

	//113ms sleep
	struct timespec req = {0, BME280_MEASURE_TIME * 1000 * 1000};
	struct timespec rem;
	nanosleep(&req, &rem);

	/* 読み出し */
	status = Bme280_Read(BME280_REG_ADDRESS_PRESS_MSB, read_data, BME280_READ_DATA_NUM);
	if (status == NG)
	{
		printf("[BME280]Data readout failed.\n");
		return;
	}

	/* 変換 */
	S32 adc_t = (S32)((((U32)read_data[e_BME280_READ_DATA_TEMP_MSB] << 16) | ((U32)read_data[e_BME280_READ_DATA_TEMP_LSB] << 8) | (U32)read_data[e_BME280_READ_DATA_TEMP_XLSB]) >> 4);
	F32_Temperature = (F32)Bme280_CompensateT(adc_t) * 0.01;
	S32 adc_p = (S32)((((U32)read_data[e_BME280_READ_DATA_PRESS_MSB] << 16) | ((U32)read_data[e_BME280_READ_DATA_PRESS_LSB] << 8) | (U32)read_data[e_BME280_READ_DATA_PRESS_XLSB]) >> 4);
	F32_Pressure = (F32)Bme280_CompensateP(adc_p) / 256.0 * 0.01;
	S32 adc_h = (S32)(((U32)read_data[e_BME280_READ_DATA_HUM_MSB] << 8) | (U32)read_data[e_BME280_READ_DATA_HUM_LSB]);
	F32_Humidity = (F32)Bme280_CompensateH(adc_h) / 1024.0;
}

/* -----------------------------------------------------------------------------
 Function	: Bme280_Get_TempHumidPress
 Memo		: 温度・湿度・気圧の取得
 Date		: 2021.08.28
------------------------------------------------------------------------------*/
void Bme280_Get_TempHumidPress(F32 *temperature, F32 *humidity, F32 *pressure)
{
	*temperature = F32_Temperature;
	*humidity = F32_Humidity;
	*pressure = F32_Pressure;
}