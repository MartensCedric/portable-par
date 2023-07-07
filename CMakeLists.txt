cmake_minimum_required(VERSION 3.17)
project(gmtk-2023)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image OpenGL::GL GLEW)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets/ ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(${PROJECT_NAME} copy_assets)
