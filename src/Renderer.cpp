#include "Renderer.h"

// constructor
Renderer::Renderer(Shader* shader) {
    this->shader = shader;
    this->initRenderData();
};
// deconstructor
Renderer::~Renderer(){
    glDeleteVertexArrays(1, &this->quadVAO);
}

void Renderer::initRenderData() {

    unsigned int VBO;
    float vertices[] = {
        // pos      // texture
        0.0f, 1.0f,   0.0f, 1.0f,
        1.0f, 0.0f,   1.0f, 0.0f,
        0.0f, 0.0f,   0.0f, 0.0f,
        
        0.0f, 1.0f,   0.0f, 1.0f,
        1.0f, 1.0f,   1.0f, 1.0f,
        1.0f, 0.0f,   1.0f, 0.0f
    };

    // VAO, VBO is area of confusion for me
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // unbinds
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Renderer::draw( glm::vec2 position, glm::vec2 size, float rotate, Texture* texture, glm::vec4 color) {
    this->shader->use();
    // initialize model matrix as identity
    glm::mat4 model = glm::mat4(1.0);

    // translation
    model = glm::translate(model, glm::vec3(position, 0.0f));
    // rotation
    model = glm::translate(model, glm::vec3(size.x * 0.5, size.y * 0.5, 0.0));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0, 0.0, 1.0));
    model = glm::translate(model, glm::vec3(size.x * -0.5, size.y * -0.5, 0.0));
    // scaling
    model = glm::scale(model, glm::vec3(size, 1.0));
    // updates uniform variables
    this->shader->setMatrix4("model", model);
    this->shader->setVector4f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}