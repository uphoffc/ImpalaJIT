# Copyright 2017 Manuel Fasching <manuel.fasching@tum.de>
# Distributed under the MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is furnished
# to do so, subject to the following conditions:
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


cmake_minimum_required(VERSION 2.6)
project(ImpalaJIT)

option(SHARED_LIB "Compile the shared library" ON)
option(TESTS "Enable Tests" OFF)

set(source_files compiler/frontend/parser.cc
                 compiler/frontend/scanner.cc
                 compiler/driver.cc
                 compiler/function_context.cc
                 compiler/include/nodes/node.h
                 impalajit.cc
                 compiler/semantic_analysis/semantic_analyzer.cc
                 compiler/code-gen/code_generator.cpp
                 compiler/code-gen/codegen_visitor.cpp
                 compiler/code-gen/pretty_printer.cpp
                 compiler/engine/engine.cpp
                 compiler/engine/optimizer.cpp
                 compiler/engine/std_math_lib.cpp)

# Note: this is due to an old cmake style. One can use a version range starting from cmake@3.19
find_package(LLVM 11.1 QUIET)
if (NOT LLVM_FOUND)
    find_package(LLVM 11.0 QUIET)
endif()
if (NOT LLVM_FOUND)
    find_package(LLVM 10.0 REQUIRED)
endif()

LLVM_MAP_COMPONENTS_TO_LIBNAMES(LLVM_LIBS core orcjit native)
message(STATUS "LLVM: ${LLVM_VERSION}")


if(SHARED_LIB)
    add_library(impalajit SHARED ${source_files})
else()
    add_library(impalajit STATIC ${source_files})
endif()

target_include_directories(impalajit PRIVATE include
                                             include/impalajit
                                             compiler/include
                                             compiler/include/nodes
                                             compiler/include/types
                                             compiler/frontend/include
                                             compiler/semantic_analysis/include
                                             compiler/code-gen/include
                                             compiler/engine)


target_compile_options(impalajit PRIVATE -std=c++14 -O3 -fPIC)
target_link_libraries(impalajit PUBLIC -rdynamic -lm)

# assume that llvm components are given as static libraries
target_link_libraries(impalajit PRIVATE ${LLVM_LIBS})
target_include_directories(impalajit PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(impalajit PRIVATE ${LLVM_DEFINITIONS})


if (TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

set(IMPALA_CMAKE_FILES lib/impalajit/cmake)
install(TARGETS impalajit DESTINATION lib
        EXPORT ImpalaJITTargets
        INCLUDES DESTINATION include)
install(EXPORT ImpalaJITTargets DESTINATION ${IMPALA_CMAKE_FILES})

install(FILES
        include/impalajit.hh
        include/impalajit.f90
        DESTINATION
        include)

install(FILES
        include/impalajit/types.hh
        DESTINATION
        include/impalajit)

install(FILES
        DESTINATION
        lib/cmake)

include(CMakePackageConfigHelpers)
configure_package_config_file(share/ImpalaJITConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/ImpalaJITConfig.cmake
        INSTALL_DESTINATION ${IMPALA_CMAKE_FILES})

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/ImpalaJITConfigVersion.cmake
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ImpalaJITConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ImpalaJITConfigVersion.cmake
        DESTINATION ${IMPALA_CMAKE_FILES})


# uninstall target
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)


# code generation target
add_custom_target(generate
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/generate.cmake
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
