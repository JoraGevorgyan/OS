cmake_minimum_required(VERSION 3.2)

project(main VERSION 1.0)

add_subdirectory(lib)
add_subdirectory(program)
target_compile_options(main PUBLIC -msse4 -pthread)
target_link_options(main PUBLIC -pthread)
