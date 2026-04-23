#pragma once
#include <glm/glm.hpp>
#include "../util.h"

class Canvas;
class Object;
class Renderer;

class App {
    public:

        App();
        ~App();

        void initialize();
        void createDemoObject();
        void setupShaders();

        void update(float dt);
        void render();
        void renderCanvas();
        void processInput();
        void printMouseInfo();

        Object* demoObject;
        Renderer* renderer;
        Canvas* canvas;

        glm::vec2 prevMousePos;
        bool keys[1024];
        struct Mouse {
            float xpos;
            float ypos;
            bool leftPressed;
            bool rightPressed;
        } mouse;


};
