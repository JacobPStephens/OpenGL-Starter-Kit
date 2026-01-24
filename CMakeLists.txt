cmake_minimum_required(VERSION 3.23)
project(glmTest)

find_package(OpenGL REQUIRED)

add_subdirectory(./external/glad)
add_subdirectory(./external/glm)
add_subdirectory(./external/glfw)
add_subdirectory(src)