cmake_minimum_required(VERSION 3.14)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(lanceur main.cpp Automaton.cpp Automaton.h State.cpp State.h Transition.cpp Transition.h AutomatonHelper.cpp AutomatonHelper.h utils.cpp utils.h)