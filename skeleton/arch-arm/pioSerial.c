#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "uart_tx.pio.h"

#define PIO_TX_GPIO 14
#define PIO_BAUD 115200

PIO pio = pio0;
uint sm = 0;

void initPIOSERIAL()
{
    // Set up the state machine we're going to use to receive them.
    uint offset = pio_add_program(pio, &uart_tx_program);
    uart_tx_program_init(pio, sm, offset, PIO_TX_GPIO, PIO_BAUD);
}

void serialWriteString(const char* string)
{
    uart_tx_program_puts(pio, sm, string);
}

void serialWriteChar(const char ch)
{
    uart_tx_program_putc(pio, sm, ch);
}