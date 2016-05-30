cmake_minimum_required (VERSION 2.8.11)
project (hash_test)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++14")

include_directories(../src)

add_executable(hash_test
	main.cpp
	../src/hash3_hash3.h
	../src/hash3_detail.h
	../src/hash3_int3.h
	../src/hash3_vector3.h
	particle.h	
)
