project(aizu)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")

add_executable(DSL_2_C DSL_2_C/main_segtree.cpp)
