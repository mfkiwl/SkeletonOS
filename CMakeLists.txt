cmake_minimum_required(VERSION 3.18.4)
project("Skeleton")

set(SKELETON_VERSION v1.0)

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


if(_BUILD_TYPE STREQUAL linux)
    
    set(CMAKE_C_COMPILER "gcc")
    #set(CMAKE_CXX_COMPILER "/usr/bin/g++")

    set(SKELETON_SCHEDULER_MAX_PROCESSES 32)

    set(ARCH arch-linux/linux.c)

    add_compile_options(-Wall -Wno-unused-variable -Wno-unused-function -O0)
    add_executable(skeleton ${SOURCES} ${ARCH})
    target_link_libraries(skeleton m)

    execute_process (
        COMMAND bash -c "mkdir ../log"
        COMMAND bash -c "mkdir ../data"
        ) 

elseif(_BUILD_TYPE STREQUAL linuxStatic)

    set(CMAKE_C_COMPILER "gcc")
    #set(CMAKE_CXX_COMPILER "/usr/bin/g++")

    set(SKELETON_SCHEDULER_MAX_PROCESSES 32)
    set(ARCH arch-linux/linux.c)

    #Generate the static library from the sources
    add_library(skeletonStatic STATIC ${SOURCES})

elseif(_BUILD_TYPE STREQUAL arm)
    
    set(ARCH arch-arm/rp2040.c)

    add_compile_options(-Wall -Wno-unused-variable -Wno-unused-function -O0)
    add_executable(skeletonArm ${SOURCES})

else()           
    message( SEND_ERROR "_BUILD_TYPE not correct ! please try right target, example: $ cmake -D_BUILD_TYPE=\"linux\" . ")
endif()


configure_file(config/cmake.h.in ${CMAKE_BINARY_DIR}/generated/cmake.h )
include_directories( ${CMAKE_BINARY_DIR}/generated/ ) # Make sure it can be included...

# Per usare la static library:

#set ( PROJECT_LINK_LIBS libtestStudent.a )
#link_directories( ~/exploringBB/extras/cmake/studentlib_static/build )
#target_link_libraries(libtest ${PROJECT_LINK_LIBS} )

#INSTALL(TARGETS mylib DESTINATION lib)


