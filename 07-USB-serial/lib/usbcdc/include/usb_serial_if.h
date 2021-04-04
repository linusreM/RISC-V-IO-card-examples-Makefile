#pragma once

#include "cdc_acm_core.h"
#include <stdio.h>
#include <stdlib.h>

extern uint8_t packet_sent, packet_receive;
extern uint32_t receive_length;
void configure_usb_serial();
int usb_serial_available();
size_t read_usb_serial(uint8_t* data);
void set_usb_clock_96m_hxtal();
extern usb_core_driver USB_OTG_dev;

ssize_t _write(int fd, const void* ptr, size_t len);
