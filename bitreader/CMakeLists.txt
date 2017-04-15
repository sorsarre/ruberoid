cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -std=c++1z -fconcepts")

set(BITREADER_SOURCES
        src/bitreader.cpp)

set(BITREADER_HEADERS
        include/ruberoid/bitreader/bitreader.hpp)

add_library(bitreader STATIC ${BITREADER_SOURCES} ${BITREADER_HEADERS})
target_include_directories(bitreader PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(bitreader PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
