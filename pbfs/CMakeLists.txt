cmake_minimum_required(VERSION 3.10)

project(PBFS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(pbfs main.cpp graph.h graph.cpp pennant.h pennant.cpp bag.h bag.cpp randomuniformbuilder.h randomuniformbuilder.cpp executionstrategy.h)

