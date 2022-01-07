project(first_vil)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DMY_DEBUG")
add_executable(i_hate_cleaning_robot i_hate_cleaning_robot/main_i_hate_cleaning_robot.cpp)
