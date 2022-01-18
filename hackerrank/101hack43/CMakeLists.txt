project(101hack43)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DMY_DEBUG")
add_executable(k_inversion_permutations k_inversion_permutations/main_k_inversion_permutations.cpp)
