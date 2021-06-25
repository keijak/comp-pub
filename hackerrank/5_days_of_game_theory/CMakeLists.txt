cmake_minimum_required(VERSION 3.17)
project(5_days_of_game_theory)

set(CMAKE_CXX_STANDARD 20)

add_executable(a_chessboard_game a_chessboard_game/main.cpp)
add_executable(deforestation deforestation/main.cpp)
add_executable(nim_game nim_game/main.cpp)
add_executable(misere_nim misere_nim/main.cpp)
add_executable(nimble nimble/main.cpp)
add_executable(poker_nim poker_nim/main.cpp)
