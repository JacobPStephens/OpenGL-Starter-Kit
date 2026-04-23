#pragma once

#include <array>

#include <glm/glm.hpp>

#include "../util.h"

class Object;

class Canvas {
    public:
        Canvas();
        void initValues();
        void modify(int x, int y);
        void interpolatePoints(glm::vec2 p1, glm::vec2 p2);
        void drawLine(glm::ivec2 p1, glm::ivec2 p2);
        void drawVerticalLine(glm::ivec2 p1, glm::ivec2 p2);
        void createPixel(glm::ivec2 pos);
        void print(); 
        std::array<std::array<int, util::scrWidth>, util::scrHeight> canvas;
        std::array<std::array<Object*, util::scrWidth>, util::scrHeight> pixelObjects;
};