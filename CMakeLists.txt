cmake_minimum_required(VERSION 3.4...3.18)
project(sqlite_blaster_python)

add_subdirectory(pybind11)
pybind11_add_module(sqlite_blaster_python src/main.cpp)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(sqlite_blaster_python
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
