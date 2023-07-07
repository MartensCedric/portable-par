cmake_minimum_required(VERSION 3.17)
project(gmtk-2023)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image OpenGL::GL GLUT::GLUT GLEW)

