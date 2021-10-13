project(abc171)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")
add_executable(F F/main.cpp)

add_executable(review1 F/review1.cpp)