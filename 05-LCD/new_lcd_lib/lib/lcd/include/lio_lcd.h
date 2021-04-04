/**
 * @file lio_lcd.h
 * @author Linus Remahl
 * @date 04 feb 2021
 * @brief This is an lcd-library for GD32VF103. 
 *
 * The target LCD uses an ST7735s controller, and should
 * be compatible with most lcds using the same controller. 
 * Something to be aware of is that the coordinate system for these LCD's
 * originates in the top left corner, with x going positive to the right, and
 * y going positive -downwards-. So for example in a 160x80 pixel display the
 * Coordinate x = 150, y = 70 will be located in the bottom right corner.
 */

#pragma once
#include "gd32vf103.h"
#include "stdio.h"


/**
 * Use this struct to configure the lcd on initialization. 
* **/
typedef struct {
	uint8_t invert;		/** Some displays need to be inverted to display the right colors. 1 for invert, 0 for no inversion. **/
	uint16_t width; 	/** The resolution of your display in the x-axis. **/
	uint16_t height;	/** The resolution of your display in the y-axis. **/
	uint16_t offset_x;	/** Different manufacturers use different offsets here (blanking area)**/
	uint16_t offset_y;	/** Different manufacturers use different offsets here (blanking area)**/
	uint16_t background;/** The default background color to use. Change with lio_lcd_change_background. **/
}lio_lcd_config_t;




/**
 * LCD initialization. Before initializing populate the lio_lcd_config_t struct
 * with proper values for your application. 
 * 
 * @param config configuration struct that sets up the LCD
 *  **/
void lio_lcd_init(lio_lcd_config_t config);

/**
 * Set the full lcd screen to the color. 
 * 
 * @param color make the lcd this color
 *  **/
void lio_lcd_clear(uint16_t color);

/**
 * Set the LCD background color. This is used by the character renderer and the bitmask renderer. 
 * 
 * @param color set the background to this color.
 *  **/
void lio_lcd_set_background(uint16_t color);

/**
 * Draw a filled rectangle to the screen. 
 * 
 * @param x1 top left corner x-coordinate.
 * @param x2 bottom right corner x-coordinate
 * @param y1 top left y-coordinate
 * @param y2 bottom right y-coordinate.
 * @param color color of the rectangle.
 *  **/
void lio_lcd_rect_fill(int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint16_t color);

/**
 * Draw outlines of a rectangle to the screen. 
 * 
 * @param x1 top left corner x-coordinate.
 * @param x2 bottom right corner x-coordinate
 * @param y1 top left y-coordinate
 * @param y2 bottom right y-coordinate.
 * @param color color of the rectangle.
 *  **/
void lio_lcd_rect_lines(int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint16_t color);

/**
 * Draw one pixel to a coordinate 
 * 
 * @param x x-coordinate
 * @param y y-coordinate
 *  **/
void lio_lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * Draws a binary encoded image 1 = color 0 = background. Reads from lsb->msb bytewise.
 * x1 y1, x2 y2 sets the boundaries of the image. 
 * 
 * @param x1 top left corner x-coordinate.
 * @param x2 bottom right corner x-coordinate
 * @param y1 top left y-coordinate
 * @param y2 bottom right y-coordinate.
 * @param p_bitmask where the binary encoded image is stored. The data is not modified.
 * @param color image color.
 *  **/
void lio_lcd_draw_bitmask(int16_t x1, int16_t x2, int16_t y1, int16_t y2, const uint8_t *p_bitmask, uint16_t color);

/**
 * Draws an ascii character on the screen whitespace will be filled with background.
 * 
 * @param x x-coordinate
 * @param y y-coordinate
 * @param c The character to draw. ASCII encoded.
 * @param color color of the character.
* **/
void lio_lcd_draw_char(int16_t x, int16_t y, char c, uint16_t color);

/**
 * Draws an ASCII encoded string to the screen. Text will wrap to start of the screen if
 * it goes outside the width of the screen. (in the x axis, y does not wrap)
 * 
 * @param x x-coordinate
 * @param y y-coordinate
 * @param str the string to print.
 * @param color color of the characters.
* **/
void lio_lcd_draw_string(int16_t x, int16_t y, const char* str, uint16_t color);

/**
 * Draws a number to the screen. The origin will be the lowest digit of the number.
 * 
 * @param x x-coordinate
 * @param y y-coordinate
 * @param num the number to draw
 * @param base number base, 2 for binary, 10 for decimal, 16 for hexadecimal, et.c.
 * @param color color of the characters.
* **/
void lio_lcd_draw_num(int16_t x, int16_t y, int32_t num, int base, uint16_t color);

/**
 * Printf but for the lcd. Use the same as you wold normal printf, but with the additional arguments.
 * floats such as %.2f does not work, and will just be blanked out.
 * 
 * @param x x-coordinate
 * @param y y-coordinate
 * @param color color of the characters.
* **/
void lio_lcd_printf(int16_t x, int16_t y, uint16_t color, char *format, ...);

/**
 * Draws a line from point 1 to point 2. Based on Bresenham's line algorithm.
 *
 * @param x1 origin x coordinate
 * @param x2 end point x coordinate
 * @param y1 origin y coordinate
 * @param y2 end point y coordinate
 * @param color color of the line.
* **/
void lio_lcd_draw_line(int32_t x1, int32_t x2, int32_t y1, int32_t y2, uint16_t color);

/**
 * Sets an active area to draw to. Intended to be used in conjunction with lcd_write_color, 
 * lcd_write_colors or lio_lcd_draw_next_pixel. When used the lcd start writing pixel data
 * to the top left corner, defined by x1 y1 going right. When the pixel coordinate goes above x2
 * it will wrap around to x1 and y+1. When y2 is reached it will wrap back around to x1 y1.
 *
 * Any function in this library excluding the previously mentioned ones will change the active area.
 *
 * @param x1 Left boundary
 * @param x2 Right boundary
 * @param y1 Top boundary
 * @param y2 Bottom boundary
* **/
void lio_lcd_set_area(int16_t x1, int16_t x2, int16_t y1, int16_t y2);

/**
 * Puts a pixel in the active area. Coordinate will advance by one.
 * 
 * @param color color of the pixel.
* **/
void lio_lcd_draw_next_pixel(uint16_t color);
/**
 * Puts a number of pixels in the active area. Pixel data is fed from a buffer.
 * 
 * @param len number of fixels to draw
 * @param p_color image data buffer
* **/
void lcd_write_colors(uint32_t len, uint16_t* p_color);

/**
 * Write a number of pixels in the active area. Pixels will be of a single color.
 *
 * @param len number of pixels to draw
 * @param color color of the pixels
* **/
void lcd_write_color(uint32_t len, uint16_t color);

void lio_lcd_draw_char_scaled(int16_t x, int16_t y, char c, uint16_t scale, uint16_t color);

void lio_lcd_draw_string_scaled(int16_t x, int16_t y, uint16_t scale, const char* str, uint16_t color);

void lio_lcd_printf_scaled(int16_t x, int16_t y, uint16_t color, uint16_t scale, char *format, ...);
