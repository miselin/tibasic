cmake_minimum_required(VERSION 3.15)
project(tibasic LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER ${DEPENDENCY_FOLDER})

include("cmake/compiler.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
