project(university_codesprint_5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DMY_DEBUG")
add_executable(cube_loving_numbers cube_loving_numbers/main_cube_loving_numbers.cpp)
