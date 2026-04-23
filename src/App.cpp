#include "App.h"
#include "Canvas.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Object.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>



App::App() {
    // this->canvas = new Canvas();
}

void App::initialize() {
    //this->createDemoObject(); 
    this->setupShaders();
    this->renderer = new Renderer(ResourceManager::getShader("sprite"));
    this->canvas = new Canvas();
    ResourceManager::loadTexture("textures/pixel.png", false, "px");

}

void App::createDemoObject() {
    ResourceManager::loadTexture("textures/proppasmile.png", true, "demo");
    glm::vec2 demoObjectSize(util::scrWidth/2.0f);
    glm::vec2 demoObjectPos = glm::vec2(util::scrWidth/2.0f, util::scrHeight/2.0f) - demoObjectSize/2.0f;
    this->demoObject = new Object(demoObjectPos, demoObjectSize, 0.0f, ResourceManager::getTexture("demo"));
}

void App::setupShaders() {
    ResourceManager::loadShader("./shaders/default.vert", "./shaders/default.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(util::scrWidth), static_cast<float>(util::scrHeight), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite")->use().setInteger("image", 0);
    ResourceManager::getShader("sprite")->use().setMatrix4("projection", projection);
}

void App::update(float dt) {
    if (this->mouse.leftPressed) {
        this->canvas->modify(this->mouse.xpos, this->mouse.ypos);
    }

    if (this->mouse.rightPressed) {
        this->canvas->print();
    }

    //this->canvas->print();
    this->printMouseInfo();

}

void App::render() {

    renderCanvas();
    //this->demoObject->draw(*this->renderer);

}
void App::renderCanvas() {

    for (int i = 0; i < this->canvas->pixelObjects.size(); i++) {
        for (int j = 0; j < this->canvas->pixelObjects[i].size(); j++) {
            if (this->canvas->canvas[i][j] != 0) {
                this->canvas->pixelObjects[i][j]->draw(*this->renderer);
            }
        }
    }

}

void App::printMouseInfo() {
    printf("xpos: %.0f ypos: %.0f left: %d right: %d\n", 
    this->mouse.xpos, this->mouse.ypos, this->mouse.leftPressed, this->mouse.rightPressed);
}

App::~App() {
    delete this->renderer;
    delete this->demoObject;
    delete canvas;
}