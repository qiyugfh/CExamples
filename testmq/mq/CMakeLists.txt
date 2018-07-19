cmake_minimum_required(VERSION 3.2)

if(CMAKE_COMPILER_IS_GNUXX)
	set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
	message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUXX)

project(testmq)

set(EXE_NAME testmq)
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(./src DIR_SRCS)

include_directories(./deps/mq)
add_subdirectory(./deps/mq)

link_directories(/usr/local/ActiveMQ-CPP/lib)
link_libraries(activemq-cpp pthread boost_serialization)


add_executable(${EXE_NAME} ${DIR_SRCS})

target_link_libraries(${EXE_NAME} mq)
