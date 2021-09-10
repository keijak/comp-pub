project(abc177)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -D_GLIBCXX_DEBUG -DMY_DEBUG")

add_executable(F1 F/review1.cpp)
