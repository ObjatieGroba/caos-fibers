cmake_minimum_required(VERSION 3.5)

project(fibers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tests runtime.cpp tests.cpp)
set_target_properties(tests PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
