/*!
    \file  main.c
    \brief USB CDC ACM device

    \version 2019-6-5, V1.0.0, demo for GD32VF103
*/

/*
    Copyright (c) 2019, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/


#include "gd32vf103.h"
#include "lcd.h"
#include "delay.h"
#include "gd32v_mpu6500_if.h"


#define GRAPH_HEIGHT    30


int main(void)
{
    /* The related data structure for the IMU, contains a vector of x, y, z floats*/
    mpu_vector_t vec, vec_temp;
    /* for lcd */
    uint16_t line_color;

    /* Initialize pins for I2C */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_I2C0);
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    /* Initialize the IMU (Notice that MPU6500 is referenced, this is due to the fact that ICM-20600
       ICM-20600 is mostly register compatible with MPU6500, if MPU6500 is used only thing that needs 
       to change is MPU6500_WHO_AM_I_ID from 0x11 to 0x70. */
    mpu6500_install(I2C0);
    
    /* Initialize LCD */
    Lcd_SetType(LCD_INVERTED);
    Lcd_Init();
    LCD_Clear(BLACK);
    
    mpu6500_getAccel(&vec_temp);
    
    while(1){
        /* Get accelleration data (Note: Blocking read) puts a force vector with 1G = 4096 into x, y, z directions respectively */
        mpu6500_getAccel(&vec);

        /* Do some fancy math to make a nice display */

        /* Green if pointing up, red if down */
        line_color = (vec.z < 0) ? RED : GREEN;
        /* Draw a unit circle (1G) */
        Draw_Circle(160/2, 80/2, 28, BLUE);
        /* Erase last line */
        LCD_DrawLine(160/2, 80/2, (160/2)+(vec_temp.y)/(4096/28), (80/2)+(vec_temp.x/(4096/28)),BLACK);
        /* Draw new line, scaled to the unit circle */
        LCD_DrawLine(160/2, 80/2, (160/2)+(vec.y)/(4096/28), (80/2)+(vec.x/(4096/28)),line_color);
        /* Store the last vector so it can be erased */
        vec_temp = vec;
        /* Wait for LCD to finish drawing */
        LCD_Wait_On_Queue();
    }
}


