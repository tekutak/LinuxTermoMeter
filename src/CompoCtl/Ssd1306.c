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
#include "Ssd1306.h"
#include "I2cCtl.h"

/* -----------------------------------------------------------------------------
 Define
------------------------------------------------------------------------------*/
#define SSD1306_DEV_ADR (0x3C)

#define SSD1306_CONFIG_MUX_RATIO_CMD (0xA8)
#define SSD1306_CONFIG_MUX_RATIO_A (0x3F)
#define SSD1306_CONFIG_DISPLAY_OFFSET_CMD (0xD3)
#define SSD1306_CONFIG_DISPLAY_OFFSET_A (0x0)
#define SSD1306_CONFIG_DISPLAY_START_LINE (0x40)
#define SSD1306_CONFIG_SEGMENT_REMAP (0xA1)
#define SSD1306_CONFIG_COM_OUT_DIRECTION (0xC8)
#define SSD1306_CONFIG_COM_PIN_CONFIG_CMD (0xDA)
#define SSD1306_CONFIG_COM_PIN_CONFIG_A (0x12)
#define SSD1306_CONFIG_CONTRAST_CMD (0x81)
#define SSD1306_CONFIG_CONTRAST_A (0x7F)
#define SSD1306_CONFIG_ENTIRE_DISPLAY_ON (0xA4)
#define SSD1306_CONFIG_DISPLAY_PIX_MODE (0xA6)
#define SSD1306_CONFIG_DISPLAY_FREQ_CMD (0xD5)
#define SSD1306_CONFIG_DISPLAY_FREQ_A (0xF0)
#define SSD1306_CONFIG_ADDRESSING_MODE_CMD (0x20)
#define SSD1306_CONFIG_ADDRESSING_MODE_A (0x0)
#define SSD1306_CONFIG_CHARGE_PUMP_CMD (0x8D)
#define SSD1306_CONFIG_CHARGE_PUMP_A (0x14)
#define SSD1306_CONFIG_DISPLAY_ON_OFF (0xAF)

#define SSD1306_CTRL_BYTE_CMD_SINGLE (0b00000000)  //1つのコマンドセットのみ
#define SSD1306_CTRL_BYTE_CMD_STREAM (0b10000000)  //後ろにつづけて複数コマンドセット送る場合
#define SSD1306_CTRL_BYTE_DATA_SINGLE (0b01000000) //描画データのWriteのみ
#define SSD1306_CTRL_BYTE_DATA_STREAM (0b11000000) //描画データの後にその他のコマンドセットを送る場合

#define SSD1306_DRAW_CANVAS_PAYLOAD_SIZE (SSD1306_COLUMNS * SSD1306_PAGES + 1) //+1はコントロールバイト

/* -----------------------------------------------------------------------------
 Typedef
------------------------------------------------------------------------------*/
typedef enum
{
    e_CONFIG_MUX_RATIO_CMD,
    e_CONFIG_MUX_RATIO_A,
    e_CONFIG_DISPLAY_OFFSET_CMD,
    e_CONFIG_DISPLAY_OFFSET_A,
    e_CONFIG_DISPLAY_START_LINE,
    e_CONFIG_SEGMENT_REMAP,
    e_CONFIG_COM_OUT_DIRECTION,
    e_CONFIG_COM_PIN_CONFIG_CMD,
    e_CONFIG_COM_PIN_CONFIG_A,
    e_CONFIG_CONTRAST_CMD,
    e_CONFIG_CONTRAST_A,
    e_CONFIG_ENTIRE_DISPLAY_ON,
    e_CONFIG_DISPLAY_PIX_MODE,
    e_CONFIG_DISPLAY_FREQ_CMD,
    e_CONFIG_DISPLAY_FREQ_A,
    e_CONFIG_ADDRESSING_MODE_CMD,
    e_CONFIG_ADDRESSING_MODE_A,
    e_CONFIG_CHARGE_PUMP_CMD,
    e_CONFIG_CHARGE_PUMP_A,
    e_CONFIG_DISPLAY_ON_OFF,

    e_END_OF_CONFIG
} ET_SSD1306_CONFIG;
#define NUM_OF_SSD1306_CONFIG (e_END_OF_CONFIG)

/* -----------------------------------------------------------------------------
 Global
------------------------------------------------------------------------------*/
static U08 U08_Draw_Canvas_Payload[SSD1306_DRAW_CANVAS_PAYLOAD_SIZE] = {0};
static U08 *U08p_Draw_Canvas = &U08_Draw_Canvas_Payload[1];

static const U08 U08_SSD1306_Init_Config[NUM_OF_SSD1306_CONFIG] = {
    SSD1306_CONFIG_MUX_RATIO_CMD,
    SSD1306_CONFIG_MUX_RATIO_A,
    SSD1306_CONFIG_DISPLAY_OFFSET_CMD,
    SSD1306_CONFIG_DISPLAY_OFFSET_A,
    SSD1306_CONFIG_DISPLAY_START_LINE,
    SSD1306_CONFIG_SEGMENT_REMAP,
    SSD1306_CONFIG_COM_OUT_DIRECTION,
    SSD1306_CONFIG_COM_PIN_CONFIG_CMD,
    SSD1306_CONFIG_COM_PIN_CONFIG_A,
    SSD1306_CONFIG_CONTRAST_CMD,
    SSD1306_CONFIG_CONTRAST_A,
    SSD1306_CONFIG_ENTIRE_DISPLAY_ON,
    SSD1306_CONFIG_DISPLAY_PIX_MODE,
    SSD1306_CONFIG_DISPLAY_FREQ_CMD,
    SSD1306_CONFIG_DISPLAY_FREQ_A,
    SSD1306_CONFIG_ADDRESSING_MODE_CMD,
    SSD1306_CONFIG_ADDRESSING_MODE_A,
    SSD1306_CONFIG_CHARGE_PUMP_CMD,
    SSD1306_CONFIG_CHARGE_PUMP_A,
    SSD1306_CONFIG_DISPLAY_ON_OFF,
};

/* -----------------------------------------------------------------------------
 Prototype
------------------------------------------------------------------------------*/
static BOOL Ssd1306_Write(U08 *data, U32 length);
// static BOOL Ssd1306_Read(U08 *buf, U32 length);

/* -----------------------------------------------------------------------------
 Function   : Ssd1306_Init
 Memo       : SSD1306初期化
 Date       : 2021.08.28
------------------------------------------------------------------------------*/
BOOL Ssd1306_Init()
{
    BOOL status = OK;
    U08 cmd_buf[BUFSIZ];

    /* 動作設定 */
    cmd_buf[0] = SSD1306_CTRL_BYTE_CMD_SINGLE;
    U08 *p = &cmd_buf[1];
    for(U32 i=0; i < NUM_OF_SSD1306_CONFIG; i++)
    {
        *(p++) = U08_SSD1306_Init_Config[i];
    }
    Ssd1306_Write(cmd_buf, NUM_OF_SSD1306_CONFIG + 1);

    /* 画面初期化(黒塗り) */
    U08_Draw_Canvas_Payload[0] = SSD1306_CTRL_BYTE_DATA_SINGLE;
    Ssd1306_Write(U08_Draw_Canvas_Payload, SSD1306_DRAW_CANVAS_PAYLOAD_SIZE);

    return status;
}

/* -----------------------------------------------------------------------------
 Function   : Ssd1306_Write
 Memo       : SSD1306レジスタ書き込みラッパー
 Date       : 2021.08.28
------------------------------------------------------------------------------*/
static BOOL Ssd1306_Write(U08 *data, U32 length)
{
    BOOL status = OK;

    status = I2cCtl_Write(SSD1306_DEV_ADR, data, length);

    return status;
}

// static BOOL Ssd1306_Read(U08 *buf, U32 length)
// {
//     BOOL status = OK;

//     status = I2cCtl_Read(SSD1306_DEV_ADR, buf, length);

//     return status;
// }

/* -----------------------------------------------------------------------------
 Function   : Ssd1306_Get_Draw_Canvas
 Memo       : キャンバスアドレス取得
 Date       : 2021.08.28
------------------------------------------------------------------------------*/
extern U08* Ssd1306_Get_Draw_Canvas()
{
    return U08p_Draw_Canvas;
}
/* -----------------------------------------------------------------------------
 Function   : Ssd1306_Update_Frame
 Memo       : 描画バッファをSSD1306に送信して画面更新
 Date       : 2021.08.28
------------------------------------------------------------------------------*/
extern void Ssd1306_Update_Frame()
{
    U08_Draw_Canvas_Payload[0] = SSD1306_CTRL_BYTE_DATA_SINGLE;
    Ssd1306_Write(U08_Draw_Canvas_Payload, SSD1306_DRAW_CANVAS_PAYLOAD_SIZE);
}