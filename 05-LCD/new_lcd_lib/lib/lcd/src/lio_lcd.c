#include "lio_lcd.h"
#include "lio_lcd_regs.h"
#include "lio_font.h"
#include "gd32vf103.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdio.h"

/* Private declarations */
void lcd_bus_init();
void spi_write_bus(uint8_t data);
void lcd_write_data(uint32_t len, uint8_t* buf);
void lcd_write_command(uint8_t command);
void lcd_CS_set();
void lcd_CS_reset();
void lcd_DC_set();
void lcd_DC_reset();
void lcd_delay_1ms(uint32_t count);
void lio_lcd_address_set(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
/*End private declarations*/

/*Private variables*/ 
lio_lcd_config_t _config;
/*End private variables*/


/*Public interface*/
void lio_lcd_init(lio_lcd_config_t config){

	_config = config;

	uint8_t buf[32] = {0};

	lcd_bus_init();
	lcd_DC_reset();
	lcd_CS_reset();
	lcd_delay_1ms(200);

	lcd_write_command(LIO_LCD_SW_RST); 	//Software reset
	lcd_delay_1ms(120);

	lcd_write_command(LIO_LCD_SLPOUT); 	//Turn off sleep mode
	lcd_delay_1ms(120);

	if(_config.invert)lcd_write_command(LIO_LCD_INVON);
	else 			  lcd_write_command(LIO_LCD_INVOFF);

	//Frame rate control
	//Frame rate=fosc/((RTNA + 20) x (LINE + FPA + BPA))

	buf[0] = LIO_LCD_FRMCTRL1_RTNA;
	buf[1] = LIO_LCD_FRMCTRL1_FPA;
	buf[2] = LIO_LCD_FRMCTRL1_BNA;

	lcd_write_command(LIO_LCD_FRMCTRL1); 	
	lcd_write_data(3, buf);

	buf[0] = LIO_LCD_FRMCTRL2_RTNA;
	buf[1] = LIO_LCD_FRMCTRL2_FPA;
	buf[2] = LIO_LCD_FRMCTRL2_BNA;

	lcd_write_command(LIO_LCD_FRMCTRL2); 	
	lcd_write_data(3, buf);

	buf[0] = LIO_LCD_FRMCTRL3_RTNAL;
	buf[1] = LIO_LCD_FRMCTRL3_FPAL;
	buf[2] = LIO_LCD_FRMCTRL3_BNAL;
	buf[3] = LIO_LCD_FRMCTRL3_RTNAF;
	buf[4] = LIO_LCD_FRMCTRL3_FPAF;
	buf[5] = LIO_LCD_FRMCTRL3_BNAF;

	lcd_write_command(LIO_LCD_FRMCTRL3); 	//FRMCTRL1 - Full color
	lcd_write_data(6, buf);

	buf[0] = LIO_LCD_INVCTR_LNINV;

	lcd_write_command(LIO_LCD_INVCTR); 	//FRMCTRL1 - Full color
	lcd_write_data(1, buf);	

	buf[0] = LIO_LCD_PWRCTR1_AVDD_VRHP;
	buf[1] = LIO_LCD_PWRCTR1_VRHN;
	buf[2] = LIO_LCD_PWRCTR1_MODE_VRH5;

	lcd_write_command(LIO_LCD_PWRCTR1); 	//FRMCTRL1 - Full color
	lcd_write_data(3, buf);	

	buf[0] = LIO_LCD_PWRCTR2_VGH_VGLSEL;

	lcd_write_command(LIO_LCD_PWRCTR2); 	//FRMCTRL1 - Full color
	lcd_write_data(1, buf);	

	buf[0] = LIO_LCD_PWRCTR3_APA;
	buf[1] = LIO_LCD_PWRCTR3_DCA;

	lcd_write_command(LIO_LCD_PWRCTR3); 
	lcd_write_data(2, buf);

	buf[0] = LIO_LCD_PWRCTR4_APB;
	buf[1] = LIO_LCD_PWRCTR4_DCB;

	lcd_write_command(LIO_LCD_PWRCTR4); 
	lcd_write_data(2, buf);

	buf[0] = LIO_LCD_PWRCTR5_APC;
	buf[1] = LIO_LCD_PWRCTR5_DCC;

	lcd_write_command(LIO_LCD_PWRCTR5); 
	lcd_write_data(2, buf);

	buf[0] = LIO_LCD_VMCTR1_VCOM;

	lcd_write_command(LIO_LCD_VMCTR1); 
	lcd_write_data(1, buf);

	buf[0]  = LIO_LCD_GMCTRP_0;
	buf[1]  = LIO_LCD_GMCTRP_1;
	buf[2]  = LIO_LCD_GMCTRP_2;
	buf[3]  = LIO_LCD_GMCTRP_3;
	buf[4]  = LIO_LCD_GMCTRP_4;
	buf[5]  = LIO_LCD_GMCTRP_5;
	buf[6]  = LIO_LCD_GMCTRP_6;
	buf[7]  = LIO_LCD_GMCTRP_7;
	buf[8]  = LIO_LCD_GMCTRP_8;
	buf[9]  = LIO_LCD_GMCTRP_9;
	buf[10] = LIO_LCD_GMCTRP_A;
	buf[11] = LIO_LCD_GMCTRP_B;
	buf[12] = LIO_LCD_GMCTRP_C;
	buf[13] = LIO_LCD_GMCTRP_D;
	buf[14] = LIO_LCD_GMCTRP_E;
	buf[15] = LIO_LCD_GMCTRP_F;

	lcd_write_command(LIO_LCD_GMCTRP); 
	lcd_write_data(16, buf);	

	buf[0]  = LIO_LCD_GMCTRN_0;
	buf[1]  = LIO_LCD_GMCTRN_1;
	buf[2]  = LIO_LCD_GMCTRN_2;
	buf[3]  = LIO_LCD_GMCTRN_3;
	buf[4]  = LIO_LCD_GMCTRN_4;
	buf[5]  = LIO_LCD_GMCTRN_5;
	buf[6]  = LIO_LCD_GMCTRN_6;
	buf[7]  = LIO_LCD_GMCTRN_7;
	buf[8]  = LIO_LCD_GMCTRN_8;
	buf[9]  = LIO_LCD_GMCTRN_9;
	buf[10] = LIO_LCD_GMCTRN_A;
	buf[11] = LIO_LCD_GMCTRN_B;
	buf[12] = LIO_LCD_GMCTRN_C;
	buf[13] = LIO_LCD_GMCTRN_D;
	buf[14] = LIO_LCD_GMCTRN_E;
	buf[15] = LIO_LCD_GMCTRN_F;

	lcd_write_command(LIO_LCD_GMCTRN); 
	lcd_write_data(16, buf);

	buf[0] = LIO_LCD_COLMOD_IFPF;

	lcd_write_command(LIO_LCD_COLMOD); 
	lcd_write_data(1, buf);

	buf[0] = LIO_LCD_MADCTL_CFG;

	lcd_write_command(LIO_LCD_MADCTL); 
	lcd_write_data(1, buf);

	lcd_write_command(LIO_LCD_DISPON);
}

void lio_lcd_clear(uint16_t color){
	uint8_t buf[] = {(0xFF00 & color) >> 8, color & 0xFF};
	lio_lcd_address_set( 0, _config.width-1, 0, _config.height-1);
	for(int i = _config.width*_config.height; i > 0; i--) lcd_write_data(2, buf);
}

void lio_lcd_rect_fill(int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint16_t color){
	lio_lcd_address_set(x1,x2,y1,y2);
	lcd_write_color((x2-x1+1)*(y2-y1+1), color);
}

void lio_lcd_rect_lines(int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint16_t color){
	//Upper
	lio_lcd_address_set(x1,x1,y1,y2);
	lcd_write_color(y2-y1, color);
	//Lower
	lio_lcd_address_set(x2,x2,y1,y2);
	lcd_write_color(y2-y1, color);
	//Left
	lio_lcd_address_set(x1,x2,y1,y1);
	lcd_write_color(x2-x1, color);
	//Right
	lio_lcd_address_set(x1,x2,y2,y2);
	lcd_write_color(x2-x1, color);
}

void lio_lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color){
	lio_lcd_address_set(x,x,y,y);
	lcd_write_color(1, color);
}

void lio_lcd_draw_next_pixel(uint16_t color){
	uint8_t buf[] = {(0xFF00 & color) >> 8, color & 0xFF};
	lcd_write_data(2, buf);
}

void lio_lcd_set_area(int16_t x1, int16_t x2, int16_t y1, int16_t y2){
	lio_lcd_address_set(x1,x2,y1,y2);
}

void lcd_write_colors(uint32_t len, uint16_t* p_color){
	uint8_t* buf = (uint8_t*) p_color;

	lcd_DC_set();
	//Claim the LCD
	lcd_CS_reset();

	for(; len > 0; len--){
		spi_write_bus(buf[1]);
		spi_write_bus(buf[0]);
		buf += 2;
	}
	while(spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS) == SET);
	lcd_CS_set();
}

void lcd_write_color(uint32_t len, uint16_t color){
	uint8_t* buf = (uint8_t*) &color;

	lcd_DC_set();
	//Claim the LCD
	lcd_CS_reset();

	for(; len > 0; len--){
		spi_write_bus(buf[1]);
		spi_write_bus(buf[0]);
	}
	while(spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS) == SET);
	lcd_CS_set();
}

void lio_lcd_draw_bitmask(int16_t x1, int16_t x2, int16_t y1, int16_t y2, const uint8_t *p_bitmask, uint16_t color){
	uint8_t mask = 0;
	lio_lcd_address_set(x1, x2, y1, y2);
	for(int i = (x2-x1+1)*(y2-y1+1); i >= 0;){
		mask = *p_bitmask;
		for(int j = 8; j > 0; j--, i--){
			if(mask & 0x01) lio_lcd_draw_next_pixel(color);
			else			lio_lcd_draw_next_pixel(_config.background); //[FIXME] - Should be background
			mask = mask >> 1;
		}
		p_bitmask++;
	}
}


void lio_lcd_draw_bitmask_scaled(int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint16_t scale, const uint8_t *p_bitmask, uint16_t color){
	uint8_t mask = 0;
	uint8_t toggle = 0;
	if(scale < 1) scale = 1;
	lio_lcd_address_set(x1, x2, y1, y2);
	for(int i = (x2-x1+1)*(y2-y1+1)/scale; i >= 0;){
		mask = *p_bitmask;
		for(int j = 8; j > 0; j--, i--){
			if(mask & 0x01) lcd_write_color(scale,color);
			else			lcd_write_color(scale,_config.background); //[FIXME] - Should be background
			mask = mask >> 1;
		}
		p_bitmask += (toggle == scale-1 ? 1 : 0);
		toggle++;
		toggle %= scale;
	}
}

void lio_lcd_draw_char(int16_t x, int16_t y, char c, uint16_t color){
	if(c < 32) return;
	c = c - 32;
	lio_lcd_draw_bitmask(x, x + 7, y, y + 15, (uint8_t*)&font_1608[c][0], color);
}

void lio_lcd_draw_char_scaled(int16_t x, int16_t y, char c, uint16_t scale, uint16_t color){
	if(c < 32) return;
	if(scale == 0) scale = 1;
	c = c - 32;
	lio_lcd_draw_bitmask_scaled(x, x + (8 * scale) - 1, y, y + (16*scale) -1, scale, (uint8_t*)&font_1608[c][0], color);
}

void lio_lcd_draw_string_scaled(int16_t x, int16_t y, uint16_t scale, const char* str, uint16_t color){
	for(; *str != '\0'; str++, x += 8 * scale){
		if(x > _config.width - 8*scale){
			x = 0;
			y += 16*scale;
		}
		lio_lcd_draw_char_scaled(x, y, *str, scale, color);
	}
}

void lio_lcd_draw_string(int16_t x, int16_t y, const char* str, uint16_t color){
	for(; *str != '\0'; str++, x += 8){
		if(x > _config.width - 8){
			x = 0;
			y += 16;
		}
		lio_lcd_draw_char(x, y, *str, color);
	}
}

void lio_lcd_draw_num(int16_t x, int16_t y, int32_t num, int base, uint16_t color){
	char string_buffer[32];
	itoa(num, string_buffer, base);
	lio_lcd_draw_string(x, y, string_buffer, color);
}

void lio_lcd_set_background(uint16_t color){
	_config.background = color;
}

void lio_lcd_draw_line(int32_t x1, int32_t x2, int32_t y1, int32_t y2, uint16_t color){
    //Based on bresenhams line algorithm
    int32_t err = 0;
    int32_t x_steps = (x2-x1) > 0 ? (x2-x1) : -(x2-x1);
    int32_t y_steps = (y2-y1) > 0 ? (y2-y1) : -(y2-y1);
    int32_t x_pos = x1;
    int32_t y_pos = y1;
    int32_t x_dir = x2 > x1 ? 1 : -1;
    int32_t y_dir = y2 > y1 ? 1 : -1;

    int32_t *pLead, *pFol, *pFDir, *pLDir, *pLPos, *pFPos;
    
    if(x_steps > y_steps){
        pLead = &x_steps;
        pLDir = &x_dir;
        pLPos = &x_pos;
        pFol = &y_steps;
        pFDir = &y_dir;
        pFPos = &y_pos;
    } else {
        pLead = &y_steps;
        pLDir = &y_dir;
        pLPos = &y_pos;
        pFol = &x_steps;
        pFDir = &x_dir;
        pFPos = &x_pos;
    }
    for(int i = *pLead + 1; i > 0; i--){
    	lio_lcd_draw_pixel(x_pos, y_pos, color);
    	err += 2*(*pFol);
    	*pLPos += *pLDir;
    	if(err > 0){
        	*pFPos += *pFDir;
        	err -= (2*(*pLead));
    	}
    }
    
}

#define LCD_PRINT_BUFFER_SIZE 256

void lio_lcd_printf(int16_t x, int16_t y, uint16_t color, char *format, ...){
	char buffer[LCD_PRINT_BUFFER_SIZE] = {'\0'};
	va_list aptr;
	va_start(aptr, format);
   	vsnprintf(buffer, LCD_PRINT_BUFFER_SIZE, format, aptr);
   	va_end(aptr);
   	lio_lcd_draw_string(x, y, buffer, color);
}

void lio_lcd_printf_scaled(int16_t x, int16_t y, uint16_t color, uint16_t scale, char *format, ...){
	char buffer[LCD_PRINT_BUFFER_SIZE] = {'\0'};
	va_list aptr;
	va_start(aptr, format);
   	vsnprintf(buffer, LCD_PRINT_BUFFER_SIZE, format, aptr);
   	va_end(aptr);
   	lio_lcd_draw_string_scaled(x, y, scale, buffer, color);
}

/*End public interface*/

/*Hardware interface*/
void lcd_bus_init(){

	spi_parameter_struct spi_config;

	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);

 	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_SPI1);
	
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 |GPIO_PIN_14| GPIO_PIN_15);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);

	spi_struct_para_init(&spi_config);

	spi_config.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_config.device_mode          = SPI_MASTER;
    spi_config.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_config.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_config.nss                  = SPI_NSS_SOFT;
    spi_config.prescale             = SPI_PSC_4;
    spi_config.endian               = SPI_ENDIAN_MSB;

	lcd_CS_set();
   
    spi_init(SPI1, &spi_config);

	spi_enable(SPI1);
}

void spi_write_bus(uint8_t data){
	//Wait for incomplete transfer in case we are still sending something!
	while(spi_i2s_flag_get(SPI1, SPI_FLAG_TBE) == RESET);
	//Send
	spi_i2s_data_transmit(SPI1, data);

}

void lio_lcd_address_set(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2){
	x1 += _config.offset_x;
	x2 += _config.offset_x;
	y1 += _config.offset_y;
	y2 += _config.offset_y;

	uint8_t buf[4];

	buf[0] = (x1 >> 8) & 0xFF;
	buf[1] =  x1 	   & 0xFF;
	buf[2] = (x2 >> 8) & 0xFF;
	buf[3] =  x2 	   & 0xFF;
	lcd_write_command(LIO_LCD_SET_COLUMN);
	lcd_write_data(4, buf);

	buf[0] = (y1 >> 8) & 0xFF;
	buf[1] =  y1 	   & 0xFF;
	buf[2] = (y2 >> 8) & 0xFF;
	buf[3] =  y2 	   & 0xFF;
	lcd_write_command(LIO_LCD_SET_ROW);
	lcd_write_data(4, buf);
	lcd_write_command(LIO_LCD_WR_MEMORY);
}

void lcd_write_data(uint32_t len, uint8_t* buf){
	//Specify send data
	lcd_DC_set();
	//Claim the LCD
	lcd_CS_reset();
	//send the data
	for(; len > 0; len--) spi_write_bus(*buf++);
	//Let go of the LCD
	while(spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS) == SET);
	lcd_CS_set();
}

void lcd_write_command(uint8_t command){
	lcd_DC_reset();
	lcd_CS_reset();
	spi_write_bus(command);
	while(spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS) == SET);
	lcd_CS_set();
}

void lcd_CS_set(){
	gpio_bit_set(GPIOC,GPIO_PIN_13);
}

void lcd_CS_reset(){
	gpio_bit_reset(GPIOC,GPIO_PIN_13);
}

void lcd_DC_set(){
	gpio_bit_set(GPIOC,GPIO_PIN_15);
}

void lcd_DC_reset(){
	gpio_bit_reset(GPIOC,GPIO_PIN_15);
}

void lcd_delay_1ms(uint32_t count){
	uint64_t start_mtime, delta_mtime;

	// Don't start measuruing until we see an mtime tick
	uint64_t tmp = get_timer_value();
	do {
	start_mtime = get_timer_value();
	} while (start_mtime == tmp);

	do {
	delta_mtime = get_timer_value() - start_mtime;
	}while(delta_mtime <(SystemCoreClock/4000.0 *count ));
}
/*End hardware interface*/