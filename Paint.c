/* -----------------------------------------------------------------------------
 Include
 -----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "common.h"
#include "Paint.h"
#include "Ssd1306.h"
#include "Font.h"
/* -----------------------------------------------------------------------------
 Define
------------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
 Typedef
------------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
 Global
------------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
 Prototype
------------------------------------------------------------------------------*/
static void Paint_ClearCanvas();
static void Paint_Draw_Temperature(F32 temperature);
static void Paint_Draw_Humidity(F32 humidity);
static void Paint_Draw_Pressure(F32 pressure);
static void Paint_DrawLargeFont(U08 flg_blk_valid, U08 offset_x, U08 offset_y);
static void Paint_DrawLargeDot(U08 offset_x, U08 offset_y);
static void Paint_DrawLargeHalfFont(U08 flg_blk_valid, U08 offset_x, U08 offset_y);
static void Paint_DrawLargeHalfDeg(U08 offset_x, U08 offset_y);
static void Paint_DrawSemiHalfFont(U08 flg_blk_valid, U08 offset_x, U08 offset_y);
static void Paint_DrawSemiHalfHPA(U08 offset_x, U08 offset_y);
static void Paint_DrawSemiHalfPercent(U08 offset_x, U08 offset_y);
static void Paint_DrawSemiHalfDot(U08 offset_x, U08 offset_y);

/* -----------------------------------------------------------------------------
 Function   : Paint_Draw_ThermoMeter
 Memo       : 温度・湿度・気圧の画面描画
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
void Paint_Draw_ThermoMeter(F32 temperature, F32 humidity, F32 pressure)
{
    Paint_ClearCanvas();
    Paint_Draw_Temperature(temperature);
    Paint_Draw_Humidity(humidity);
    Paint_Draw_Pressure(pressure);
    Ssd1306_Update_Frame();
}

/* -----------------------------------------------------------------------------
 Function   : Paint_Draw_Temperature
 Memo       : 温度の描画
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_Draw_Temperature(F32 temperature)
{
    const U08 offset_x = 0;
    const U08 offset_y = 2;
    const U08 font_width = 30;
    U08 pos_x = offset_x;
    temperature *= 10.0;
    S32 value = (S32)temperature;
    S32 digit_1 = value % 10;
    value /= 10;
    S32 digit_2 = value % 10;
    value /= 10;
    S32 digit_3 = value % 10;

    Paint_DrawLargeFont(U08a_font_map[digit_3], pos_x, offset_y);
    pos_x += font_width;
    Paint_DrawLargeFont(U08a_font_map[digit_2], pos_x, offset_y);
    pos_x += font_width;
    Paint_DrawLargeDot(pos_x - 2, 7);
    pos_x += 7;
    Paint_DrawLargeHalfFont(U08a_font_map[digit_1], pos_x, 5);
    pos_x += 18;
    Paint_DrawLargeHalfDeg(pos_x, 6);
}
/* -----------------------------------------------------------------------------
 Function   : Paint_Draw_Humidity
 Memo       : 湿度の描画
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_Draw_Humidity(F32 humidity)
{
    const U08 offset_x = 75;
    const U08 offset_y = 3;
    const U08 font_width = 10;
    U08 pos_x = offset_x;
    humidity *= 10.0;
    S32 value = (S32)humidity;
    S32 digit_1 = value % 10;
    value /= 10;
    S32 digit_2 = value % 10;
    value /= 10;
    S32 digit_3 = value % 10;
    value /= 10;
    S32 digit_4 = value % 10;

    if (digit_4 > 0)
    {
        Paint_DrawSemiHalfFont(U08a_font_map[digit_4], offset_x, offset_y);
    }
    pos_x += font_width;
    Paint_DrawSemiHalfFont(U08a_font_map[digit_3], pos_x, offset_y);
    pos_x += font_width;
    Paint_DrawSemiHalfFont(U08a_font_map[digit_2], pos_x, offset_y);
    pos_x += font_width;
    Paint_DrawSemiHalfDot(pos_x, offset_y + 1);
    pos_x += 3;
    Paint_DrawSemiHalfFont(U08a_font_map[digit_1], pos_x, offset_y);
    pos_x += font_width + 2;
    Paint_DrawSemiHalfPercent(127 - 10, offset_y + 2);
}
/* -----------------------------------------------------------------------------
 Function   : Paint_Draw_Pressure
 Memo       : 気圧の描画
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_Draw_Pressure(F32 pressure)
{
    const U08 offset_x = 65;
    const U08 offset_y = 0;
    const U08 font_width = 10;
    U08 pos_x = offset_x;
    pressure *= 10.0;
    S32 value = (S32)pressure;
    S32 digit_1 = value % 10;
    value /= 10;
    S32 digit_2 = value % 10;
    value /= 10;
    S32 digit_3 = value % 10;
    value /= 10;
    S32 digit_4 = value % 10;
    value /= 10;
    S32 digit_5 = value % 10;

    if (digit_5 > 0)
    {
        Paint_DrawSemiHalfFont(U08a_font_map[digit_5], offset_x, offset_y);
    }
    pos_x += font_width;
    Paint_DrawSemiHalfFont(U08a_font_map[digit_4], pos_x, offset_y);
    pos_x += font_width;
    Paint_DrawSemiHalfFont(U08a_font_map[digit_3], pos_x, offset_y);
    pos_x += font_width;
    Paint_DrawSemiHalfFont(U08a_font_map[digit_2], pos_x, offset_y);
    pos_x += font_width;
    Paint_DrawSemiHalfDot(pos_x, offset_y + 1);
    pos_x += 3;
    Paint_DrawSemiHalfFont(U08a_font_map[digit_1], pos_x, offset_y);
    pos_x += font_width + 2;
    Paint_DrawSemiHalfHPA(127 - 15, offset_y + 2);
}
/* -----------------------------------------------------------------------------
 Function   : Paint_ClearCanvas
 Memo       : キャンバスのクリア
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_ClearCanvas()
{
    U08 *p_canvas = Ssd1306_Get_Draw_Canvas();
    memset(p_canvas, 0x0, SSD1306_COLUMNS * SSD1306_PAGES);
}
/* -----------------------------------------------------------------------------
 Function   : Paint_DrawLargeFont
 Memo       : ラージフォント描画
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_DrawLargeFont(U08 flg_blk_valid, U08 offset_x, U08 offset_y)
{
    U08 *disp_buffer_org = Ssd1306_Get_Draw_Canvas() + offset_x + offset_y * SSD1306_COLUMNS;
    U08 *disp_buffer_temp;
    U08 *disp_buffer;
    U08 row, column;
    U08 width, height;

    if (flg_blk_valid & e_FONT_LARGE_BLK_1)
    {
        row = St_large_font_blk_1.u08v_start_y;
        column = St_large_font_blk_1.u08v_start_x;
        width = St_large_font_blk_1.u08v_width;
        height = St_large_font_blk_1.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_font_blk_1[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_BLK_2)
    {
        row = St_large_font_blk_2.u08v_start_y;
        column = St_large_font_blk_2.u08v_start_x;
        width = St_large_font_blk_2.u08v_width;
        height = St_large_font_blk_2.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_font_blk_2[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_BLK_3)
    {
        row = St_large_font_blk_3.u08v_start_y;
        column = St_large_font_blk_3.u08v_start_x;
        width = St_large_font_blk_3.u08v_width;
        height = St_large_font_blk_3.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_font_blk_3[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_BLK_4)
    {
        row = St_large_font_blk_4.u08v_start_y;
        column = St_large_font_blk_4.u08v_start_x;
        width = St_large_font_blk_4.u08v_width;
        height = St_large_font_blk_4.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_font_blk_4[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_BLK_5)
    {
        row = St_large_font_blk_5.u08v_start_y;
        column = St_large_font_blk_5.u08v_start_x;
        width = St_large_font_blk_5.u08v_width;
        height = St_large_font_blk_5.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_font_blk_5[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_BLK_6)
    {
        row = St_large_font_blk_6.u08v_start_y;
        column = St_large_font_blk_6.u08v_start_x;
        width = St_large_font_blk_6.u08v_width;
        height = St_large_font_blk_6.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_font_blk_6[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_BLK_7)
    {
        row = St_large_font_blk_7.u08v_start_y;
        column = St_large_font_blk_7.u08v_start_x;
        width = St_large_font_blk_7.u08v_width;
        height = St_large_font_blk_7.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_font_blk_7[j * width + i];
            }
        }
    }
}
/* -----------------------------------------------------------------------------
 Function   : Paint_DrawLargeDot
 Memo       : ラージフォント描画(ドット)
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_DrawLargeDot(U08 offset_x, U08 offset_y)
{
    U08 *disp_buffer_org = Ssd1306_Get_Draw_Canvas() + offset_x + offset_y * SSD1306_COLUMNS;
    U08 *disp_buffer_temp;
    U08 *disp_buffer;
    U08 row, column;
    U08 width, height;
    row = St_large_font_dot.u08v_start_y;
    column = St_large_font_dot.u08v_start_x;
    width = St_large_font_dot.u08v_width;
    height = St_large_font_dot.u08v_height;
    disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
    for (U08 j = 0; j < height; j++)
    {
        disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
        for (U08 i = 0; i < width; i++)
        {
            *(disp_buffer++) |= U08a_large_font_dot[j * width + i];
        }
    }
}
/* -----------------------------------------------------------------------------
 Function   : Paint_DrawLargeDot
 Memo       : ラージハーフフォント描画
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_DrawLargeHalfFont(U08 flg_blk_valid, U08 offset_x, U08 offset_y)
{
    U08 *disp_buffer_org = Ssd1306_Get_Draw_Canvas() + offset_x + offset_y * SSD1306_COLUMNS;
    U08 *disp_buffer_temp;
    U08 *disp_buffer;
    U08 row, column;
    U08 width, height;

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_1)
    {
        row = St_large_half_font_blk_1.u08v_start_y;
        column = St_large_half_font_blk_1.u08v_start_x;
        width = St_large_half_font_blk_1.u08v_width;
        height = St_large_half_font_blk_1.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_half_font_blk_1[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_2)
    {
        row = St_large_half_font_blk_2.u08v_start_y;
        column = St_large_half_font_blk_2.u08v_start_x;
        width = St_large_half_font_blk_2.u08v_width;
        height = St_large_half_font_blk_2.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_half_font_blk_2[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_3)
    {
        row = St_large_half_font_blk_3.u08v_start_y;
        column = St_large_half_font_blk_3.u08v_start_x;
        width = St_large_half_font_blk_3.u08v_width;
        height = St_large_half_font_blk_3.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_half_font_blk_3[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_4)
    {
        row = St_large_half_font_blk_4.u08v_start_y;
        column = St_large_half_font_blk_4.u08v_start_x;
        width = St_large_half_font_blk_4.u08v_width;
        height = St_large_half_font_blk_4.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_half_font_blk_4[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_5)
    {
        row = St_large_half_font_blk_5.u08v_start_y;
        column = St_large_half_font_blk_5.u08v_start_x;
        width = St_large_half_font_blk_5.u08v_width;
        height = St_large_half_font_blk_5.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_half_font_blk_5[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_6)
    {
        row = St_large_half_font_blk_6.u08v_start_y;
        column = St_large_half_font_blk_6.u08v_start_x;
        width = St_large_half_font_blk_6.u08v_width;
        height = St_large_half_font_blk_6.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_half_font_blk_6[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_7)
    {
        row = St_large_half_font_blk_7.u08v_start_y;
        column = St_large_half_font_blk_7.u08v_start_x;
        width = St_large_half_font_blk_7.u08v_width;
        height = St_large_half_font_blk_7.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_large_half_font_blk_7[j * width + i];
            }
        }
    }
}
/* -----------------------------------------------------------------------------
 Function   : Paint_DrawLargeHalfDeg
 Memo       : ラージハーフフォント描画(℃)
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_DrawLargeHalfDeg(U08 offset_x, U08 offset_y)
{
    U08 *disp_buffer_org = Ssd1306_Get_Draw_Canvas() + offset_x + offset_y * SSD1306_COLUMNS;
    U08 *disp_buffer_temp;
    U08 *disp_buffer;
    U08 row, column;
    U08 width, height;
    row = St_large_half_font_deg.u08v_start_y;
    column = St_large_half_font_deg.u08v_start_x;
    width = St_large_half_font_deg.u08v_width;
    height = St_large_half_font_deg.u08v_height;
    disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
    for (U08 j = 0; j < height; j++)
    {
        disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
        for (U08 i = 0; i < width; i++)
        {
            *(disp_buffer++) |= U08a_large_half_font_deg[j * width + i];
        }
    }
}
/* -----------------------------------------------------------------------------
 Function   : Paint_DrawSemiHalfFont
 Memo       : セミハーフフォント描画
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_DrawSemiHalfFont(U08 flg_blk_valid, U08 offset_x, U08 offset_y)
{
    U08 *disp_buffer_org = Ssd1306_Get_Draw_Canvas() + offset_x + offset_y * SSD1306_COLUMNS;
    U08 *disp_buffer_temp;
    U08 *disp_buffer;
    U08 row, column;
    U08 width, height;

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_1)
    {
        row = St_semi_half_font_blk_1.u08v_start_y;
        column = St_semi_half_font_blk_1.u08v_start_x;
        width = St_semi_half_font_blk_1.u08v_width;
        height = St_semi_half_font_blk_1.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_semi_half_font_blk_1[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_2)
    {
        row = St_semi_half_font_blk_2.u08v_start_y;
        column = St_semi_half_font_blk_2.u08v_start_x;
        width = St_semi_half_font_blk_2.u08v_width;
        height = St_semi_half_font_blk_2.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_semi_half_font_blk_2[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_3)
    {
        row = St_semi_half_font_blk_3.u08v_start_y;
        column = St_semi_half_font_blk_3.u08v_start_x;
        width = St_semi_half_font_blk_3.u08v_width;
        height = St_semi_half_font_blk_3.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_semi_half_font_blk_3[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_4)
    {
        row = St_semi_half_font_blk_4.u08v_start_y;
        column = St_semi_half_font_blk_4.u08v_start_x;
        width = St_semi_half_font_blk_4.u08v_width;
        height = St_semi_half_font_blk_4.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_semi_half_font_blk_4[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_5)
    {
        row = St_semi_half_font_blk_5.u08v_start_y;
        column = St_semi_half_font_blk_5.u08v_start_x;
        width = St_semi_half_font_blk_5.u08v_width;
        height = St_semi_half_font_blk_5.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_semi_half_font_blk_5[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_6)
    {
        row = St_semi_half_font_blk_6.u08v_start_y;
        column = St_semi_half_font_blk_6.u08v_start_x;
        width = St_semi_half_font_blk_6.u08v_width;
        height = St_semi_half_font_blk_6.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_semi_half_font_blk_6[j * width + i];
            }
        }
    }

    if (flg_blk_valid & e_FONT_LARGE_HALF_BLK_7)
    {
        row = St_semi_half_font_blk_7.u08v_start_y;
        column = St_semi_half_font_blk_7.u08v_start_x;
        width = St_semi_half_font_blk_7.u08v_width;
        height = St_semi_half_font_blk_7.u08v_height;
        disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
        for (U08 j = 0; j < height; j++)
        {
            disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
            for (U08 i = 0; i < width; i++)
            {
                *(disp_buffer++) |= U08a_semi_half_font_blk_7[j * width + i];
            }
        }
    }
}
/* -----------------------------------------------------------------------------
 Function   : Paint_DrawSemiHalfHPA
 Memo       : セミハーフフォント描画(hPa)
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_DrawSemiHalfHPA(U08 offset_x, U08 offset_y)
{
    U08 *disp_buffer_org = Ssd1306_Get_Draw_Canvas() + offset_x + offset_y * SSD1306_COLUMNS;
    U08 *disp_buffer_temp;
    U08 *disp_buffer;
    U08 row, column;
    U08 width, height;
    row = St_semi_half_font_hPa.u08v_start_y;
    column = St_semi_half_font_hPa.u08v_start_x;
    width = St_semi_half_font_hPa.u08v_width;
    height = St_semi_half_font_hPa.u08v_height;
    disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
    for (U08 j = 0; j < height; j++)
    {
        disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
        for (U08 i = 0; i < width; i++)
        {
            *(disp_buffer++) |= U08a_semi_half_font_hPa[j * width + i];
        }
    }
}
/* -----------------------------------------------------------------------------
 Function   : Paint_DrawSemiHalfPercent
 Memo       : セミハーフフォント描画(%)
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_DrawSemiHalfPercent(U08 offset_x, U08 offset_y)
{
    U08 *disp_buffer_org = Ssd1306_Get_Draw_Canvas() + offset_x + offset_y * SSD1306_COLUMNS;
    U08 *disp_buffer_temp;
    U08 *disp_buffer;
    U08 row, column;
    U08 width, height;
    row = St_semi_half_font_Percent.u08v_start_y;
    column = St_semi_half_font_Percent.u08v_start_x;
    width = St_semi_half_font_Percent.u08v_width;
    height = St_semi_half_font_Percent.u08v_height;
    disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
    for (U08 j = 0; j < height; j++)
    {
        disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
        for (U08 i = 0; i < width; i++)
        {
            *(disp_buffer++) |= U08a_semi_half_font_Percent[j * width + i];
        }
    }
}
/* -----------------------------------------------------------------------------
 Function   : Paint_DrawSemiHalfDot
 Memo       : セミハーフフォント描画(ドット)
 Date       : 2021.08.29
------------------------------------------------------------------------------*/
static void Paint_DrawSemiHalfDot(U08 offset_x, U08 offset_y)
{
    U08 *disp_buffer_org = Ssd1306_Get_Draw_Canvas() + offset_x + offset_y * SSD1306_COLUMNS;
    U08 *disp_buffer_temp;
    U08 *disp_buffer;
    U08 row, column;
    U08 width, height;
    row = St_semi_half_font_dot.u08v_start_y;
    column = St_semi_half_font_dot.u08v_start_x;
    width = St_semi_half_font_dot.u08v_width;
    height = St_semi_half_font_dot.u08v_height;
    disp_buffer_temp = disp_buffer_org + row * SSD1306_COLUMNS + column;
    for (U08 j = 0; j < height; j++)
    {
        disp_buffer = disp_buffer_temp + j * SSD1306_COLUMNS;
        for (U08 i = 0; i < width; i++)
        {
            *(disp_buffer++) |= U08a_semi_half_font_dot[j * width + i];
        }
    }
}