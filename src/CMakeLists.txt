
add_library(internals STATIC tibasic.cpp tibasic.h tokens.cpp)
target_link_libraries(internals cmake_cpp_tibasic_compiler_options)

add_executable(tibasic main.cpp)
target_link_libraries(tibasic internals cmake_cpp_tibasic_compiler_options)
