#include <stdio.h>
#include <sys/time.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"
#include "ff.h"
#include "sd_card.h"
#include "logger.h"
#include "arch.h"

uint64_t getTime()
{
    return time_us_64();
}

void hardwareSchedulerRun()
{
    watchdog_update();
}

void getDataTime(char* buffer)
{
    datetime_t t;
    
    rtc_get_datetime(&t);
    datetime_to_str(buffer, 64, &t);

}

uint32_t rnd(void)
{
    int k, random=0;
    volatile uint32_t *rnd_reg=(uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
    
    for(k=0;k<32;k++){
    
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
    gpio_put(25, blink);
    blink = !blink;

	return 0;
}

void initDrivers()
{
    stdio_init_all();
	gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

	initPIOSERIAL();
    rtcInit();

    sleep_ms(100);
}

static uint8_t SDCardAvailable = 1;

int fileWrite(char* filename, char *message)
{
    FRESULT fr;
    FIL fil;
    FATFS fs;
    int ret;

    if(SDCardAvailable == 0)
    {
        writeString("[ERROR] No SD CARD write possible - Please, insert SDCARD and reboot the system ! \r\n");
        return 1;
    }


    // Initialize SD card
    if (!sd_init_driver()) {
        writeString("[ERROR] Could not initialize SD card, SPI Driver failure \r\n");
        while (true);
    }

    // Mount drive
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        writeString("[ERROR] Could not mount filesystem \r\n");
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    // Open file for writing ()
    fr = f_open(&fil, filename, FA_WRITE | FA_OPEN_APPEND);
    if (fr != FR_OK) {
        writeString("[ERROR] Could not open file \r\n");
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    // Write something to file
    ret = f_printf(&fil, message);
    if (ret < 0) {
        writeString("[ERROR] Could not write to file \r\n");
        f_close(&fil);
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK) {
        writeString("[ERROR] Could not close file \r\n");
        SDCardAvailable = 0;
        //while (true);
        return 1;
    }

}
