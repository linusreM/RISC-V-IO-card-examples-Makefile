
#include "gd32vf103.h"
#include "delay.h"

int main(void)
{
    /* GD32VF103 Utilizes clock gating for peripherals so that they don't consume power while not being used
       therefore for each peripheral you intend to use you need to enable the clock for that peripheral.
       This example uses GPIO from PORT B so its corresponding RCU needs to be activated. */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOA);

    /* The following two lines initializes the pins connected to the LEDs on the longan board as outputs
       with push-pull. */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);           
    /* You can init multiple pins on the same port by using bitwise or */

    /* This configures the A3 and A4 pins as inputs with internal pull ups enabled */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8);

    /* gpio_bit_write() sets the state of a pin to either 3.3V or 0V(GND). 
       Use SET or 1 for 3.3V, and RESET or 0 for 0V(GND) */
    
    
    while(1){                                                                               // Loop forever
        
        if(gpio_input_bit_get(GPIOA, GPIO_PIN_8)){                                          // Read pin A8
            gpio_bit_write(GPIOB, GPIO_PIN_0, !gpio_output_bit_get(GPIOB, GPIO_PIN_0));      // If A8 is HIGH(SET), toggle B0
        }
        if(gpio_input_bit_get(GPIOA, GPIO_PIN_7)){                                          // Read pin A7                        
            gpio_bit_write(GPIOB, GPIO_PIN_1, (!gpio_output_bit_get(GPIOB, GPIO_PIN_1)));  // If A7 is HIGH(SET), toggle B1
        }
        if(gpio_input_bit_get(GPIOA, GPIO_PIN_6)){                                          // Read pin A6                        
            gpio_bit_write(GPIOB, GPIO_PIN_2, (!gpio_output_bit_get(GPIOB, GPIO_PIN_2)));  // If A6 is HIGH(SET), toggle B2
        }
        delay_1ms(500);                                                                     // Wait for half a second
    }
    
}
