cmake_minimum_required(VERSION 3.16)
project(lab2 C)

set(CMAKE_C_STANDARD 11)
set(IPPROOT ~/intel/oneapi/ipp/latest)

find_library(IPP_CORE libippcore.so PATHS ${IPPROOT}/lib/intel64/)
find_library(IPP_VM libippvm.so PATHS ${IPPROOT}/lib/intel64/)
find_library(IPP_S libipps.so PATHS ${IPPROOT}/lib/intel64/)
find_library(IPP_I libippi.so PATHS ${IPPROOT}/lib/intel64/)

add_executable(lab2 main.c)

target_include_directories(lab2 PRIVATE ${IPPROOT}/include)
target_link_libraries(lab2 m ${IPP_CORE} ${IPP_VM} ${IPP_I} ${IPP_S})
