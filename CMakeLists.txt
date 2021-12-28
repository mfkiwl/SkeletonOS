cmake_minimum_required(VERSION 3.18.4)
project("Skeleton")

#Bring the headers, such as Student.h into the project
include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/main.c src/cbuf.c)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c")

set(CMAKE_CROSSCOMPILING 0)


# how to use:
# 0. create build directory
# 1. cmake -D_BUILD_TYPE="linux" .
# 2. make

# LINUX OS
if(_BUILD_TYPE STREQUAL linux)
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

if(_BUILD_TYPE STREQUAL linuxStatic)

    set(CMAKE_C_COMPILER "gcc")
    #set(CMAKE_CXX_COMPILER "/usr/bin/g++")

    #Generate the static library from the sources
    add_library(skeletonStatic STATIC ${SOURCES})

endif()

# MCU Version
if(_BUILD_TYPE STREQUAL mcu)
    add_compile_options(-Wall -Wno-unused-variable -Wno-unused-function -O0)
    add_executable(skeletonMCU ${SOURCES})
endif()



# Per usare la static library:

#set ( PROJECT_LINK_LIBS libtestStudent.a )
#link_directories( ~/exploringBB/extras/cmake/studentlib_static/build )
#target_link_libraries(libtest ${PROJECT_LINK_LIBS} )

#INSTALL(TARGETS mylib DESTINATION lib)


