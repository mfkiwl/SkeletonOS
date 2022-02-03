#include <stdio.h>
#include <string.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "boards.h"
#include "arch.h"

#include "esp32.h"

#define ESP_CRNL "\r\n"

#define STATION 1

#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define BAUD_RATE 115200
#define BAUD_RATE_HIGHSPEED_S "3686400" 
#define BAUD_RATE_HIGHSPEED 3686400

#define BUFFER_UART_SIZE 512

#define SENSOR_ID "08:3a:f2:8f:d7:ed/" // WIFI AP MAC ADDR

enum{ESP_RESET = 1, AT_START, AT_BAUDSET, AT_TEST_NEW_BAUD, AT_CONFIG, AT_DATA, AT_TEST, WAIT};
enum{AT_CONFIG_1 = 11, AT_CONFIG_2, AT_CONFIG_3, AT_CONFIG_4, AT_CONFIG_5, AT_CONFIG_6, AT_CONFIG_7};
static uint8_t request = ESP_RESET;

enum{GET = 1, SEND};
static uint8_t message = GET;

static void on_uart_rx() 
{
    uint8_t ch;

    while (uart_is_readable(uart1)) {
        ch = uart_getc(uart1);
        serialWriteChar(ch);
    }
}

void initESP()
{
    // Set up our UART with a basic baud rate.
    uart_init(uart1, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(GPIO_UART_ESP32_TX, GPIO_FUNC_UART);
    gpio_set_function(GPIO_UART_ESP32_RX, GPIO_FUNC_UART);
    gpio_set_function(GPIO_UART_ESP32_CTS, GPIO_FUNC_UART);
    gpio_set_function(GPIO_UART_ESP32_RTS, GPIO_FUNC_UART);

    uart_set_baudrate(uart1, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(uart1, true, true);

    // Set our data format
    uart_set_format(uart1, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(uart1, false);

    // Set up a RX interrupt
    int UART_IRQ = uart1 == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(uart1, true, false);

    // Reset pin init for ESP32
    gpio_init(GPIO_ESP32_EN);
    gpio_set_dir(GPIO_ESP32_EN, GPIO_OUT);
}

void espSetup(uint8_t pid)
{
    uint8_t ret = -1;

    switch (request)
    {
        case ESP_RESET:
        {
            gpio_put(GPIO_ESP32_EN, 0); // ESP off
            sleep_ms(50);
            gpio_put(GPIO_ESP32_EN, 1); // ESP on

            uart_set_baudrate(uart1, BAUD_RATE); 

            request = AT_START;
            ret = ESP_RESET;

            break;
        }
        
        case AT_START:
        {
            uart_puts(uart1, "AT"ESP_CRNL);  

            request = AT_BAUDSET;
            ret = AT_START;
            
            break;
        }

        case AT_BAUDSET:
        {
            uart_puts(uart1, "AT+UART_CUR="BAUD_RATE_HIGHSPEED_S",8,1,0,3"ESP_CRNL); // ESP32    

            request = AT_TEST_NEW_BAUD;       
            ret = AT_BAUDSET;
            
            break;
        }

        case AT_TEST_NEW_BAUD:
        {
            uart_set_baudrate(uart1, BAUD_RATE_HIGHSPEED);  
            //uart_puts(UART_ESP_ID, "AT+CWAUTOCONN=1"ESP_CRNL); // default is 1 and its fine !
            uart_puts(uart1, "AT"ESP_CRNL);  

            request = AT_CONFIG_1;       
            ret = AT_TEST_NEW_BAUD;

            break;
        }

        case AT_CONFIG_1:
        {
            uart_puts(uart1, "ATE0"ESP_CRNL); // echo off

            request = AT_CONFIG_2;    
            ret = AT_CONFIG_1;

            break;
        }

        case AT_CONFIG_2:
        {
            #if STATION
                uart_puts(uart1, "AT+CWMODE=1"ESP_CRNL); // STATION MODE
            #endif

             #if AP
                uart_puts(uart1, "AT+CWMODE=2"ESP_CRNL); // AP MODE
            #endif   

            request = AT_CONFIG_3;
            ret = AT_CONFIG_2;

            break;
        }

        case AT_CONFIG_3:
        {
            #if STATION
                uart_puts(uart1, "AT+CWJAP=\"WSensorAP-U2\",\"pippopluto69\""ESP_CRNL); // only station mode
            #endif

            request = AT_CONFIG_4;
            ret = AT_CONFIG_3;

            break;
        }

        case AT_CONFIG_4:
        {
            #if STATION
                uart_puts(uart1, "AT+CWRECONNCFG=5,10"ESP_CRNL); // try to reply connection 5 sec for 10 times
            #endif

            request = AT_CONFIG_5;
            ret = AT_CONFIG_4;

            break;
        }

        case AT_CONFIG_5:
        {
            uart_puts(uart1, "AT+MQTTUSERCFG=0,1,\""SENSOR_ID"\",\"user\",\"pass\",0,0,\"\""ESP_CRNL);  

            request = AT_CONFIG_6;
            ret = AT_CONFIG_5;

            break;
        }

        case AT_CONFIG_6:
        {
            uart_puts(uart1, "AT+MQTTCONN=0,\"10.42.0.1\",10000,1"ESP_CRNL); // application address

            request = WAIT;
            ret = AT_CONFIG_6;

            //sleep_ms(2000); // OCCHIO !

            break;
        }

        case WAIT:
        {
            request = WAIT;
            ret = WAIT;

            //sleep_ms(1); // OCCHIO !

            break;
        }
        
        default:
        {
            ret = -99;
            break;
        }

    }

}