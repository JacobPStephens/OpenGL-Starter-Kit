#include <iostream>
#include "Canvas.h"
#include "Object.h"
#include "ResourceManager.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

Canvas::Canvas() {
    initValues();
}

void Canvas::modify(int x, int y) {
    this->canvas[y][x] = 1;
    createPixels();
}
void Canvas::initValues() {
    for (int r = 0; r < this->canvas.size(); r++) {
        this->canvas[r].fill(0);

    }
}

void Canvas::createPixels() {
    for (int r = 0; r < this->canvas.size(); r++) {
        for (int c = 0; c < this->canvas[r].size(); c++) {
            if (this->canvas[r][c] == 0) {
                continue;
            }
            this->pixelObjects[r][c] = new Object(
                glm::vec2(c, r),
                glm::vec2(1.0f),
                0.0f,
                ResourceManager::getTexture("px")        
            );
            this->pixelObjects[r][c]->color = glm::vec4(1.0f);
        }
    }
}
void Canvas::print() {
    for (int r = 0; r < this->canvas.size(); r++) {
        for (int c = 0; c < this->canvas[r].size(); c++) {
            printf("%i ", this->canvas[r][c]);
        }
        printf("\n");
    }
}