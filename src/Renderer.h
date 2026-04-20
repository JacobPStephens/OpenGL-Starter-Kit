#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

class Renderer {

    public:
        Renderer(Shader* shader);
        ~Renderer();

        void draw(
            glm::vec2 position, 
            glm::vec2 size,
            float rotation,
            Texture* texture, 
            glm::vec4 color
        );

    private:
        Shader* shader;
        unsigned int quadVAO;
        void initRenderData();

};
