cmake_minimum_required(VERSION 3.10)

project(PBFS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenMP)

add_executable(pbfs main.cpp graph.h graph.cpp pennant.h pennant.cpp bag.h bag.cpp graphbuilder.h textfileloader.h textfileloader.cpp randomuniformbuilder.h randomuniformbuilder.cpp executionstrategy.h)

target_link_libraries(pbfs OpenMP::OpenMP_CXX)

if(MSVC)
	set_property(TARGET pbfs PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")


	# We may need to copy OpenMP .dlls in case we use LLVM Clang
	if (DEFINED OpenMP_DIR)
		add_custom_command(TARGET pbfs
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${OpenMP_DIR}/bin/libomp.dll"
                    $<TARGET_FILE_DIR:pbfs>)

		add_custom_command(TARGET pbfs
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${OpenMP_DIR}/bin/libiomp5md.dll"                    
                    $<TARGET_FILE_DIR:pbfs>)
	endif(DEFINED OpenMP_DIR)

endif()


