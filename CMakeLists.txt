cmake_minimum_required(VERSION 3.23)
project(Proyecto2_Inmersive_Checkers)

set(CMAKE_CXX_STANDARD 14)
find_package(SFML 2.5 COMPONENTS system window graphics)

add_executable(Proyecto2_Inmersive_Checkers src/Main.cpp)
target_link_libraries(Proyecto2_Inmersive_Checkers sfml-system sfml-window sfml-graphics)
