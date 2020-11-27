cmake_minimum_required(VERSION 3.10)

project(PBFS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add_executable(pbfs main.cpp graph.h graph.cpp pennant.h pennant.cpp bag.h bag.cpp randomuniformbuilder.h randomuniformbuilder.cpp executionstrategy.h)
add_executable(pbfs main.cpp graph.h graph.cpp pennant.h pennant.cpp bag.h bag.cpp graphbuilder.h textfileloader.h textfileloader.cpp randomuniformbuilder.h randomuniformbuilder.cpp executionstrategy.h)

if(MSVC)
	set_property(TARGET pbfs PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()


