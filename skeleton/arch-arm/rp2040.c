#include <stdio.h>
#include <sys/time.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

#include "pioSerial.h"

#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "ff.h"
#include "sd_card.h"
#include "logger.h"
#include "scheduler.h"
#include "arch.h"
#include "boards.h"
#include "esp32.h"

void dspRun()
{
    RUN(1);
}

uint64_t getMicrosecTime()
{
    uint64_t tmp;
    tmp = time_us_64();

    return tmp;
}

uint64_t getMillisTime()
{
    uint64_t tmp;
    tmp = time_us_64() / 1000;

    return tmp;
}

void hardwareSchedulerRun()
{
    watchdog_update();
}

void getFormattedTime(char* formattedTime)
{
    datetime_t t;
    
    rtc_get_datetime(&t);
    datetime_to_str(formattedTime, 64, &t);
}

uint32_t rnd(void)
{
    int k, random=0;
    volatile uint32_t *rnd_reg=(uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
    
    for(k=0; k<32; k++){
    
    random = random << 1;
    random = random + (0x00000001 & (*rnd_reg));

    }
    return random;
}

// private
void rtcInit()
{
    // Start on Friday 5th of June 2020 15:45:00
    datetime_t t = {
            .year  = 2021,
            .month = 01,
            .day   = 01,
            .dotw  = 0, // 0 is Sunday, so 5 is Friday
            .hour  = 00,
            .min   = 00,
            .sec   = 00
    };

    // Start the RTC
    rtc_init();
    rtc_set_datetime(&t);
}

static uint8_t blink;

int SquareGenerator(int pid)
{
    gpio_put(GPIO_LED_SYSTEM, blink);
    blink = !blink;

	return 0;
}

void initDrivers()
{
    stdio_init_all();
	gpio_init(GPIO_LED_SYSTEM);
    gpio_set_dir(GPIO_LED_SYSTEM, GPIO_OUT);

	initPIOSERIAL();
    rtcInit();
    initESP();

    multicore_launch_core1(dspRun);

    sleep_ms(100); // safety configure
}

static uint8_t SDCardAvailable = 1;

int textFileWrite(char* filename, char* content, uint8_t mode)
{
    FRESULT fr;
    FIL fil;
    FATFS fs;
    int ret;

    if(SDCardAvailable == 0)
    {
        serialWriteString("[ERROR] No SD CARD write possible - Please, insert SDCARD and reboot the system ! \r\n");
        return 1;
    }

    // Initialize SD card
    if (!sd_init_driver()) {
        serialWriteString("[ERROR] Could not initialize SD card, SPI Driver failure \r\n");
        while (true);
    }

    // Mount drive
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not mount filesystem \r\n");
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    // Open file for writing ()
    // fr = f_open(&fil, filename, FA_WRITE | FA_OPEN_APPEND);
    fr = f_open(&fil, filename, FA_WRITE | ((mode == APPEND) ? FA_OPEN_APPEND : FA_OPEN_ALWAYS));
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not open file \r\n");
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    // Write something to file
    ret = f_printf(&fil, content);
    if (ret < 0) {
        serialWriteString("[ERROR] Could not write to file \r\n");
        f_close(&fil);
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not close file \r\n");
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    return 0;
}

int textFileRead(char* filename, char* content)
{
    FRESULT fr;
    FIL fil;
    FATFS fs;
    int ret;

    if(SDCardAvailable == 0)
    {
        serialWriteString("[ERROR] No SD CARD read possible - Please, insert SDCARD and reboot the system ! \r\n");
        return 1;
    }

    // Initialize SD card
    if (!sd_init_driver()) {
        serialWriteString("[ERROR] Could not initialize SD card, SPI Driver failure \r\n");
        while (true);
    }

    // Mount drive
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not mount filesystem \r\n");
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    // Open file for reading
    fr = f_open(&fil, filename, FA_READ);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not open file \r\n");
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    uint64_t size = f_size(&fil);

    // read the whole file
    fr = f_read(&fil, content, size, &ret);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not read file \r\n");
        f_close(&fil);
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not close file \r\n");
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    return 0;
}

int nonVolatileRead(char* name, void* content, int size)
{
    FRESULT fr;
    FIL fil;
    FATFS fs;
    int ret;

    if(SDCardAvailable == 0)
    {
        serialWriteString("[ERROR] No SD CARD read possible - Please, insert SDCARD and reboot the system ! \r\n");
        return 1;
    }

    // Initialize SD card
    if (!sd_init_driver()) {
        serialWriteString("[ERROR] Could not initialize SD card, SPI Driver failure \r\n");
        while (true);
    }

    // Mount drive
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not mount filesystem \r\n");
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    // Open file for reading
    fr = f_open(&fil, name, FA_READ);
    if (fr == FR_NO_FILE)
    {
        serialWriteString("[WARNING] File not exist \r\n");
        return -1;
    }

    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not open file \r\n");
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    fr = f_read(&fil, content, size, &ret);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not read th file \r\n");
        f_close(&fil);
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not close file \r\n");
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    return 0;
}

int nonVolatileWrite(char* name, void* content, int size)
{
    FRESULT fr;
    FIL fil;
    FATFS fs;
    int ret;

    if(SDCardAvailable == 0)
    {
        serialWriteString("[ERROR] No SD CARD write possible - Please, insert SDCARD and reboot the system ! \r\n");
        return 1;
    }

    // Initialize SD card
    if (!sd_init_driver()) {
        serialWriteString("[ERROR] Could not initialize SD card, SPI Driver failure \r\n");
        while (true);
    }

    // Mount drive
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not mount filesystem \r\n");
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    // Open file for writing ()
    fr = f_open(&fil, name, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not open file \r\n");
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    // Write something to file
    fr = f_write(&fil, content, size, &ret);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not write to file \r\n");
        f_close(&fil);
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK) {
        serialWriteString("[ERROR] Could not close file \r\n");
        SDCardAvailable = 0;
        while (true);
        return 1;
    }

    return 0;
}

void arch_exit(int code, char* msg_format, ...)
{}

