cmake_minimum_required(VERSION 2.6)
find_package(GTest REQUIRED)

include(CTest)

add_executable(basic_tests ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/basic.cpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/parameter_count.cpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/simple_conditionals.cpp)
target_include_directories(basic_tests PRIVATE ${GTEST_INCLUDE_DIRS} ../include)
target_link_libraries(basic_tests PRIVATE ${TARGET_NAME} ${GTEST_LIBRARIES})


add_executable(advanced_tests ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                              ${CMAKE_CURRENT_SOURCE_DIR}/complex_conditionals.cpp
                              #${CMAKE_CURRENT_SOURCE_DIR}/many_ifs.cpp
                              ${CMAKE_CURRENT_SOURCE_DIR}/multiple_functions.cpp)
target_include_directories(advanced_tests PRIVATE ${GTEST_INCLUDE_DIRS} ../include)
target_link_libraries(advanced_tests PRIVATE ${TARGET_NAME} ${GTEST_LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/impala_files DESTINATION .)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/conf DESTINATION .)
add_test(NAME basic_tests COMMAND basic_tests)
add_test(NAME advanced_tests COMMAND advanced_tests)