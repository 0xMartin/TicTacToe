cmake_minimum_required(VERSION 3.5)

project(TicTacToe LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data/)


file(GLOB sources "*.c" "obj/*.c")
file(GLOB headers "*.h" "obj/*.h")

add_compile_options(-Wall -Wextra -std=c11)

add_executable(${PROJECT_NAME} ${headers} ${sources})

add_subdirectory(s3d)
target_link_libraries(${PROJECT_NAME} PRIVATE s3d)

