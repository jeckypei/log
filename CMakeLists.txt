PROJECT(liblog)
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
 
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -D_GNU_SOURCE -lpthread -O2")
SET(CMAKE_C_LDFLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -D_GNU_SOURCE -lpthread -O2")



set(log_LIB_SRCS
    src/log.c
    )

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

link_directories(
    /usr/local/lib
    )


add_library (log SHARED ${log_LIB_SRCS})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/)

