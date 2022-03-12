project(aizu)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")

add_executable(DSL_2_C DSL_2_C/main_segtree.cpp)
add_executable(2446 2446/main_2446.cpp)
add_executable(2136 2136/main_2136.cpp)
