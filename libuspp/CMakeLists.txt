cmake_minimum_required(VERSION 3.10)
project(libuspp C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include) # if you are building with custom includes
include_directories(/usr/local/include) # if you are building with system includes
include_directories(/usr/include) # if you are building with system includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_library (
        CJSON_LIB
        NAMES cjson libcjson # what to look for
)

message(STATUS "CJSON: [${CJSON_LIB}]")

set(CMAKE_C_STANDARD 99)

add_library(uspp dephandle.c dephandle.h fm.c fm.h uspm.c uspm.h)
#add_library(uspm uspm.c uspm.h)

target_link_libraries(uspp LINK_PUBLIC ${CJSON_LIB} curl)

install(TARGETS uspp ARCHIVE DESTINATION /usr/local/lib LIBRARY DESTINATION /usr/local/lib)
install(FILES uspm.h DESTINATION /usr/local/include/uspm)
install(FILES dephandle.h DESTINATION /usr/local/include/uspm)
install(FILES fm.h DESTINATION /usr/local/include/uspm)