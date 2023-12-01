cmake_minimum_required(VERSION 3.26)
project(Task_Manager)

set(CMAKE_CXX_STANDARD 17)

add_executable(Task_Manager src/Main.cpp
        src/Util.cpp
        src/Util.h
        src/Program.cpp
        src/Program.h
        src/Task.cpp
        src/Task.h
        main.cpp)
