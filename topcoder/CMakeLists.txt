project(topcoder)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")

add_executable(srm694_div2_hard srm694/div2_hard.cpp)
