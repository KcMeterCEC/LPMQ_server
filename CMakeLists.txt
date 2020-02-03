cmake_minimum_required(VERSION 3.3.2)

# function <start>
macro(lpmqd_append_sources)
    set(LPMQD_SOURCES ${LPMQD_SOURCES} ${ARGV} CACHE INTERNAL "global source" FORCE)
endmacro(lpmqd_append_sources)
# function <end>

# initialize var <start>
unset(LPMQD_SOURCES CACHE)
# initialize var <end>

project("lpmqd")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_BUILD_TYPE "Debug")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

SET(COMPILE_FLAGS)

add_subdirectory(${PROJECT_SOURCE_DIR}/worker)
include_directories(${PROJECT_SOURCE_DIR}/worker)
add_subdirectory(${PROJECT_SOURCE_DIR}/process)
include_directories(${PROJECT_SOURCE_DIR}/process)
add_subdirectory(${PROJECT_SOURCE_DIR}/memory)
include_directories(${PROJECT_SOURCE_DIR}/memory)
add_subdirectory(${PROJECT_SOURCE_DIR}/io)
include_directories(${PROJECT_SOURCE_DIR}/io)

list(APPEND CUR_SOURCE 
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/server.cpp
    ${LPMQD_SOURCES}
    )

add_executable(${PROJECT_NAME} ${CUR_SOURCE})
target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11)

if(CMAKE_CROSSCOMPILING AND STATIC_LINK)
    message(STATUS "static link is used for crosscompile mode.")
    target_link_libraries(${PROJECT_NAME} PUBLIC -pthread "-static")
else(CMAKE_CROSSCOMPILING AND STATIC_LINK)
    target_link_libraries(${PROJECT_NAME} -pthread)
endif(CMAKE_CROSSCOMPILING AND STATIC_LINK)



