project(arc125)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -D_GLIBCXX_DEBUG -DMY_DEBUG")
add_executable(A A/main.cpp)
add_executable(B B/main.cpp)
add_executable(C C/main.cpp)
add_executable(D D/main.cpp)

add_executable(Dtle D/tle.cpp)
