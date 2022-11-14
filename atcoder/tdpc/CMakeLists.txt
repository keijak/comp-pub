project(tdpc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")

add_executable(P P/main.cpp)
add_executable(S S/main_S.cpp)
add_executable(Jr J/review1.cpp)
