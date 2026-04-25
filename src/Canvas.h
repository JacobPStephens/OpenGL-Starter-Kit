#pragma once

#include <array>

#include <glm/glm.hpp>

#include "../util.h"

class Object;
struct Pixel;

class Canvas {
    public:
        Canvas();
        ~Canvas();
        void initValues();
        void modify(int x, int y);
        void interpolatePoints(glm::vec2 p1, glm::vec2 p2);

        void drawLine(glm::ivec2 p1, glm::ivec2 p2);
        void drawSteepLine(glm::ivec2 p1, glm::ivec2 p2);
        void drawShallowLine(glm::ivec2 p1, glm::ivec2 p2);
        void drawHorzLine(glm::ivec2 p1, glm::ivec2 p2);
        void drawVertLine(glm::ivec2 p1, glm::ivec2 p2);

        void updatePixelGrid(glm::ivec2 pos, int pixelValue);
        Object* createPixelObject(glm::ivec2 pos);
        void print(); 
        
        void initPixelGrid();
        bool firstTouch;
        std::array<std::array<Object*, util::scrWidth>, util::scrHeight> pixelGrid;

        std::array<std::array<int, util::scrWidth>, util::scrHeight> canvas;
        // std::array<std::array<Object*, util::scrWidth>, util::scrHeight> pixelObjects;
        
 
};