project(yukicoder)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address,undefined -DMY_DEBUG")

add_executable(1600 1600/main.cpp)
add_executable(1622 1622/main.cpp)
add_executable(1623 1623/main.cpp)
add_executable(1624 1624/main.cpp)
add_executable(1625 1625/main.cpp)
add_executable(1626 1626/main.cpp)
add_executable(1627 1627/main.cpp)

