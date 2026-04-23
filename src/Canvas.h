#pragma once

#include <array>
#include "../util.h"

class Object;

class Canvas {
    public:
        Canvas();
        void initValues();
        void modify(int x, int y);
        void createPixels();
        void print(); 
        std::array<std::array<int, util::scrWidth>, util::scrHeight> canvas;
        std::array<std::array<Object*, util::scrWidth>, util::scrHeight> pixelObjects;
};