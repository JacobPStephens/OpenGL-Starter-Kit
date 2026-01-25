#ifndef GHOST_CLASS_H
#define GHOST_CLASS_H

#include "Direction.h"
#include "Mode.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "Eyes.h"
#include "ResourceManager.h"

#include <array>

class Ghost : public GameObject {

    public:

        Ghost(std::string name, PlayerObject* player, std::vector<std::vector<int>>* map, const float tileSize, Ghost* redGhost=nullptr);
        Ghost(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, std::vector<Texture2D> sprites, glm::vec3 color, float alpha, float rotation, std::string name, float tileSize, PlayerObject* player, std::vector<std::vector<int>> *map, GameObject* ghostMapTarget, GameObject* ghostAdjTarget);
        static glm::vec3 GetGhostRate(std::string name);
        //static std::array<float, 8> GetScatterTimings(int level);
        static std::map<int, int> FrightenedDurations;
        static int FrightenedDuration;
        // Mode switching
        GhostMode CurrentMode = SCATTER;
        GhostMode PreviousModeBeforeFrightened = SCATTER;
        static std::array<float, 8> ScatterTimings;
        int ModeIdx = 0;
        float ModeTimer;
        void SwitchScatterChase();
        float FrightenedTimer;
        float FreezeTimer = 0.0f;
        float FlashTimer = 0.0f;


        ~Ghost();
        glm::vec2 targetPos;
        float TILE_SIZE;

        std::vector<std::vector<int>>* Map;

        // int Row;

        float Threshold = 0.5f;
        // int Col;
        void HandleHomeIdle(float dt, Ghost* redGhost, PlayerObject* player);
        void HandleHomeEnter(PlayerObject* player, float dt);
        void HandleHomeExit(PlayerObject* player, float dt);
        void HandleFrightened(float dt, PlayerObject* player, Ghost* redGhost);
        void HandleEaten(float dt, PlayerObject* player, Ghost* redGhost);
        void HandleScatterChase(PlayerObject* player, float dt, Ghost* redGhost);
        float HomeIdleTimer = 0.0f;
        bool AbsoluteTargetReached(glm::vec2 ghostPos, glm::vec2 absoluteTarget, float threshold);
        void UpdateTargetAndDir(PlayerObject* player, Ghost* redGhost);
        float GetGhostVelocityAfterTunnel(std::string name);
        void SyncTimings(PlayerObject* player);
        bool InSpecialZone(int row, int col);
        static std::map<std::string, glm::vec2> ScatterTile;

        static int EXAMPLE_INT;

        GameObject* ghostMapTarget;
        GameObject* ghostAdjTarget;

        GameObject* DebugMapTarget;
        GameObject* DebugAdjTarget;

        Eyes* EyesObj;


        glm::vec2 AdjTargetPos;
        glm::vec2 MapTargetPos;
        bool InTunnel = false;
        bool IsTunnel(glm::vec2 target);


        Direction Dir = LEFT;
        void EnterFrightened(PlayerObject* player);
        void ExitFrightened(PlayerObject* player);
        bool HasBeenEatenThisDot = false;
        void EnterEaten();

        void Reset(PlayerObject* player);
        void Update(float deltaTime, PlayerObject* player, Ghost* redGhost=nullptr);
        void SnapToGrid(Direction direction, bool bothAxes);
        glm::vec2 Move(float deltaTime, glm::vec2 ghostPos, Direction dir, float speed);
        glm::vec2 GetMapTarget(int playerRow, int playerCol, Direction playerDir, std::string ghostType, GhostMode currentMode, Ghost* redGhost=nullptr);
        glm::vec2 GetAdjTarget(int row, int col, glm::vec2 ghostSize, Direction currDir, glm::vec2 mapTargetPos, bool isFrightened, float tileSize, std::vector<std::vector<int>>* map);
        Direction GetDirection(int row, int col, glm::vec2 adjTargetPos);

        float GetDistance(float x1, float y1, float x2, float y2);

        bool AdjTargetReached(glm::vec2 ghostPos, glm::vec2 adjTargetPos, float tileSize);

        void TurnAround(Direction currentDir, glm::vec2 currentAdjTarget);

        

};

#endif