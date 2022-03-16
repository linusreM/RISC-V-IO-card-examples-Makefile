#include "gd32vf103.h"
#include "delay.h"

/* This contains most of the LCD functions */
/* Intended as a slight upgrade from the older LCD library. */
/* In this form it is blocking, but could be rather easily modified to be non-blocking aswell */
#include "lio_lcd.h"

/* This contains higher level graphical functions
   currently just a graph utility but more will be added. */
#include "lcd_light_gui.h"

/* This contains an image */
#include "kth_logo.h"

#define BITMASK 0xFFFFFFF8
#define RED 	0xF800
#define GREEN	0x07E0
#define BLUE	0x001F

int main(){
	uint8_t bitmask[] = {0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x42,0x42,0x3F,0x00,0x00};

	uint16_t minutes = 0;
	uint16_t seconds = 0;
	uint16_t tenmilli = 0;

    lio_lcd_config_t lcd_conf = {0x00, 160, 80, 0, 24, RED | GREEN};

	lio_lcd_init(lcd_conf);

	lio_lcd_clear(0);
	
	lio_lcd_draw_bitmask(0, 7, 0, 15, bitmask, BLUE);
	
    //Print numbers with arbitrary base
	lio_lcd_draw_num(90, 60, 0xA5, 2, 0);

	lio_lcd_set_background(0);

	while(1){

		delay_until_1ms(10);
		
        //This library implements full printf (well, no floats at this point though)
		lio_lcd_printf_scaled(20, 20, 0xFFFF, 2, "%02d:%02d:%02d", minutes, seconds, tenmilli);

		tenmilli++;
		if(tenmilli > 99){
			seconds += 1;
			tenmilli = 0;
			if(seconds > 59){
				seconds = 0;
				minutes = (minutes+1) % 59;
			}
		}	
		while(!delay_finished());
	}
    
}
