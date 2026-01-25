#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <vector>
#include <map>
#include <tuple>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "PlayerObject.h"
#include "Direction.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;
class Game {

    public:

        GameState State;
        GameObject* DebugMapTarget;
        GameObject* DebugLine;

        unsigned int Width, Height, BorderHeight;
        bool Keys[1024];

        int playerEatStreak = 1;
        float timeSinceDeath = 0.0f;
        float timeSinceWin = 0.0f;
        float timeSinceStart = 0.0f;
        bool GameOver = false;
        Game(unsigned int width, unsigned int height, unsigned int borderHeight);
        ~Game();

        // initialize shaders, textures, levels
        void Init();

        void BuildWalls();
        void BuildDots();
        void BuildPowerDots();
        void BuildTiles();

        std::vector<std::vector<int>>* BuildMap();

        std::vector<std::string> Split(std::string);
        void PrintVector(std::vector<std::string>);
        
        void IncreaseScore(int points);
        void SpawnEatDisplay(glm::vec2 ghostPos, int playerEatStreak);
        void SpawnFruit();
        
        void ResetGame();
        void ResetCharacters();
        void ResetDots();
        // core game loop
        void ProcessInput(float dt);
        void Update(float dt);

        GameObject* GhostMapTarget;

        std::tuple<glm::vec2, glm::vec2> AssembleWallInfo(float x1, float y1, float x2, float y2);
        void HandleGhostCollisions();
        void HandleDeathSequence(float deltaTime);
        void HandleWinSequence(float deltaTime);
        void HandleStartSequence(float deltaTime);
        void FreezeAllGhosts(float duration, std::string ghostNameException);
        void HandleCollisions();
        void HandleDotCollisions();
        void HandleDotCheckpoints();
        void HandleFruitCollision();
        void HandlePowerDotCollisions();
        void ActivatePowerMode();
        void KillPlayer();

        bool CheckPlayerWallCollision(Direction direction);
        void SnapPlayerToGrid(Direction direction, bool bothAxes);
        bool CheckCollision(glm::vec2 onePos, glm::vec2 oneSize, glm::vec2 twoPos, glm::vec2 twoSize);
        void Render();
        void LoadAllTextures();

        void CreateHighScoreLetters();
        void CreateHighScoreNumbers();
        void CreateScoreNumbers();
        void CreateGameOver();
        void CreateLives(int numLives);
        void CreateReady();
        void Create1Up();
        void DeleteReady();


        int FrameCount = 0;
        float FrameTimer = 0.0f;
        int framesSum;
        int numFrames;

        std::vector<int> Frames;
        void PrintFPS(float dt);
};

#endif