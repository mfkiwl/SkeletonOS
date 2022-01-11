set(SKELETON_VERSION v1.0)

include_directories(include)

file(GLOB ARCH arch-${PLATFORM}/*.c)

set(MAIN main-${PLATFORM}.c)

file(GLOB SOURCES
    src/*.c
)

if (PLATFORM STREQUAL linux)

    set(SKELETON_SCHEDULER_MAX_PROCESSES 64)

    add_compile_options(-Wall -Wno-unused-variable -Wno-unused-function -O0)
    
    # libSkeleton is always created 
    add_library(Skeleton STATIC ${SOURCES} ${ARCH})

    add_executable(skeleton ${MAIN})

    target_link_libraries(skeleton m Skeleton)

elseif(PLATFORM STREQUAL arm) # RP2040
    
    include_directories(arch-arm)

    set(SKELETON_SCHEDULER_MAX_PROCESSES 16)

    add_executable(skeleton ${MAIN} ${SOURCES} ${ARCH})

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

endif()

configure_file(config/cmake.h.in ${CMAKE_BINARY_DIR}/generated/cmake.h)

include_directories( ${CMAKE_BINARY_DIR}/generated/ ) # Make sure it can be included...
