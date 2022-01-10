project(sgsw)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DMY_DEBUG")

add_executable(alice_loves_segment alice_loves_segment/main_segtree.cpp)
