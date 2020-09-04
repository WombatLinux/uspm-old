project(uspm)

cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libuspp)
include_directories(/usr/local/include)
include_directories(/usr/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/uspm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/uspm/include)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)

find_library (
        CJSON_LIB
        NAMES cjson libcjson
)

find_library (
        USPP_LIB
        NAMES uspp libuspp
)

message(STATUS "CJSON: [${CJSON_LIB}]")

message(STATUS "USPP: [${USPP_LIB}]")

if(NOT CJSON_LIB)
    message(FATAL_ERROR "CJSON not found, please install from cJSON repo.")
endif()

if(NOT USPP_LIB)
    message(FATAL_ERROR "uspp not found, please build and install from the libuspp directory")
endif()


add_executable(uspm uspm.c)

target_link_libraries(uspm LINK_PUBLIC ${USPP_LIB} ${CJSON_LIB})

install (TARGETS uspm
        RUNTIME DESTINATION /usr/local/bin)

if (${APPLE})
    install (FILES uspm.1 DESTINATION /usr/local/share/man/man1)
else()
    install (FILES uspm.1 DESTINATION /usr/share/man/man1)
endif()
