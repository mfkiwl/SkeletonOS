add_library(pioSerial INTERFACE)
target_sources(pioSerial INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/pioSerial.c
)

target_include_directories(pioSerial INTERFACE
   include
)

target_link_libraries(pioSerial INTERFACE
    pico_stdlib 
    hardware_pio
)
