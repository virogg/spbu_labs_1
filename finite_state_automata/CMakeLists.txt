cmake_minimum_required(VERSION 3.25)
project(finite_state_automata)

set(CMAKE_CXX_STANDARD 17)

add_executable(finite_state_automata main.cpp
        Tokens.h
        Hash.h
        State.h
        FSA.h)
