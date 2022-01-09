set(SKELETON_VERSION v1.0)

include_directories(include)

set(ARCH 
        arch-arm/rp2040.c
        arch-arm/pioSerial.c
        )

set(SOURCES
        src/main.c
        src/scheduler.c
        src/cbuf.c
        src/dsp.c
        src/logger.c
)

set(SKELETON_SCHEDULER_MAX_PROCESSES 32)

add_executable(skeleton ${SOURCES} ${ARCH})

pico_generate_pio_header(skeleton ${CMAKE_CURRENT_LIST_DIR}/uart_tx.pio)

pico_enable_stdio_usb(skeleton 0)
pico_enable_stdio_uart(skeleton 0)

add_subdirectory(arch-arm/drivers/FatFs_SPI build)

# pull in common dependencies
target_link_libraries(skeleton
    pico_stdlib 
    hardware_rtc 
    hardware_uart 
    hardware_pio 
    hardware_dma 
    hardware_adc 
    hardware_irq
    hardware_i2c
    hardware_pwm
    hardware_spi
    pico_multicore
    FatFs_SPI
    )

# create map/bin/hex file etc.
pico_add_extra_outputs(skeleton)

configure_file(config/cmake.h.in ${CMAKE_BINARY_DIR}/generated/cmake.h )
include_directories( ${CMAKE_BINARY_DIR}/generated/ ) # Make sure it can be included...