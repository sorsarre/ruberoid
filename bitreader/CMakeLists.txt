cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(BITREADER_SOURCES
        src/bitreader.cpp)

set(BITREADER_HEADERS
        include/ruberoid/bitreader/bitreader.hpp)

add_library(bitreader STATIC ${BITREADER_SOURCES} ${BITREADER_HEADERS})
target_include_directories(bitreader PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(bitreader PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

enable_testing()
find_package(GTest REQUIRED)

add_executable(bitreader_gtest src/bitreader_gtest.cpp)
target_include_directories(bitreader_gtest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/include
        )
target_link_libraries(bitreader_gtest ${GTEST_BOTH_LIBRARIES})
add_test(AllTestsBitreader bitreader_gtest)