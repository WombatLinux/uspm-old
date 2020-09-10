cmake_minimum_required(VERSION 3.10)
project(libuspp C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(/usr/local/include)
include_directories(/usr/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_library (
        CJSON_LIB
        NAMES cjson libcjson # what to look for
)

find_library(CURL_LIB curl)

find_package(OpenSSL REQUIRED)

message(STATUS "[${CJSON_LIB}] [${CURL_LIB}] [${OPENSSL}]")

set(CMAKE_C_STANDARD 99)

add_library(uspp SHARED dephandle.c dephandle.h fm.c fm.h uspp.c uspp.h)
#add_library(uspm uspm.c uspm.h)

target_link_libraries(uspp LINK_PUBLIC ${CJSON_LIB} curl OpenSSL::SSL)

add_custom_target(uspp-install)
add_custom_command(TARGET uspp-install COMMAND cmake --install .)

if (${APPLE})
    install(TARGETS uspp ARCHIVE DESTINATION /usr/local/lib LIBRARY DESTINATION /usr/local/lib)
    install(FILES uspp.h DESTINATION /usr/local/include/uspm)
    install(FILES dephandle.h DESTINATION /usr/local/include/uspm)
    install(FILES fm.h DESTINATION /usr/local/include/uspm)
else()
    install(TARGETS uspp ARCHIVE DESTINATION /usr/local/lib LIBRARY DESTINATION /usr/local/lib)
    install(FILES uspp.h DESTINATION /usr/include/uspm)
    install(FILES dephandle.h DESTINATION /usr/include/uspm)
    install(FILES fm.h DESTINATION /usr/include/uspm)
endif()