cmake_minimum_required (VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2")

project(Everything)

add_subdirectory(Core)
add_subdirectory(Editor)
add_subdirectory(Benchmark)
add_subdirectory(Test)
