project(sgsw)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DMY_DEBUG")
add_executable(frog_in_the_well frog_in_the_well/main_frog_in_the_well.cpp)
