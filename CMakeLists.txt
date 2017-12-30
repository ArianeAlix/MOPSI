cmake_minimum_required(VERSION 3.8)
project(MOPSI_v2)

set(CMAKE_CXX_STANDARD 11)
find_package(Imagine REQUIRED)

set(SOURCE_FILES main.cpp Ctree.h Tree.h Tree.cpp Ctree.cpp)
add_executable(MOPSI_v2 ${SOURCE_FILES})
ImagineUseModules(MOPSI_v2 Images)
