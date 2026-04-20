add_executable(app)

target_sources(app
    PRIVATE
        ./main.cpp
        ./App.cpp
        ./Object.cpp
        ./Renderer.cpp
        ./ResourceManager.cpp
        ./Shader.cpp
        ./Texture.cpp
)

target_link_libraries(app
    PRIVATE
        glad
        glm::glm
        glfw
        stb
)