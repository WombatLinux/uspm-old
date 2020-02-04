project(uspm)

cmake_minimum_required(VERSION 3.15)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include) # if you are building with custom includes
include_directories(/usr/local/include) # if you are building with system includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)

find_library (
        CJSON_LIB
        NAMES cjson libcjson # what to look for
        #HINTS "${CMAKE_SOURCE_DIR}/lib" # where to look
        #NO_DEFAULT_PATH # do not search system default paths
)

# check if we found the library
message(STATUS "CJSON: [${CJSON_LIB}]")

add_executable(uspm main.c )
target_link_libraries(uspm LINK_PUBLIC ${CJSON_LIB} curl)