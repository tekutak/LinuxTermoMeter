#ifndef _FONT_H_
#define _FONT_H_
/* -----------------------------------------------------------------------------
 Include
------------------------------------------------------------------------------*/
#include "common.h"

/* -----------------------------------------------------------------------------
 Typedef
------------------------------------------------------------------------------*/
typedef struct  
{
	U08 u08v_width;
	U08 u08v_height;
	U08 u08v_start_x;
	U08 u08v_start_y;
}ST_FONT_ELEMENT;

typedef enum
{
	e_FONT_LARGE_BLK_1 = 0b00000001,
	e_FONT_LARGE_BLK_2 = 0b00000010,
	e_FONT_LARGE_BLK_3 = 0b00000100,
	e_FONT_LARGE_BLK_4 = 0b00001000,
	e_FONT_LARGE_BLK_5 = 0b00010000,
	e_FONT_LARGE_BLK_6 = 0b00100000,
	e_FONT_LARGE_BLK_7 = 0b01000000,

	e_END_OF_FONT_LARGE_BLK
}ET_FONT_LARGE_BLK;
#define FONT_LARGE_BLK_NUM (e_END_OF_FONT_LARGE_BLK)

typedef enum
{
	e_FONT_LARGE_HALF_BLK_1 = 0b00000001,
	e_FONT_LARGE_HALF_BLK_2 = 0b00000010,
	e_FONT_LARGE_HALF_BLK_3 = 0b00000100,
	e_FONT_LARGE_HALF_BLK_4 = 0b00001000,
	e_FONT_LARGE_HALF_BLK_5 = 0b00010000,
	e_FONT_LARGE_HALF_BLK_6 = 0b00100000,
	e_FONT_LARGE_HALF_BLK_7 = 0b01000000,

	e_END_OF_FONT_LARGE_HALF_BLK
}ET_FONT_LARGE_HALF_BLK;
#define FONT_LARGE_HALF_BLK_NUM (e_END_OF_FONT_LARGE_HALF_BLK)

/* -----------------------------------------------------------------------------
 Extern
------------------------------------------------------------------------------*/
extern const ST_FONT_ELEMENT St_large_font_blk_1;
extern const ST_FONT_ELEMENT St_large_font_blk_2;
extern const ST_FONT_ELEMENT St_large_font_blk_3;
extern const ST_FONT_ELEMENT St_large_font_blk_4;
extern const ST_FONT_ELEMENT St_large_font_blk_5;
extern const ST_FONT_ELEMENT St_large_font_blk_6;
extern const ST_FONT_ELEMENT St_large_font_blk_7;
extern const ST_FONT_ELEMENT St_large_font_dot;
extern const ST_FONT_ELEMENT St_large_half_font_blk_1;
extern const ST_FONT_ELEMENT St_large_half_font_blk_2;
extern const ST_FONT_ELEMENT St_large_half_font_blk_3;
extern const ST_FONT_ELEMENT St_large_half_font_blk_4;
extern const ST_FONT_ELEMENT St_large_half_font_blk_5;
extern const ST_FONT_ELEMENT St_large_half_font_blk_6;
extern const ST_FONT_ELEMENT St_large_half_font_blk_7;
extern const ST_FONT_ELEMENT St_large_half_font_deg;
extern const ST_FONT_ELEMENT St_semi_half_font_blk_1;
extern const ST_FONT_ELEMENT St_semi_half_font_blk_2;
extern const ST_FONT_ELEMENT St_semi_half_font_blk_3;
extern const ST_FONT_ELEMENT St_semi_half_font_blk_4;
extern const ST_FONT_ELEMENT St_semi_half_font_blk_5;
extern const ST_FONT_ELEMENT St_semi_half_font_blk_6;
extern const ST_FONT_ELEMENT St_semi_half_font_blk_7;
extern const ST_FONT_ELEMENT St_semi_half_font_hPa;
extern const ST_FONT_ELEMENT St_semi_half_font_Percent;
extern const ST_FONT_ELEMENT St_semi_half_font_dot;
extern const ST_FONT_ELEMENT St_semi_half_font_arrow;
extern const ST_FONT_ELEMENT St_font_battery;
extern const ST_FONT_ELEMENT St_font_battery_frame;
extern const ST_FONT_ELEMENT St_font_battery_bar;
extern const ST_FONT_ELEMENT St_font_usb;

extern const U08 U08a_font_map[];

extern const U08 U08a_large_font_blk_1[];
extern const U08 U08a_large_font_blk_2[];
extern const U08 U08a_large_font_blk_3[];
extern const U08 U08a_large_font_blk_4[];
extern const U08 U08a_large_font_blk_5[];
extern const U08 U08a_large_font_blk_6[];
extern const U08 U08a_large_font_blk_7[];
extern const U08 U08a_large_font_dot[];
extern const U08 U08a_large_half_font_blk_1[];
extern const U08 U08a_large_half_font_blk_2[];
extern const U08 U08a_large_half_font_blk_3[];
extern const U08 U08a_large_half_font_blk_4[];
extern const U08 U08a_large_half_font_blk_5[];
extern const U08 U08a_large_half_font_blk_6[];
extern const U08 U08a_large_half_font_blk_7[];
extern const U08 U08a_large_half_font_deg[];
extern const U08 U08a_semi_half_font_blk_1[];
extern const U08 U08a_semi_half_font_blk_2[];
extern const U08 U08a_semi_half_font_blk_3[];
extern const U08 U08a_semi_half_font_blk_4[];
extern const U08 U08a_semi_half_font_blk_5[];
extern const U08 U08a_semi_half_font_blk_6[];
extern const U08 U08a_semi_half_font_blk_7[];
extern const U08 U08a_semi_half_font_hPa[];
extern const U08 U08a_semi_half_font_Percent[];
extern const U08 U08a_semi_half_font_dot[];
extern const U08 U08a_semi_half_font_arrow[];
extern const U08 U08a_font_battery[];
extern const U08 U08a_font_battery_frame[];
extern const U08 U08a_font_battery_bar[];
extern const U08 U08a_font_usb[];

#endif /* _FONT_H_ */