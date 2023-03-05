/*
 * oled.c
 *
 * Created: 2019-01-17 17:29:47
 *  Author: linds
 */ 

#include "global_defs.h"
#include "u8glib/u8g.h"


u8g_t u8g;

void OLED_init(void){
	u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(1,3), PN(1,2), PN(1,4));
}

void OLED_update(void){
	
	char line1[] = "I:";
	char line2[] = "x mA";
	line2[0] = vehicle_current+48; //ASCII
	
	char line3[] = "CONN";
	char line4[] = "LOST";
	
	int x_sc = u8g_GetWidth(&u8g)/2;
	int x_c = u8g_GetWidth(&u8g)/2+u8g_GetWidth(&u8g)/4;
	int y_c = u8g_GetHeight(&u8g)/2;
	
	int x = x_sc+joy_right/16;
	int y = (1024-joy_left)/16;
	
	u8g_FirstPage(&u8g);
	do {
		u8g_SetFont(&u8g, u8g_font_courB18);
		if (timeout == 0){
			u8g_DrawStr(&u8g, 0, 18, line1);
			u8g_DrawStr(&u8g, 0, 38, line2);
		}
		else{
			u8g_DrawStr(&u8g, 0, 18, line3);
			u8g_DrawStr(&u8g, 0, 38, line4);
		}
		u8g_SetColorIndex(&u8g, 1);
		u8g_DrawCircle(&u8g, x, y, 4, U8G_DRAW_ALL);
		u8g_DrawLine(&u8g, x, y, x_c, y_c);
		u8g_DrawFrame(&u8g, x_sc, 1, 128-x_sc, 64-1);
	}while (u8g_NextPage(&u8g));
	
}