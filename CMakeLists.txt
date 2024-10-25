cmake_minimum_required(VERSION 3.12)
project(
  xiulibc
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Glob source files
file(GLOB_RECURSE SOURCES 
  "src/*.cpp"
  "src/*.hpp"
  "src/*.h"
)

add_library(xiulibc ${SOURCES})

if(MSVC)
  target_compile_options(xiulibc PRIVATE /W4)
else()
  target_compile_options(xiulibc PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rules
install(TARGETS xiulibc
  EXPORT xiulibcTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY include/
  DESTINATION include
)
