cmake_minimum_required(VERSION 3.16)

project(homework-assignment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Keep raylib build minimal for this app.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../external/raylib ${CMAKE_CURRENT_BINARY_DIR}/raylib)

file(GLOB_RECURSE HOMEWORK_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE HOMEWORK_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

add_executable(homework-assignment ${HOMEWORK_SOURCES} ${HOMEWORK_HEADERS})

target_link_libraries(homework-assignment PRIVATE raylib)

if(WIN32)
    target_link_libraries(homework-assignment PRIVATE winmm)
endif()
