#include "usb_serial_if.h"
#include "gd32vf103.h"
#include "drv_usb_hw.h"
#include "systick.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system_gd32vf103.h"

#define USB_BUFFER_SIZE 1024
#define TIMEOUT_PRINTF 10000
typedef struct{
    uint8_t buffer[USB_BUFFER_SIZE];
    uint32_t max_size;
    uint32_t index;
}USB_serial_buffer_t;

USB_serial_buffer_t usb_buffer = { .buffer = "", .max_size = USB_BUFFER_SIZE, .index = 0};

uint8_t tx_buffer[512] = "";

usb_core_driver USB_OTG_dev = 
{
    .dev = {
        .desc = {
            .dev_desc       = (uint8_t *)&device_descriptor,
            .config_desc    = (uint8_t *)&configuration_descriptor,
            .strings        = usbd_strings,
        }
    }
};

uint32_t usb_serial_connected(){
    if(cdc_acm_get_line_state()) return 1;
    return 0;
}

size_t read_usb_serial(uint8_t* data){
    size_t ret = 0;
    static uint8_t receive_buffer[64] = {'\0'};
    usb_buffer.index = 0;
    if (USBD_CONFIGURED == USB_OTG_dev.dev.cur_status){
        while(packet_receive == 1){
            if(receive_length > 0){
                memcpy(&usb_buffer.buffer[usb_buffer.index], receive_buffer, receive_length);
                usb_buffer.index += receive_length;
                ret += receive_length;
            }
            packet_receive = 0;
            receive_length = 0;
            usbd_ep_recev (&USB_OTG_dev, CDC_ACM_DATA_OUT_EP, receive_buffer, 64);
            usb_mdelay(1);
        }
        usb_buffer.buffer[usb_buffer.index++] = '\0';
        memcpy(data, usb_buffer.buffer, usb_buffer.index);
    }
    return usb_buffer.index -1;  
}


//TODO move to USB library
void configure_usb_serial(){
    set_usb_clock_96m_hxtal();
    eclic_global_interrupt_enable();
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL2_PRIO2);
    usb_rcu_config();
    usb_timer_init();
    usb_intr_config();
    usbd_init (&USB_OTG_dev, USB_CORE_ENUM_FS, &usbd_cdc_cb);
}

int usb_serial_available(){return USBD_CONFIGURED == USB_OTG_dev.dev.cur_status;}


//TODO Risk of overflow when printing large amounts of text, should probably do some length checking

ssize_t _write(int fd, const void* ptr, size_t len){
    int64_t temp = get_timer_value();
    while(packet_sent == 0 && temp + TIMEOUT_PRINTF > get_timer_value());
    packet_sent = 0;
    strncpy((char*)tx_buffer, ptr, len);
    usbd_ep_send(&USB_OTG_dev, CDC_ACM_DATA_IN_EP, tx_buffer, len);
    return len;
}

void set_usb_clock_96m_hxtal(void){

    /* reset the RCC clock configuration to the default reset state */
    /* enable IRC8M */
    RCU_CTL |= RCU_CTL_IRC8MEN;
    
    /* reset SCS, AHBPSC, APB1PSC, APB2PSC, ADCPSC, CKOUT0SEL bits */
    RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC |
                  RCU_CFG0_ADCPSC | RCU_CFG0_ADCPSC_2 | RCU_CFG0_CKOUT0SEL);

    /* reset HXTALEN, CKMEN, PLLEN bits */
    RCU_CTL &= ~(RCU_CTL_HXTALEN | RCU_CTL_CKMEN | RCU_CTL_PLLEN);

    /* Reset HXTALBPS bit */
    RCU_CTL &= ~(RCU_CTL_HXTALBPS);

    /* reset PLLSEL, PREDV0_LSB, PLLMF, USBFSPSC bits */
    
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PREDV0_LSB | RCU_CFG0_PLLMF |
                  RCU_CFG0_USBFSPSC | RCU_CFG0_PLLMF_4);
    RCU_CFG1 = 0x00000000U;

    /* Reset HXTALEN, CKMEN, PLLEN, PLL1EN and PLL2EN bits */
    RCU_CTL &= ~(RCU_CTL_PLLEN | RCU_CTL_PLL1EN | RCU_CTL_PLL2EN | RCU_CTL_CKMEN | RCU_CTL_HXTALEN);
    /* disable all interrupts */
    RCU_INT = 0x00FF0000U;


    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;

    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(1){
        }
    }

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/1 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB/2 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV2;

    if(HXTAL_VALUE==25000000){

        /* CK_PLL = (CK_PREDIV0) * 24 = 96 MHz */
        RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
        RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL24);

        /* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
        RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF | RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
        RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 | RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);
        /* enable PLL1 */
        RCU_CTL |= RCU_CTL_PLL1EN;
        /* wait till PLL1 is ready */
        while((RCU_CTL & RCU_CTL_PLL1STB) == 0){
        }

    }else if(HXTAL_VALUE==8000000){
        /* CK_PLL = (CK_PREDIV0) * 24 = 96 MHz */
        RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
        RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL24);

		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 | RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2 );
    }

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLL;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLL)){
    }
    //SystemCoreClock = 96000000;
    SystemCoreClockUpdate();
}