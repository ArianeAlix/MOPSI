cmake_minimum_required(VERSION 2.6)
project(MOPSI)

set(CMAKE_CXX_STANDARD 11)
find_package(Imagine REQUIRED)

set(SOURCE_FILES main.cpp Ctree.h Tree.h Tree.cpp Ctree.cpp)
add_executable(MOPSI_filtre ${SOURCE_FILES})
ImagineUseModules(MOPSI_filtre Images)
