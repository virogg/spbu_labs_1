cmake_minimum_required(VERSION 3.25)
project(game_1)

set(CMAKE_CXX_STANDARD 17)
set(Sources Trainer.cpp BattleArena.cpp)
set(Headers Pokemon.h Inventory.h Item.h Trainer.h BattleArena.cpp Effect.h)
add_executable(game_1 main.cpp
        Trainer.cpp
        BattleArena.cpp)
