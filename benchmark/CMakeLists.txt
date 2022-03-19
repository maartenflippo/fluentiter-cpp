cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(FluentIterBench LANGUAGES CXX)

set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:google/benchmark#v1.6.1")
CPMAddPackage(NAME FluentIter SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} benchmark::benchmark FluentIter::FluentIter)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(FluentIter INTERFACE -Wall -Wpedantic -Wextra -Werror)
elseif(MSVC)
  target_compile_options(FluentIter INTERFACE /W4 /WX)
endif()
