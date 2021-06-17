cmake_minimum_required(VERSION 3.16)
project(lab3 C)

#find_package(OpenMP)

set(CMAKE_C_STANDARD 11)

add_executable(lab3 main_static.c)
target_link_libraries(lab3 m)

#if(OpenMP_C_FOUND)
#    target_link_libraries(lab3 PUBLIC OpenMP::OpenMP_C)
#endif()