project(round_58)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DMY_DEBUG")
add_executable(path_inversions path_inversions/main_path_inversions.cpp)
