project(spoj)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DENABLE_DEBUG")
add_executable(SUBXOR SUBXOR/main.cpp)
