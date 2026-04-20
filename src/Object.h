#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Texture;
class Renderer;

class Object {
    public:
        glm::vec2 position;
        glm::vec2 size;
        float rotation;
        glm::vec4 color;
        Texture* texture;
        std::string name;

        Object();
        Object(glm::vec2 position, glm::vec2 size, const float tileSize, Texture* texture);

        void update(float dt);

        void draw(Renderer &renderer);
};
