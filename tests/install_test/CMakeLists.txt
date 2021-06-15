cmake_minimum_required(VERSION 2.6)
project(install_test)

find_package(ImpalaJIT REQUIRED)
message(STATUS "IMPALAJIT_DIR: ${IMPALAJIT_DIR}")

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCE_DIR}/example.cc)
target_link_libraries(${CMAKE_PROJECT_NAME} LLVM::impalajit)

file(COPY ${PROJECT_SOURCE_DIR}/example.conf DESTINATION .)
file(COPY ${PROJECT_SOURCE_DIR}/impala_file DESTINATION .)
