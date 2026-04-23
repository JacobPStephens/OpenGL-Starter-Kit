#include <iostream>
#include "Canvas.h"
#include "Object.h"
#include "ResourceManager.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

Canvas::Canvas() {
    initValues();
    glm::vec2 point1(0, 0);
    glm::vec2 point2(50, 10);
    drawLine(point1, point2);
}

void Canvas::drawLine(glm::ivec2 p1, glm::ivec2 p2) {

    int deltaY = p2.y - p1.y;
    int deltaX = p2.x - p1.x;
    if (deltaX == 0) {
        drawVerticalLine(p1, p2);
        return;
    }
    " doesnt work with negative slope. \
    kind of works by change when abs(deltaX) > abs(deltaY) \
    but not exactly. (last y pixel might not get included) \    
    ";
    float slope = (float)deltaY / (float)deltaX;
    printf("slope= %f\n", slope);

    int minX = std::min(p1.x, p2.x);
    int maxX = std::max(p1.x, p2.x);
    int minY = std::min(p1.y, p2.y);
    // assert(slope > 0.0f);
    // assert(abs(deltaX) < abs(deltaY));

    float y = minY; 
    int prevTruncY = minY; 
    int truncY; 
    for (int x = minX; x <= maxX; x++) {
        int truncY = (int)y;
        createPixel(glm::ivec2(x, truncY));
        // printf("%f %d %d\n", y, prevTruncY, truncY);
        //drawVerticalLine(glm::ivec2(x, prevTruncY+1), glm::ivec2(x, truncY-1));

        y += slope; 
        prevTruncY = truncY;
        
        // this->canvas[p1.y+i][p1.x+i] = 1; 
        // createPixel(glm::ivec2(p1.x+i, p1.y+i));
    } 
    // printf("%f\n", slope);
}
void Canvas::drawVerticalLine(glm::ivec2 p1, glm::ivec2 p2) {
    for (int i = p1.y; i <= p2.y; i++) {
        createPixel(glm::ivec2(p1.x, i));
    }
}
void Canvas::createPixel(glm::ivec2 pos) {
    printf("pixel @ %d %d\n", pos.x, pos.y);
    this->pixelObjects[pos.y][pos.x] = new Object(
        glm::vec2(pos.x, pos.y),
        glm::vec2(1.0f),
        0.0f,
        ResourceManager::getTexture("px")
    );
    this->pixelObjects[pos.y][pos.x]->color = glm::vec4(1.0f);
    this->canvas[pos.y][pos.x] = 1;
    // printf("Created pixel at pos %d %d\n", pos.x, pos.y); 
}

// void Canvas::modify( InputState
    
//     glm::vec2 prevPoint, glm::vec2 currPoint) {

//     // this->canvas[y][x] = 1;
//     // interpolatePoints(this->prevPoint, point);
//     // this->prevPoint 
//     // createPixels();
// }

// void Canvas::interpolatePoints(glm::vec2 p1, glm::vec2 p2) {
//     int deltaY = p2.y - p1.y;
//     int deltaX = p2.x - p1.x;
//     if (deltaX == 0) {
//         return;
//     }
//     float slope = deltaY / deltaX;
//     printf("%f\n", slope);
// }

void Canvas::initValues() {
    for (int r = 0; r < this->canvas.size(); r++) {
        this->canvas[r].fill(0);
    }
}

// void Canvas::createPixels() {
//     for (int r = 0; r < this->canvas.size(); r++) {
//         for (int c = 0; c < this->canvas[r].size(); c++) {
//             if (this->canvas[r][c] == 0) {
//                 continue;
//             }
//             this->pixelObjects[r][c] = new Object(
//                 glm::vec2(c, r),
//                 glm::vec2(1.0f),
//                 0.0f,
//                 ResourceManager::getTexture("px")        
//             );
//             this->pixelObjects[r][c]->color = glm::vec4(1.0f);
//         }
//     }
// }
void Canvas::print() {
    for (int r = 0; r < this->canvas.size(); r++) {
        for (int c = 0; c < this->canvas[r].size(); c++) {
            printf("%i ", this->canvas[r][c]);
        }
        printf("\n");
    }
}