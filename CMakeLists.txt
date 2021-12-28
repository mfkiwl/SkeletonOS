cmake_minimum_required(VERSION 3.18.4)
project("Skeleton")

#Bring the headers, such as Student.h into the project
include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/main.c src/cbuf.c)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c")

set(CMAKE_SYSTEM_NAME linux)
set(CMAKE_CROSSCOMPILING 0)

# LINUX OS
if(CMAKE_SYSTEM_NAME STREQUAL linux)
    set(CMAKE_C_COMPILER "gcc")
    #set(CMAKE_CXX_COMPILER "/usr/bin/g++")

    add_compile_options(-Wall -Wno-unused-variable -Wno-unused-function -O0)
    add_executable(skeleton ${SOURCES})
    target_link_libraries(skeleton m)

    execute_process (
        COMMAND bash -c "mkdir ../log"
        COMMAND bash -c "mkdir ../data"
        )
        
endif()

# MCU Version
if(CMAKE_SYSTEM_NAME EQUAL bare)
    add_compile_options(-Wall -Wno-unused-variable -Wno-unused-function -O0)
    add_executable(skeleton ${SOURCES})
endif()



