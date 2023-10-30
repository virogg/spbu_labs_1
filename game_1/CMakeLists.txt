cmake_minimum_required(VERSION 3.25)
project(game_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(game_1 main.cpp Pokemon.h Inventory.h Item.h Trainer.h BattleArena.h)
