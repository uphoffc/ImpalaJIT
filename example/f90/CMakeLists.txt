cmake_minimum_required(VERSION 2.6)
project(impala-f90)
enable_language (Fortran)

add_subdirectory(${PROJECT_SOURCE_DIR}/root)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCE_DIR}/example.F90)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/root/include)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -ffree-line-length-512)
target_link_libraries(${CMAKE_PROJECT_NAME} impalajit-static)


file(COPY ${PROJECT_SOURCE_DIR}/example.conf DESTINATION .)
file(COPY ${PROJECT_SOURCE_DIR}/impala_file DESTINATION .)