#pragma once

class Object;
class Renderer;

class App {
    public:

        App(unsigned int scrWidth, unsigned int scrHeight);
        ~App();

        void initialize();
        void createDemoObject();
        void setupShaders();

        void update(float dt);
        void render();
        void processInput();

        unsigned int scrWidth, scrHeight;

        Object* demoObject;
        Renderer* renderer;

        bool keys[1024];
        struct Mouse {
            float xpos;
            float ypos;
            bool leftPressed;
            bool rightPressed;
        } mouse;

        void printMouseInfo();

};
