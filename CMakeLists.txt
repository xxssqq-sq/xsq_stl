cmake_minimum_required(VERSION 3.16)
project(xsq_stl)

set(CMAKE_CXX_STANDARD 11)

add_executable(xsq_stl main.cpp stl_alloc.h stl_config.h stl_iterator.h stl_iterator_base.h stl_algobase.h type_traits.h stl_list.h)