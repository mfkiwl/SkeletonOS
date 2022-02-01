#include <stdio.h>
#include <string.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "boards.h"
#include "arch.h"

#include "esp32.h"

#define ESP_CRNL "\r\n"

#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define BAUD_RATE 115200
#define BAUD_RATE_HIGHSPEED_S "3686400" 
#define BAUD_RATE_HIGHSPEED 3686400

#define BUFFER_UART_SIZE 512

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

uint8_t espSetup()
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
            //LED_GREEN_FADE(); // led event !

            uart_puts(uart1, "AT"ESP_CRNL);         
            request = AT_BAUDSET;
            ret = AT_START;
            
            break;
        }
        
        default:
        {
            ret = -99;
            break;
        }

    }

    return ret;
}