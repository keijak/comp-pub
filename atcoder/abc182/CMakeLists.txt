project(abc182)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DENABLE_DEBUG")

add_executable(rev1 F/rev1.cpp)
