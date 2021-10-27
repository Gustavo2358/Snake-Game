cmake_minimum_required(VERSION 3.11)

project(snake)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp snake.cpp snakebody.cpp)

enable_abcg(${PROJECT_NAME})
