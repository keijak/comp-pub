project(world_codesprint_10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")
add_executable(permutation_happiness permutation_happiness/main.cpp)
