cmake_minimum_required(VERSION 3.7)

############# ENN #1
set(ENN1_SOURCES src/enn1.cpp)
add_executable(enn1 ${ENN1_SOURCES})
target_link_libraries(enn1 rbruntime)
