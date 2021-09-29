project(code_formula_2014_final)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")
add_executable(E E/main.cpp)
