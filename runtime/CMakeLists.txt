cmake_minimum_required(VERSION 3.7)

############# Runtime
set(RUNTIME_SOURCES
        src/value.cpp
        src/instruction_registry.cpp)

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
target_include_directories(rbruntime PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(rbruntime PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(rbruntime bitreader)
