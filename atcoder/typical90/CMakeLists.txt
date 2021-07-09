cmake_minimum_required(VERSION 3.17)
project(typical90)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -fno-sanitize-recover=address,undefined -DENABLE_DEBUG")

add_executable(087 087/main.cpp)
add_executable(088 088/main.cpp)
