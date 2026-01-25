#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <array>
#include "Direction.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Mode.h"

class PlayerObject : public GameObject {

    public:

        Direction CurrentDirection;
        Direction QueuedDirection;
        Direction Facing;
        int Row;
        int Col;
        float FreezeFrameTimer = 0.0f;
        float EatTimer = 0.0f;

        float ModeTimerReference = 0.0f;
        int ModeIdxReference = 0;
        bool isDead = false;
        bool hasWon = false;
        bool hasStarted = false;
        
        bool Stuck = false;

        GhostMode GhostModeReference;

        PlayerMode CurrentMode;
        PlayerMode PreviousMode;
        float HuntingTimer = 0.0f;
        PlayerObject();
        PlayerObject(glm::vec2 pos, glm::vec2 size, const float tileSize, float velocity, Texture2D& sprites, float cycleLength, std::string name);
        void HandleSwitchMode();
        std::array<float, 8> ScatterTimings;
        void EnterEatingState();
        void ExitEatingState();
        void Update(float dt, float screenWidth);
        glm::vec2 Move(float dt, float screenWidth);
        void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif 