#ifndef RPY_PWM_HAT_CONSTANTS_H
#define RPY_PWM_HAT_CONSTANTS_H

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

// Registers/etc:
uint8_t MODE1              = 0x00;
uint8_t MODE2              = 0x01;
uint8_t SUBADR1            = 0x02;
uint8_t SUBADR2            = 0x03;
uint8_t SUBADR3            = 0x04;
uint8_t PRESCALE           = 0xFE;
uint8_t LED0_ON_L          = 0x06;
uint8_t LED0_ON_H          = 0x07;
uint8_t LED0_OFF_L         = 0x08;
uint8_t LED0_OFF_H         = 0x09;
uint8_t ALL_LED_ON_L       = 0xFA;
uint8_t ALL_LED_ON_H       = 0xFB;
uint8_t ALL_LED_OFF_L      = 0xFC;
uint8_t ALL_LED_OFF_H      = 0xFD;

// Bits:
uint8_t RESTART            = 0x80;
uint8_t SLEEP              = 0x10;
uint8_t ALLCALL            = 0x01;
uint8_t INVRT              = 0x10;
uint8_t OUTDRV             = 0x04;

#endif //RPY_PWM_HAT_CONSTANTS_H
