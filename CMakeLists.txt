cmake_minimum_required(VERSION 3.12)

project(a-star CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(SFML_MINIMAL_VERSION 2.5.1)
set(FLAGS -std=c++17 -Wall -g3 -fno-omit-frame-pointer -Wextra -Werror)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})
find_package(SFML ${SFML_MINIMAL_VERSION} COMPONENTS system graphics window REQUIRED)

add_subdirectory(src)


