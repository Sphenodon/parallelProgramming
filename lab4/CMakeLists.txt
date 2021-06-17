cmake_minimum_required(VERSION 3.16)
project(lab4 C)

#find_package(OpenMP)

set(CMAKE_C_STANDARD 11)

add_executable(main_guided.c)
target_link_libraries(lab4 m)

#if(OpenMP_C_FOUND)
#    target_link_libraries(lab4 PUBLIC OpenMP::OpenMP_C)
#endif()