#include "App.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Object.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

App::App(unsigned int scrWidth, unsigned int scrHeight) {
    this->scrWidth = scrWidth;
    this->scrHeight = scrHeight;
}

void App::initialize() {
    this->createDemoObject(); 
    this->setupShaders();
    this->renderer = new Renderer(ResourceManager::getShader("sprite"));
}

void App::createDemoObject() {
    ResourceManager::loadTexture("textures/proppasmile.png", true, "demo");
    glm::vec2 demoObjectSize(this->scrWidth/2.0f);
    glm::vec2 demoObjectPos = glm::vec2(this->scrWidth/2.0f, this->scrHeight/2.0f) - demoObjectSize/2.0f;
    this->demoObject = new Object(demoObjectPos, demoObjectSize, 0.0f, ResourceManager::getTexture("demo"));
}

void App::setupShaders() {
    ResourceManager::loadShader("./shaders/default.vert", "./shaders/default.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->scrWidth), static_cast<float>(this->scrHeight), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite")->use().setInteger("image", 0);
    ResourceManager::getShader("sprite")->use().setMatrix4("projection", projection);
}

void App::update(float dt) {
    // this->printMouseInfo();
}

void App::render() {
    this->demoObject->draw(*this->renderer);
}

void App::printMouseInfo() {
    printf("xpos: %.0f ypos: %.0f left: %d right: %d\n", 
    this->mouse.xpos, this->mouse.ypos, this->mouse.leftPressed, this->mouse.rightPressed);
}

App::~App() {
    delete this->renderer;
    delete this->demoObject;
}