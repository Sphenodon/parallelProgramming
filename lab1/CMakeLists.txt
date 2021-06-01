cmake_minimum_required(VERSION 3.16)
project(lab1 C)

#find_package(OpenMP)

set(CMAKE_C_STANDARD 11)

add_executable(lab1 main.c)
target_link_libraries(lab1 m)

#if(OpenMP_C_FOUND)
#    target_link_libraries(lab1 PUBLIC OpenMP::OpenMP_C)
#endif()