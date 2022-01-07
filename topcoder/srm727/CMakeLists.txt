project(srm727)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DMY_DEBUG")

cmake_minimum_required(VERSION 3.20)
find_package(GTest CONFIG REQUIRED)

add_executable(hard hard/main_hard.cpp)
target_link_libraries(hard PRIVATE GTest::gtest GTest::gmock GTest::gtest_main)
