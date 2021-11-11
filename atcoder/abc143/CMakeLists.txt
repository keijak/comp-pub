project(abc143)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -D_GLIBCXX_DEBUG -DMY_DEBUG")

add_executable(F F/review1.cpp)
add_executable(E E/reviewE.cpp)
