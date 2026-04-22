cmake_minimum_required(VERSION 3.23)
project(opengl_kit_v2)

# google test requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(tests)
add_subdirectory(./external/glad)
add_subdirectory(./external/glm)
add_subdirectory(./external/glfw)
add_subdirectory(./external/stb)
add_subdirectory(src)