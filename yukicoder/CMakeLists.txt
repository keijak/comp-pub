project(yukicoder)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")

add_executable(1068 1068/review1.cpp)
add_executable(0058 0058/main.cpp)
