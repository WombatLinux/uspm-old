project(uspm)

cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include) # if you are building with custom includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libuspp) # if you are building with custom includes
include_directories(/usr/local/include) # if you are building with system includes
include_directories(/usr/include) # if you are building with system includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/uspm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/uspm/include)

add_subdirectory(libuspp)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)

find_library (
        CJSON_LIB
        NAMES cjson libcjson
)

# check if we found the library
message(STATUS "CJSON: [${CJSON_LIB}]")


# USPM
add_executable(uspm uspm/main.c)
add_executable(uspm-mkpkg uspm-mkpkg/main.c)
add_executable(uspm-extended uspm-extended/main.c)

target_link_libraries(uspm LINK_PUBLIC uspp ${CJSON_LIB} curl)
#target_link_libraries(uspm ${CMAKE_CURRENT_SOURCE_DIR}/../libs/MacOS/libfreetype.a)
target_link_libraries(uspm-extended LINK_PUBLIC uspp ${CJSON_LIB} curl)
target_link_libraries(uspm-mkpkg LINK_PUBLIC ${CJSON_LIB})

install (TARGETS uspm
    RUNTIME DESTINATION /usr/local/bin)

install (TARGETS uspm-mkpkg
        RUNTIME DESTINATION /usr/local/bin)

install (TARGETS uspm-extended
        RUNTIME DESTINATION /usr/local/bin)

#install (FILES manpage DESTINATION /us)
#target_link_libraries(uspm-ui LINK_PUBLIC ${NCURSES_LIB} ${CURSES_LIBRARIES})
