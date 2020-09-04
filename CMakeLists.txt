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

add_subdirectory(libuspp)
add_subdirectory(uspm-extra)
add_subdirectory(uspm-dev)

message(STATUS "CJSON: [${CJSON_LIB}]")

message(STATUS "USPP: [${USPP_LIB}]")

if(NOT CJSON_LIB)
    message(FATAL_ERROR "CJSON not found, please install from cJSON repo.")
endif()

if(NOT USPP_LIB)
    message(FATAL_ERROR "uspp not found, please build and install from the libuspp directory")
endif()

# USPM
add_executable(uspm uspm/main.c)
add_executable(uspm-test EXCLUDE_FROM_ALL libuspptest/main.c)

add_custom_target(uspm-all)
add_custom_command(TARGET uspm-all COMMAND make COMMAND make uspm-extended COMMAND make uspm-mkpkg)

#  add_dependencies(uspm uspp-install) doesnt work

target_link_libraries(uspm LINK_PUBLIC ${USPP_LIB} ${CJSON_LIB} curl)
target_link_libraries(uspm-test LINK_PUBLIC ${USPP_LIB} ${CJSON_LIB})

install (TARGETS uspm
    RUNTIME DESTINATION /usr/local/bin)

if (${APPLE})
    install (FILES uspm/uspm.1 DESTINATION /usr/local/share/man/man1)
else()
    install (FILES uspm/uspm.1 DESTINATION /usr/share/man/man1)
endif()
#target_link_libraries(uspm-ui LINK_PUBLIC ${NCURSES_LIB} ${CURSES_LIBRARIES})
