cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}/CMake" p)
list(APPEND CMAKE_MODULE_PATH "${p}") #For old Imagine++
list(APPEND CMAKE_SYSTEM_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?
find_package(Imagine REQUIRED)
include_directories(C:/Users/alixa/Downloads/C++Python/pybind11-master/include  C:/pyzo2015a/include)

project(MOPSI)
add_executable(MOPSI main.cpp Tree.cpp Tree.h)
ImagineUseModules(MOPSI Graphics)

