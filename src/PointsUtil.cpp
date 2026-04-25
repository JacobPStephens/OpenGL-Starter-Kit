#include "PointsUtil.h"

glm::ivec2 PointsUtil::getLeftPoint(glm::ivec2 p1, glm::ivec2 p2) {
    if (p1.x <= p2.x) {
        return p1;
    }
    return p2;
}
glm::ivec2 PointsUtil::getRightPoint(glm::ivec2 p1, glm::ivec2 p2) {
    if (p1.x > p2.x) {
        return p1;
    }
    return p2;
}
glm::ivec2 PointsUtil::getTopPoint(glm::ivec2 p1, glm::ivec2 p2) {
    if (p1.y <= p2.y) {
        return p1;
    }
    return p2;
}
glm::ivec2 PointsUtil::getBotPoint(glm::ivec2 p1, glm::ivec2 p2) {
    if (p1.y > p2.y) {
        return p1;
    }
    return p2;
}

glm::ivec2 PointsUtil::getDeltas(glm::ivec2 p1, glm::ivec2 p2) {
    int deltaX = p2.x-p1.x;
    int deltaY = p2.y-p1.y; 
    return glm::ivec2(deltaX, deltaY);
}

float PointsUtil::getSlope(glm::ivec2 p1, glm::ivec2 p2) {
    glm::ivec2 deltas = PointsUtil::getDeltas(p1, p2);
    float slope = (float)deltas.y/(float)deltas.x;
    return slope;
}