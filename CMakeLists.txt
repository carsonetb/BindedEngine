cmake_minimum_required(VERSION 3.18)  # or whatever recent version you have
project(PythonCPPBind)

set(DEV_MODULE Development.Module)

find_package(Python 3.8 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_subdirectory(extern/nanobind)
add_subdirectory(extern/raylib)

nanobind_add_module(
  binding
  src/binding.cpp 
  src/engine.cpp 
  src/signal.cpp 
  src/object.cpp 
  src/callback.hpp
  src/shape.cpp
  src/input.cpp
)

set_target_properties(binding PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/python")
target_include_directories(binding PRIVATE extern/raylib-cpp/include extern/earcut)
target_link_libraries(binding PRIVATE -static -lpthread -static-libgcc -static-libstdc++ raylib)