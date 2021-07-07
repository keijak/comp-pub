cmake_minimum_required(VERSION 3.17)
project(typical90)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -fno-sanitize-recover=address,undefined -DENABLE_DEBUG")

add_executable(083 083/main.cpp)
add_executable(084 084/main.cpp)
add_executable(085 085/main.cpp)
add_executable(086 086/main.cpp)
