project(jsc2019_qual)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")
add_executable(F F/main.cpp)
