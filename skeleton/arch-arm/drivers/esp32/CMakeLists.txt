add_library(esp32 INTERFACE)
target_sources(esp32 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/esp32.c
)

target_include_directories(esp32 INTERFACE
   include
)

target_link_libraries(esp32 INTERFACE
    pico_stdlib
)
