cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")


############ Bitreader
set(BITREADER_SOURCES
        src/bitreader.cpp)

set(BITREADER_HEADERS
        include/ruberoid/bitreader/bitreader.hpp
        include/ruberoid/data_source/memory_byte_source.hpp
        include/ruberoid/common/shared_buffer.hpp
        include/ruberoid/data_source/file_byte_source.hpp
        include/ruberoid/common/file_reader.hpp
        include/ruberoid/common/direct_file_reader.hpp
    )

add_library(bitreader STATIC ${BITREADER_SOURCES} ${BITREADER_HEADERS})
target_include_directories(bitreader PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(bitreader PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

############# Runtime
set(RUNTIME_SOURCES
        src/runtime/value.cpp
        src/runtime/instruction_registry.cpp)

set(RUNTIME_HEADERS
        include/ruberoid/runtime/value.hpp
        include/ruberoid/runtime/field.hpp
        include/ruberoid/runtime/context.hpp
        include/ruberoid/runtime/array.hpp
        include/ruberoid/runtime/instruction.hpp
        include/ruberoid/runtime/execution.hpp
        include/ruberoid/runtime/execution_context.hpp
        include/ruberoid/runtime/instruction_registry.hpp
        include/ruberoid/runtime/program_parser.hpp
        include/ruberoid/runtime/instructions/brd.hpp
        include/ruberoid/runtime/instructions/call.hpp
        include/ruberoid/runtime/instructions/ctxc.hpp
        include/ruberoid/runtime/instructions/ctxf.hpp
        include/ruberoid/runtime/instructions/ret.hpp
        include/ruberoid/runtime/instructions/slota.hpp
        include/ruberoid/runtime/instructions/slotd.hpp
        include/ruberoid/runtime/instructions/slote.hpp
        include/ruberoid/runtime/instructions/slotf.hpp
        include/ruberoid/runtime/scope_stack.hpp
        include/ruberoid/runtime/instructions/baln.hpp
        include/ruberoid/runtime/instructions/bskp.hpp
        include/ruberoid/runtime/instructions/var.hpp)

add_library(rbruntime SHARED ${RUNTIME_SOURCES} ${RUNTIME_HEADERS})
target_include_directories(rbruntime PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(rbruntime PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

############# Unit tests
enable_testing()
find_package(GTest REQUIRED)

############# Bitreader gtest
add_executable(bitreader_gtest src/bitreader_gtest.cpp)
target_include_directories(bitreader_gtest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/include
        )
target_link_libraries(bitreader_gtest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(bitreader_gtest pthread)
add_test(AllTestsBitreader bitreader_gtest)

############## Common gtest
add_executable(common_gtest
        src/shared_buffer_gtest.cpp
        src/memory_byte_source_gtest.cpp
        src/file_byte_source_gtest.cpp
        src/gtest_common.hpp
        src/gtest_common_gtest.cpp)

target_include_directories(common_gtest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/include
        )

target_link_libraries(common_gtest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(common_gtest pthread)
add_test(AllTestsCommon common_gtest)