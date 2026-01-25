add_library(glad)

target_include_directories(glad PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

#message("cmake curr source dir: " ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(glad
    PUBLIC
    src/glad.c
    include/glad/glad.h
)
