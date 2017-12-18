cmake_minimum_required(VERSION 3.9)
project(KnnMPI)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES code/source/Main.cpp code/header/Util.h code/source/Util.cpp code/source/Matrix.cpp)
add_executable(KnnMPI ${SOURCE_FILES})