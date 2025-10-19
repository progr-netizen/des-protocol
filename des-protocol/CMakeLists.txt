cmake_minimum_required(VERSION 3.16)
project(DES LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(des STATIC
  src/des.cpp
)

target_include_directories(des PUBLIC src)

add_executable(des_cli src/main.cpp)
target_link_libraries(des_cli des)

add_executable(des_tests tests/test_vectors.cpp)
target_link_libraries(des_tests des)
