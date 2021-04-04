

#include "gd32vf103.h"
#include "delay.h"

/* This contains most of the LCD functions */
#include "lcd.h"

/* This contains an image */
#include "kth_logo.h"

int main(void)
{

    int main(){
	lio_lcd_config_t lcd_conf = {0x00, 160, 80, 0, 24, RED | GREEN};
	uint8_t bitmask[] = {0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x42,0x42,0x3F,0x00,0x00};
	uint16_t color = 0;
	uint32_t port = 0;
	uint32_t count = 0;
	uint16_t hours = 13;
	uint16_t minutes = 37;
	uint16_t seconds = 0;
	char string[] = "String! Some more string!!!!!";
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
	lio_lcd_init(lcd_conf);
	char num_string[10] = {0};

	int32_t analog_read = 0;
    

    graph_config_t yellow_graph = {
        .color = RED | GREEN,                //Graph color
        .background = 0,            //Background color (used for erasing)
        .x_origin = 30,                 //x,y coordinates for upper left corner
        .y_origin = 20,
        .scale_setting = SCALE_TO_FIT,  //Scaling behaviour, SCALE_TO_FIT (fits the data to the window),  
                                        //                   NO_SCALING (just 1:1 maps the data),
                                        //                   USE_CUSTOM (uses .scaling as the divisor)
        .scaling = 1,
        .lower_bound = 0                //The lowest pixel on the graph maps to this number (not used by SCALE_TO_FIT)
    };
    gui_init_graph(&yellow_graph, 100, GRAPH_HEIGHT);



	
	lio_lcd_clear(BLUE);
	lio_lcd_rect_fill(20, 50, 20, 50, RED);
	lio_lcd_rect_lines(20, 50, 20, 50, GREEN);
	lio_lcd_draw_bitmask(30, 37, 30, 43, bitmask, BLUE);
	lio_lcd_draw_char(20, 50, 't', RED);
	lio_lcd_draw_char(28, 50, 'e', RED);
	lio_lcd_draw_char(36, 50, 's', RED);
	lio_lcd_draw_char(44, 50, 't', RED);
	lio_lcd_draw_char(52, 50, '!', RED);
	lio_lcd_draw_string(10, 10, string, 0x0000);

	lio_lcd_draw_num(90, 60, 0xA5, 2, 0);

	lio_lcd_draw_line(80, 20, 40, 25, 0xFFFF);
	lio_lcd_draw_line(80, 50, 40, 75, 0xFFFF);
	lio_lcd_draw_line(80,125, 40, 10, 0xFFFF);
	lio_lcd_draw_line(80,145, 40, 60, 0xFFFF);
	lio_lcd_set_background(0);

	while(1){
		
		count += 1;
		port = gpio_input_port_get(GPIOB);
		gpio_port_write(GPIOB, (port & BITMASK) | (count & (~BITMASK)));
		lio_lcd_draw_num(60, 60, -count, 10, 0xFFFF);
		//lio_lcd_printf(20, 20, 0, "Hex: 0x%08x", count);
		lio_lcd_printf_scaled(20, 20, 0xFFFF, 2, "%02d:%02d:%02d", hours, minutes, seconds);
		

		analog_read = count*0xF0FF00FF;
        gui_erase_graph(&yellow_graph);
        gui_append_graph(&yellow_graph, &analog_read, 1);
        gui_render_graph(&yellow_graph);
		for(volatile int i = 300000; i > 0; i--);

		seconds++;
		if(seconds > 59){
			minutes += 1;
			seconds = 0;
			if(minutes > 59){
				minutes = 0;
				hours = (hours+1) % 24;
			}
		}	
		
	}
}

    /* Initialize the LCD for Longan nano */
    //lio_lcd_config_t lcd_conf = {0x00, 160, 80, 0, 24, RED | GREEN};

    /* The lcd on the longan nano is a 160*80 px ips display. It has 16bit colors.
       The colors use the format RGB565 which encodes 5 bits for red, 6 bits for green and 5 for blue.
       
       The bits are arranged as such:
       | b15 | b14 | b13 | b12 | b11 | b10 | b9  | b8  | b7  | b6  | b5  | b4  | b3  | b2  | b1  | b0  |
       |-----------------------------|-----------------------------------|-----------------------------|
       | R 4 | R 3 | R 2 | R 1 | R 0|||G 5 | G 4 | G 3 | G 2 | G 1 | G 0|||B 4 | B 3 | B 2 | B 1 | B 0 |

       This gives RED = 0xF800, GREEN = 0x07E0, BLUE = 0x001F
       
       The lcd.h libary contains defines for most basic colors
       */

    /* This lcd api uses a global variable for the background color 
       colors are represented with a 16bit integer. */
    //BACK_COLOR = BLACK;

    /* Use LCD_clear() to fill the entire screen with a color */
    //LCD_Clear(BLACK);
    
    /* Use this to display a string */
    //LCD_ShowStr(0, 0, (uint8_t*)"Hello world!", WHITE, TRANSPARENT);

    /* Draw a line from point A to point B. First two arguments are x and y for point A, and argument 3 and four point B */
    //LCD_DrawLine(0, 16, 159, 16, WHITE);

    /* Draw a rectangle */
    //LCD_DrawRectangle(2, 20, 31, 49, GREEN);
    
    /* Draw circle, first two arguments are origo and third the radius */
    //Draw_Circle(41, 59, 20, RED);
    /* Make the circle a little clock */
    //LCD_DrawLine(41, 59, 41, 43, CYAN);
    //LCD_DrawLine(41, 59, 51, 66, CYAN);

    /* Draws a 3*3 point at the coordinate */
    //LCD_DrawPoint_big(68, 30, MAGENTA);
    //LCD_DrawPoint_big(68, 40, MAGENTA);
    //LCD_DrawPoint_big(68, 50, MAGENTA);

    /* Draws a single pixel on the coordinate */
    //LCD_DrawPoint(78, 30, LIGHTBLUE);
    //LCD_DrawPoint(78, 40, BLUE + RED);   //BLUE + RED = MAGENTA
    //LCD_DrawPoint(78, 50, BLUE + GREEN); //BLUE + GREEN = CYAN

    /* This function takes a pointer to an array containing colors to draw a picture. */
    //LCD_ShowPicture(90, 20, 139, 69, (uint8_t*)&kth_logo_map);

    /* Display an integer */
    //LCD_ShowNum(35,22,12, 2,RED);

    /* This function contains an example on how to use the graph UI element. */
    //while(1){
    //    LCD_WR_Queue();
    //};
//}
