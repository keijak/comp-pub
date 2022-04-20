cmake_minimum_required(VERSION 3.17)
project(typical90)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -fno-sanitize-recover=address,undefined -DMY_DEBUG")

add_executable(review 031/review1.cpp)

