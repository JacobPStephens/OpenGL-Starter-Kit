add_executable(game)

target_sources(game
    PRIVATE
        Eyes.cpp
        Game.cpp
        GameObject.cpp
        Ghost.cpp
        main.cpp
        PlayerObject.cpp
        ResourceManager.cpp
        Shader.cpp
        SpriteRenderer.cpp
        Texture2D.cpp
)

target_link_libraries(game
    PRIVATE
        glad
        glm::glm
        glfw
        stb
)