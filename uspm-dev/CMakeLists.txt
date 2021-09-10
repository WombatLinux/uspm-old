project(uspm-dev)

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


add_executable(uspm-makepkg EXCLUDE_FROM_ALL mkpkg.c)
add_executable(uspm-chksum EXCLUDE_FROM_ALL chksum.c)

target_link_libraries(uspm-makepkg LINK_PUBLIC ${USPP_LIB} ${CJSON_LIB})
target_link_libraries(uspm-chksum LINK_PUBLIC ${USPP_LIB})

install (TARGETS uspm-makepkg OPTIONAL
        RUNTIME DESTINATION /usr/local/bin)
install (TARGETS uspm-chksum OPTIONAL
        RUNTIME DESTINATION /usr/local/bin)
