project(uspm)

cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include) # if you are building with custom includes
include_directories(/usr/local/include) # if you are building with system includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)

# find library
find_library (
        CJSON_LIB
        NAMES cjson libcjson # what to look for
)

#find_package(Curses REQUIRED)

# check if we found the library
message(STATUS "CJSON: [${CJSON_LIB}]")
#message(STATUS "NCURSES: [${NCURSES_LIB}]")

add_executable(uspm uspm/main.c)
add_executable(uspm-mkpkg uspm-mkpkg/main.c)

target_link_libraries(uspm LINK_PUBLIC ${CJSON_LIB} curl)
target_link_libraries(uspm-mkpkg LINK_PUBLIC ${CJSON_LIB})

install (TARGETS uspm
    RUNTIME DESTINATION /usr/local/bin)

#install (FILES manpage DESTINATION /us)
#target_link_libraries(uspm-ui LINK_PUBLIC ${NCURSES_LIB} ${CURSES_LIBRARIES})
