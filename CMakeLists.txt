cmake_minimum_required(VERSION 4.0)
project(Practica_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Practica_1 main.cpp
        ds/stack.hpp
        ds/queue.hpp
        ds/linked_list.hpp
        game/powerups.hpp
        game/edge.hpp
        game/square.hpp
        game/player.hpp
        game/board.hpp
        game/board.cpp
        game/turn_manager.hpp
        core/util.hpp
        core/io.hpp
        core/io.cpp
        game/engine.hpp
        game/engine.cpp)
