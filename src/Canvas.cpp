#include <iostream>
#include "Canvas.h"
#include "ResourceManager.h"
#include "Object.h"
#include "Pixel.h"
#include "PointsUtil.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

Canvas::Canvas() {
    initPixelGrid();
    glm::vec2 point1(0, 0);
    glm::vec2 point2(500, 249);
    glm::vec2 point3(0, 500);
    glm::vec2 point4(900, 465);
    drawShallowLine(point3, point4);
}

void Canvas::drawLine(glm::ivec2 p1, glm::ivec2 p2) {
    int deltaY = p2.y - p1.y;
    int deltaX = p2.x - p1.x;
    if (deltaX == 0) {
        drawVertLine(p1, p2);
        return;
    }
    if (deltaY == 0) {
        drawHorzLine(p1, p2);
        return;
    }
    float slope = (float)deltaY / (float)deltaX;
    if (slope <= 1.0f) {
        drawShallowLine(p1, p2);
    }
    else {
        drawSteepLine(p1, p2);
    }
}
void Canvas::drawHorzLine(glm::ivec2 p1, glm::ivec2 p2) {
    // printf("Drawing horizontal from %d %d to %d %d\n", p1.x, p1.y, p2.x, p2.y);
    int leftPtX = PointsUtil::getLeftPoint(p1, p2).x;
    int rightPtX = PointsUtil::getRightPoint(p1, p2).x;
    for (int i = leftPtX; i <= rightPtX; i++) {
        glm::ivec2 pos(i, p1.y);
        updatePixelGrid(pos, 1);
        // printf("updating pixel grid\n");
    }
    
}
void Canvas::drawVertLine(glm::ivec2 p1, glm::ivec2 p2) {
    assert(p1.x == p2.x);
    int topPtY = PointsUtil::getTopPoint(p1, p2).y;
    int botPtY = PointsUtil::getBotPoint(p1, p2).y;
    for (int i = topPtY; i <= botPtY; i++) {
        glm::ivec2 pos(p1.x, i);
        updatePixelGrid(pos, 1);
    }
}
void Canvas::drawShallowLine(glm::ivec2 p1, glm::ivec2 p2) {
    // printf("drawing shallow line with p1, p2 %d %d %d %d\n", p1.x, p1.y, p2.x, p2.y);
    float slope = PointsUtil::getSlope(p1, p2); 
    int slopeSign = util::getSign(slope);
    assert(slope != 0);
    glm::ivec2 smallerYPoint = PointsUtil::getTopPoint(p1, p2);
    glm::ivec2 largerYPoint = PointsUtil::getBotPoint(p1, p2);

    int prevTruncX = smallerYPoint.x;
    int truncX;
    double x = smallerYPoint.x; 
    for (int y = smallerYPoint.y; y <= largerYPoint.y; y++) {
        // printf("value of x: %lf", x);
        int truncX = (int)x;
        // printf("value of truncX: %d", truncX);
        updatePixelGrid(glm::ivec2(truncX, y), 1);
        drawHorzLine(glm::ivec2(prevTruncX+slopeSign, y), glm::ivec2(truncX-slopeSign, y));
        x += (1 / slope);
        prevTruncX = truncX;
    }

}
void Canvas::drawSteepLine(glm::ivec2 p1, glm::ivec2 p2) {
    // printf("drawing steep line with p1, p2 %d %d %d %d\n", p1.x, p1.y, p2.x, p2.y);
    float slope = PointsUtil::getSlope(p1, p2); 
    int slopeSign = util::getSign(slope);
    assert(slope != 0);
    glm::ivec2 smallerXPoint = PointsUtil::getLeftPoint(p1, p2);
    glm::ivec2 largerXPoint = PointsUtil::getRightPoint(p1, p2);

    int prevTruncY = smallerXPoint.y;
    int truncY;
    double y = smallerXPoint.y; 
    for (int x = smallerXPoint.x; x <= largerXPoint.x; x++) {
        // printf("value of y: %lf",y);
        int truncY = (int)y;
        // printf("value of truncY: %d", truncY);
        updatePixelGrid(glm::ivec2(x, truncY),  1);
        drawVertLine(glm::ivec2(x, prevTruncY+slopeSign), glm::ivec2(x, truncY-slopeSign));
        //drawHorzLine(glm::ivec2(prevTruncX+slopeSign, y), glm::ivec2(truncX-slopeSign, y));
        y += slope;
        prevTruncY = truncY;
    }
}
void Canvas::updatePixelGrid(glm::ivec2 pos, int value) {
    this->canvas[pos.y][pos.x] = 1;
    Object* px = new Object(
        glm::vec2(pos.x, pos.y),
        glm::vec2(1.0f),
        0.0f,
        ResourceManager::getTexture("px") 
    );
    // printf("created pixel at %d %d\n", pos.x, pos.y);
    this->pixelGrid[pos.y][pos.x] = px;
}

void Canvas::initPixelGrid() {
    // printf("Initializing pixel grid\n");
    for (int r = 0; r < this->pixelGrid.size(); r++) {
        this->canvas[r].fill(0);
    }
}   


Canvas::~Canvas() {
    for (int r = 0; r < this->pixelGrid.size(); r++) {
        for (int c = 0; c < this->pixelGrid[r].size(); c++) {
            if (this->canvas[r][c] != 0) {
                delete this->pixelGrid[r][c];
            }
        }
    }
}
//     Pixel* px = this->pixelGrid[pos.y][pos.x];
//     px->value = value;
    
//     if (px->object == nullptr) {
//         px->object = new Object(
//             glm::vec2(pos.x, pos.y),
//             glm::vec2(1.0f),
//             0.0f,
//             ResourceManager::getTexture("px") 

//         );
//         //px->object->color = glm::vec4(1.0f);
//     }
// }

// Object* Canvas::createPixelObject(glm::ivec2 pos) {
//     printf("creating px object %d %d\n", pos.x, pos.y);
//     return new Object(
//         glm::vec2(pos.x, pos.y),
//         glm::vec2(1.0f),
//         0.0f,
//         ResourceManager::getTexture("px") 
//     );

// }
//     int deltaY = p2.y - p1.y;
//     int deltaX = p2.x - p1.x;
//     if (deltaX == 0) {
//         drawVertLine(p1, p2);
//         return;
//     }
//     float slope = (float)deltaY / (float)deltaX;
    
//     if (slope >= 1.0f) {
//         drawSteepLine(p1, p2);
//     }
//     else {
//         drawShallowLine(p1, p2);
//     }
//     // " doesnt work with negative slope. \
//     // kind of works by change when abs(deltaX) > abs(deltaY) \
//     // but not exactly. (last y pixel might not get included) \    
//     // ";
//     // float slope = (float)deltaY / (float)deltaX;
//     // // printf("slope= %f\n", slope);

//     // int minX = std::min(p1.x, p2.x);
//     // int maxX = std::max(p1.x, p2.x);
//     // int minY = std::min(p1.y, p2.y);
//     // int maxY = std::max(p1.y, p2.y);
//     // // assert(slope > 0.0f);
//     // // assert(abs(deltaX) < abs(deltaY));

//     // float y;
//     // if (slope > 0) {
//     //     y = minY; 
//     // }
//     // else {
//     //     y = maxY;
//     // }
//     // int prevTruncY = minY; 
//     // int truncY; 
//     // for (int x = minX; x <= maxX; x++) {
//     //     int truncY = (int)y;
//     //     createPixel(glm::ivec2(x, truncY));
//     //     // printf("%f %d %d\n", y, prevTruncY, truncY);
//     //     drawVerticalLine(glm::ivec2(x, prevTruncY+1), glm::ivec2(x, truncY-1));

//     //     y += slope; 
//     //     prevTruncY = truncY;
        
//     //     // this->canvas[p1.y+i][p1.x+i] = 1; 
//     //     // createPixel(glm::ivec2(p1.x+i, p1.y+i));
//     // } 
//     // printf("%f\n", slope);
// }
// // void Canvas::drawSteepLine(glm::ivec2 p1, glm::ivec2 p2, float slope) {

// //     glm::ivec2 leftPt, rightPt;
// //     if (p1.x <= p2.x) {
// //         leftPt = p1;
// //         rightPt = p2;
// //     }
// //     else {
// //         leftPt = p2;
// //         rightPt = p1;
// //     }
// //     for (int x = leftPt.x; x <= rightPt.x; x++) {

// //     }
// // }
// void Canvas::drawShallowLine(glm::ivec2 p1, glm::ivec2 p2) {

// }
// void Canvas::createPixel(glm::ivec2 pos) {
//     // printf("pixel @ %d %d\n", pos.x, pos.y);
//     this->pixelObjects[pos.y][pos.x] = new Object(
//         glm::vec2(pos.x, pos.y),
//         glm::vec2(1.0f),
//         0.0f,
//         ResourceManager::getTexture("px")
//     );
//     this->pixelObjects[pos.y][pos.x]->color = glm::vec4(1.0f);
//     this->canvas[pos.y][pos.x] = 1;
//     // printf("Created pixel at pos %d %d\n", pos.x, pos.y); 
// }

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
// // }
// void Canvas::print() {
//     for (int r = 0; r < this->canvas.size(); r++) {
//         for (int c = 0; c < this->canvas[r].size(); c++) {
//             printf("%i ", this->canvas[r][c]);
//         }
//         printf("\n");
//     }
// }