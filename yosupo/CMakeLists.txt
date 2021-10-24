project(yosupo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")

add_executable(rmq_fischer_heun staticrmq/fischer_heun.cpp)
add_executable(lca_fischer_heun lca/fischer_heun.cpp)

add_executable(rectangle_sum rectangle_sum/main.cpp)
add_executable(persistent_queue persistent_queue/main_persistent_queue.cpp)
