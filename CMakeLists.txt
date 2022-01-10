cmake_minimum_required(VERSION 3.12)

if((_BUILD_TYPE STREQUAL macos) OR (_BUILD_TYPE STREQUAL linux))

set(PLATFORM linux)

project(skeleton C CXX)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

add_subdirectory(skeleton)


else() # RP2040 SDK

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(skeleton_proto C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

# Add blink example
add_subdirectory(blink)

# Add Skeleton
add_subdirectory(skeleton)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )
endif()

execute_process (
    COMMAND bash -c "mkdir ../log"
    COMMAND bash -c "mkdir ../data"
    ) 
