#pragma once
#include <glm/glm.hpp>
class PointsUtil {
    public:
        static glm::ivec2 getLeftPoint(glm::ivec2 p1, glm::ivec2 p2);
        static glm::ivec2 getRightPoint(glm::ivec2 p1, glm::ivec2 p2);
        static glm::ivec2 getTopPoint(glm::ivec2 p1, glm::ivec2 p2);
        static glm::ivec2 getBotPoint(glm::ivec2 p1, glm::ivec2 p2);
        static glm::ivec2 getDeltas(glm::ivec2 p1, glm::ivec2 p2);
        static float getSlope(glm::ivec2 p1, glm::ivec2 p2);
};