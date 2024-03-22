add_library(cmake_cpp_tibasic_compiler_options INTERFACE)

option(WARNINGS_ARE_ERRORS "Disallow warnings by treating them as errors" OFF)
option(ASAN "Enable address sanitizer" OFF)
option(MSAN "Enable memory sanitizer" OFF)
option(TSAN "Enable thread sanitizer" OFF)
option(UBSAN "Enable undefined behavior sanitizer" OFF)

if (MSVC)
  target_compile_options(cmake_cpp_tibasic_compiler_options INTERFACE /W4 "/permissive-")

  if (WARNINGS_ARE_ERRORS)
    target_compile_options(cmake_cpp_tibasic_compiler_options INTERFACE /WX)
  endif ()
else ()
  target_compile_options(cmake_cpp_tibasic_compiler_options INTERFACE
    -Wall -Wextra -Wpedantic -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5
    -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
    -Wformat=2 -Winit-self)
  target_compile_definitions(cmake_cpp_tibasic_compiler_options INTERFACE _POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700 _DEFAULT_SOURCE)

  if (WARNINGS_ARE_ERRORS)
    target_compile_options(cmake_cpp_tibasic_compiler_options INTERFACE -Werror)
  endif ()
endif ()

if (ASAN)
  target_compile_options(cmake_cpp_tibasic_compiler_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
  target_link_libraries(cmake_cpp_tibasic_compiler_options INTERFACE -fsanitize=address)
endif ()

if (MSAN)
  target_compile_options(cmake_cpp_tibasic_compiler_options INTERFACE -fsanitize=memory -fno-omit-frame-pointer)
  target_link_libraries(cmake_cpp_tibasic_compiler_options INTERFACE -fsanitize=memory)
endif ()

if (TSAN)
  target_compile_options(cmake_cpp_tibasic_compiler_options INTERFACE -fsanitize=thread)
  target_link_libraries(cmake_cpp_tibasic_compiler_options INTERFACE -fsanitize=thread)
endif ()

if (UBSAN)
  target_compile_options(cmake_cpp_tibasic_compiler_options INTERFACE -fsanitize=undefined)
  target_link_libraries(cmake_cpp_tibasic_compiler_options INTERFACE -fsanitize=undefined)
endif ()
