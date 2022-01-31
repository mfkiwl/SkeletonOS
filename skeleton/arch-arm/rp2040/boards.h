#ifndef BOARDS_H_
#define BOARDS_H_

#include "hardware/spi.h"

// SPI (SDCARD)
#define SPI_SDCARD_ENUM           0
#define SPI_SDCARD_BUS            spi0_hw
#define SPI_SDCARD_BAUD           12500 * 1000
#define GPIO_SPI_SDCARD_SCK       18
#define GPIO_SPI_SDCARD_MISO      16
#define GPIO_SPI_SDCARD_MOSI      19

#define GPIO_SDCARD_CS      17
#define GPIO_SDCARD_CD      22


// UART 0 (ESP32)
#define UART_ESP32_BUS           uart1
#define GPIO_UART_ESP32_CTS      6 
#define GPIO_UART_ESP32_RTS      7
#define GPIO_UART_ESP32_TX       4
#define GPIO_UART_ESP32_RX       5

#define GPIO_ESP32_EN       54

// LED 25 (KeepAlive)
#define GPIO_LED_SYSTEM      25 


#endif // !BOARDS_H_