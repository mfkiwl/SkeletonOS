set(SKELETON_VERSION v1.0)

include_directories(include)

set(MAIN main-${PLATFORM}.c)

set(SOURCES
    src/cbuf.c
    src/config.c
    src/json.c
    src/logger.c
    src/scheduler.c
    src/uuid4.c
)

if (PLATFORM STREQUAL linux)

    set(SKELETON_SCHEDULER_MAX_PROCESSES 64)
    set(SKELETON_SCHEDULER_MAX_CORE 4)

    file(GLOB ARCH arch-${PLATFORM}/*.c)

    add_compile_options(-Wall -Wno-unused-variable -Wno-unused-function -O0)
    
    # libSkeleton is always created 
    add_library(Skeleton STATIC ${SOURCES} ${ARCH})
    target_link_libraries(Skeleton m)
    target_include_directories(Skeleton INTERFACE include)

    add_executable(skeleton ${MAIN} src/dsp.c)

    target_link_libraries(skeleton m Skeleton)

    execute_process (
        COMMAND bash -c "mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/log"
        COMMAND bash -c "mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/data"
    )

    # copy common.json file to build dir in order to run the app
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/common.json
        ${CMAKE_CURRENT_BINARY_DIR}/common.json
        COPYONLY
    )

elseif(PLATFORM STREQUAL arm) # RP2040
    
    # creata BOARDS choice with env var
    include_directories(arch-arm/rp2040/)
    file(GLOB ARCH arch-${PLATFORM}/rp2040.c)

    set(SKELETON_SCHEDULER_MAX_PROCESSES 16)
    set(SKELETON_SCHEDULER_MAX_CORE 2)

    add_executable(skeleton ${MAIN} ${SOURCES} src/dsp.c ${ARCH})

    pico_generate_pio_header(skeleton ${CMAKE_CURRENT_LIST_DIR}/uart_tx.pio)

    pico_enable_stdio_usb(skeleton 0)
    pico_enable_stdio_uart(skeleton 0)

    add_subdirectory(arch-arm/drivers/FatFs_SPI build)
    add_subdirectory(arch-arm/drivers/pioSerial)
    add_subdirectory(arch-arm/drivers/esp32)

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
        pioSerial
        esp32
    )

    # create map/bin/hex file etc.
    pico_add_extra_outputs(skeleton)

endif()

configure_file(config/cmake.h.in ${CMAKE_CURRENT_BINARY_DIR}/generated/cmake.h)

include_directories( ${CMAKE_CURRENT_BINARY_DIR}/generated/ ) # Make sure it can be included...
