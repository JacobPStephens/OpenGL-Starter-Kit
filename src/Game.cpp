#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "Ghost.h"
#include "Eyes.h"

#include "LevelSpecs.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>

#include <filesystem>

SpriteRenderer* Renderer;
std::queue<float> fpsQueue;
float queueSum;

const glm::vec2 PLAYER_SIZE(13.0f/8.0f * TILE_SIZE);
const glm::vec2 GHOST_SIZE(14.0f/8.0f * TILE_SIZE);
//const float PLAYER_VELOCITY(53.0f/8.0f * TILE_SIZE);
const float PLAYER_VELOCITY = GetSpec("playerSpeed", 1) * speedRatio * TILE_SIZE;

const float BOTTOM_BORDER_HEIGHT = 48.0f;// 48.0f;

const int MIN_Y_TILE = 0;
const int MAX_Y_TILE = 31;

const float MAZE_CEILING = TOP_BORDER_HEIGHT;
const float MAZE_FLOOR = TOP_BORDER_HEIGHT + (248.0f * RESOLUTION_SCALE - TOP_BORDER_HEIGHT - BOTTOM_BORDER_HEIGHT);
// int remainingDots = 240 + 4;
int score = 0;
int highscore = 0;
int extraLivesGranted = 0;

// 0.129, 0.129, 1.0

// 256.074 RED GHOST SPAWN Y
PlayerObject* Player;
//Ghost* NewGhost;

std::vector<Ghost*> Ghosts;
// Ghost* RedGhost;
// Eyes* RedGhostEyes;
// GameObject* RedGhostMapTarget;
// GameObject* RedGhostAdjTarget;
// Ghost* PinkGhost;
// Eyes* PinkGhostEyes;
// GameObject* PinkGhostMapTarget;
// GameObject* PinkGhostAdjTarget;

//GameObject* Wall;
//GameObject* Wall2;
GameObject* Background;
GameObject* CurrentSensor;
GameObject* QueuedSensor;
std::vector<GameObject*> walls;
//std::vector<GameObject*> dots;
std::array<GameObject*, 240> dots;

std::array<GameObject*, 4> powerDots;
//std::vector<GameObject*> highScoreLetters;
std::array<GameObject*, 9> highScoreLetters;
std::array<GameObject*, 7> scoreNumbers;
std::array<GameObject*, 7> highScoreNumbers;
std::vector<GameObject*> extraLives;
std::array<GameObject*, 6> readyLetters;
std::array<GameObject*, 8> gameOverLetters;
//std::vector<GameObject*> readyLetters;
std::vector<GameObject*> oneUpLetters;

std::vector<Texture2D*> digitTextures;
std::vector<Texture2D> deathTextures;
std::vector<Texture2D*> deathSprites;
//std::vector<Texture2D&> playerSprites;

GameObject* eatDisplayObject;
GameObject* fruit;

GameObject* MapTiles;
std::vector<std::vector<int>>* map;

Game::Game(unsigned int width, unsigned int height, unsigned int borderHeight) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), BorderHeight(borderHeight)
{

}

Game::~Game() {
    delete Renderer;
    delete Player;
    delete map;
    // delete RedGhost;
    // delete RedGhostEyes;
    // delete RedGhostMapTarget;
    // delete RedGhostAdjTarget;
    // delete PinkGhost;
    // delete PinkGhostEyes;
    // delete PinkGhostMapTarget;
    // delete PinkGhostAdjTarget;
    //delete NewGhost;

    for (Ghost* ghost : Ghosts) {
        delete ghost;
    }

    for (GameObject*& letter : highScoreLetters) {
        delete letter;
    }
    for (GameObject*& number : scoreNumbers) {
        delete number;
    }
    for (GameObject*& number : highScoreNumbers) {
        delete number;
    }

    // delete remaining extra lives
    for (GameObject*& life : extraLives) {
        delete life;
    }
    // delete ready letters if exited early
    for (GameObject*& readyLetter : readyLetters) {
        if (readyLetter){
            delete readyLetter;
        }
    }  

    for (GameObject*& gameOverLetter : gameOverLetters) {
        if (gameOverLetter) {
            delete gameOverLetter;
        }
    }
    
    for (GameObject*& oneUpLetter : oneUpLetters) {
        delete oneUpLetter;
    }

    if (eatDisplayObject) {
        delete eatDisplayObject;
    }

    if (fruit) {
        delete fruit;
    }
    //delete Wall;
    //delete Wall2;
    delete CurrentSensor;
    delete QueuedSensor;
    delete Background;

    for (int i = 0; i < digitTextures.size(); i++) {
        delete digitTextures[i];
    }
    // for (int i = 0; i < deathTextures.size(); i++) {
    //     delete deathTextures[i];
    // }
    // delete all wall ptrs
    for (int i = 0; i < walls.size(); i++) {
        delete walls[i];
    }
    for (int i = 0; i < dots.size(); i++) {
        delete dots[i];
    }  
    for (int i = 0; i < powerDots.size(); i++) {
        delete powerDots[i];
    }      
    delete MapTiles;
}


// const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// const float BALL_RADIUS = 12.5f;
// BallObject *Ball;

std::vector<std::vector<int>>* Game::BuildMap() {
    std::ifstream mapInput;
    std::string line;
    
    mapInput.open("./util/map.txt");
    if (!mapInput.is_open())
        std::cout <<"MAP FILE DID NOT OPEN" << std::endl;

    float halfTile = TILE_SIZE / 2.0f;
    float halfDot = (TILE_SIZE / 4.0f) / 2.0f;

    map = new std::vector<std::vector<int>>();
    
    while (std::getline (mapInput, line)) {
        std::vector<int> rowInt;
        std::vector<std::string> rowStr = Split(line);

        //PrintVector(rowStr);
        for (int i = 0; i < rowStr.size(); i++) {
            //std::cout << rowStr[i] << std::endl;
            //std::cout << rowStr[i].size() << std::endl;

            if (rowStr[i] == "") {
                std::cout <<" Empty string" << std::endl;
            }
            // if (rowStr[i][0] == '\r') {
            //     std::cout << "new line detected";
            // }

            rowInt.push_back(std::stoi(rowStr[i]));
        }  

        map->push_back(rowInt);
        



        //PrintVector(coordinates);
        //glm::vec2 tilePos(std::stoi(coordinates[1]) * TILE_SIZE, std::stoi(coordinates[0]) * TILE_SIZE);
        //std::cout << "Tile Position " << tilePos.x << " " << tilePos.y << std::endl;
        //glm::vec2 dotPos(tilePos.x + halfTile - halfDot, tilePos.y + halfTile - halfDot);
        //std::cout << "Dot Position " << dotPos.x << " " << dotPos.y << std::endl;

    }
    mapInput.close();
    return map;
}
void Game::BuildDots() {


    //assert (dots.empty());
    std::ifstream dotInput;
    std::string line;
    
    dotInput.open("./util/dots.txt");
    if (!dotInput.is_open())
        std::cout <<"DOT FILE DID NOT OPEN" << std::endl;


    // for (int i = 0; i < ans.size(); i++) {
    //     std::cout<<ans[i]<<std::endl;
    // }
    float halfTile = TILE_SIZE / 2.0f;
    float halfDot = (TILE_SIZE / 4.0f) / 2.0f;
    //std::vector<Texture2D> pxSprite = { ResourceManager::GetTexture("px")};
    Texture2D& pxSprite = ResourceManager::GetTexture("px");
    int i = 0;
    while (std::getline (dotInput, line)) {
        std::vector<std::string> coordinates = Split(line);
        //PrintVector(coordinates);

        /*
        float yTile = std::stoi(coordinates[0]);
        double normalizedYTile = (yTile-MIN_Y_TILE) / (MAX_Y_TILE - MIN_Y_TILE);
        
        float tileYPos = MAZE_CEILING + (MAZE_FLOOR - MAZE_CEILING) * normalizedYTile - 2.0f;
        glm::vec2 tilePos(std::stoi(coordinates[1]) * TILE_SIZE, tileYPos);*/
        glm::vec2 tilePos(std::stoi(coordinates[1]) * TILE_SIZE, std::stoi(coordinates[0]) * TILE_SIZE);
        //std::cout << "Tile Position " << tilePos.x << " " << tilePos.y << std::endl;
        glm::vec2 dotPos(tilePos.x + halfTile - halfDot, tilePos.y + halfTile-halfDot + TOP_BORDER_HEIGHT);
        //std::cout << "Dot Position " << dotPos.x << " " << dotPos.y << std::endl;
        GameObject* d = new GameObject(dotPos, glm::vec2(TILE_SIZE / 4.0f), TILE_SIZE, pxSprite);
        d->Destroyed = false;
        //dots.push_back(d);
        dots[i] = d;

        i++;

    }
    
    dotInput.close();
}

void Game::BuildPowerDots() {
    std::cout<<"Building dots..."<<std::endl;
    std::ifstream powerDotInput;
    std::string line;
    
    powerDotInput.open("./util/powerDots.txt");
    if (!powerDotInput.is_open())
        std::cout <<"DOT FILE DID NOT OPEN" << std::endl;


    // for (int i = 0; i < ans.size(); i++) {
    //     std::cout<<ans[i]<<std::endl;
    // }
    float halfTile = TILE_SIZE / 2.0f;
    float halfDot = (TILE_SIZE / 4.0f) / 2.0f;
    //std::vector<Texture2D> pxSprite = { ResourceManager::GetTexture("powerDot")};
    Texture2D& pxSprite = ResourceManager::GetTexture("powerDot");
    int i = 0;
    while (std::getline (powerDotInput, line)) {
        std::vector<std::string> coordinates = Split(line);
        //PrintVector(coordinates);
        glm::vec2 tilePos(std::stoi(coordinates[1]) * TILE_SIZE, std::stoi(coordinates[0]) * TILE_SIZE + TOP_BORDER_HEIGHT);
        //std::cout << "Tile Position " << tilePos.x << " " << tilePos.y << std::endl;
        glm::vec2 dotPos(tilePos.x, tilePos.y);
        //std::cout << "Dot Position " << dotPos.x << " " << dotPos.y << std::endl;
        GameObject* d = new GameObject(dotPos, glm::vec2(TILE_SIZE), TILE_SIZE, pxSprite);
        d->Destroyed = false;
        //powerDots.push_back(d);
        powerDots[i] = d;

        i++;

    }
    powerDotInput.close();
}
void Game::BuildWalls() {
    //std::vector<Texture2D> grayWallSprites = { ResourceManager::GetTexture("px")};
    Texture2D& grayWallSprites = ResourceManager::GetTexture("px");
    std::string line;
    std::ifstream wallInput;

    // read file into 2d vector of walls
    std::vector<std::vector<int>> wallTiles;
    std::vector<int> wall;
    
    wallInput.open("./util/wallsOut.txt");
    if (!wallInput.is_open())
        std::cout <<"WALL FILE DID NOT OPEN" << std::endl;


    while (std::getline (wallInput, line)) {
        std::string num;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != ' ' && line[i] != '\n')
                num += line[i];
            else {
                wall.push_back(std::stoi(num));
                num = "";
            }
        }
        wall.push_back(std::stoi(num));
        wallTiles.push_back(wall);
        wall.clear();
    }
    wallInput.close();

    // construct all walls in 2D vector
    for (int i = 0; i < wallTiles.size(); i++) {

        /*float yTile = wallTiles[i][1];
        
        float normalizedYTile = (yTile-MIN_Y_TILE) / (MAX_Y_TILE - MIN_Y_TILE);
        float yTilePos = MAZE_CEILING + (MAZE_FLOOR - MAZE_CEILING) * normalizedYTile;
        std::cout << yTile << "normalized: " << normalizedYTile << std::endl;
        glm::vec2 pos(wallTiles[i][0] * TILE_SIZE, yTilePos);*/

        glm::vec2 pos(wallTiles[i][0] * TILE_SIZE, wallTiles[i][1] * TILE_SIZE + TOP_BORDER_HEIGHT);
        //glm::vec2 size(wallTiles[i][2] * TILE_SIZE, wallTiles[i][3] * TILE_SIZE * (616.0f/744.0f));
        glm::vec2 size(wallTiles[i][2] * TILE_SIZE, wallTiles[i][3] * TILE_SIZE);

        //Wall* w = new Wall(pos, size, grayWallSprites, i);
        GameObject* w = new GameObject(pos, size, TILE_SIZE, grayWallSprites);
        walls.push_back(w);
    }
}

std::vector<std::string> Game::Split(std::string s) {
    std::vector<std::string> res;
    std::string part;
    for (int i = 0; i < s.size(); i++) {

        if (!std::isspace(s[i])) {
            part += s[i];
        }
        else {
            if (part != "") {
                res.push_back(part);
            }
            part = "";
        }
    }
    if (part != "" && !std::isspace(part[0])) {
        res.push_back(part);
    }
    return res;

}
void Game::PrintVector(std::vector<std::string> v) {

    std::cout << "<";
    for (int i = 0; i < v.size(); i++) {
        if (i != v.size()-1) {
            std::cout << v[i] << ", ";
        }
        else {
            std::cout << v[i];
        }
    }
    std::cout<<">\n";
}

void Game::ResetGame() {
    std::cout << " = = = = = RESETTING GAME = = = = = " << std::endl;
    score = 0;
    for (int i = 0; i < scoreNumbers.size(); i++) {
        scoreNumbers[i]->Sprite = ResourceManager::GetTexture("0");
    }

    CurrentLevel = 1;
    playerEatStreak = 1;
    timeSinceDeath = 0.0f;
    timeSinceWin = 0.0f;
    timeSinceStart = 0.0f;
    GameOver = false;
    Player->hasWon = false;
    timeSinceWin = 0.0f;
    
    extraLivesGranted = 0;
    ResetCharacters();
    ResetDots();

}
void Game::ResetCharacters() {
    std::cout << "SOFT RESET!" << std::endl;
    std::cout << " curr level : " << CurrentLevel << std::endl;
    fruit = nullptr;

    // create player
    // glm::vec2 startingTileCenter = glm::vec2(13.0f * TILE_SIZE + TILE_SIZE/2.0f, 23.0f * TILE_SIZE + TILE_SIZE/2.0f + TOP_BORDER_HEIGHT);
    glm::vec2 startingTile = glm::vec2(13.0f * TILE_SIZE, 23.0f * TILE_SIZE + TOP_BORDER_HEIGHT);
    glm::vec2 playerPos = glm::vec2(startingTile.x+TILE_SIZE-PLAYER_SIZE.x/2.0f, startingTile.y+TILE_SIZE/2.0f-PLAYER_SIZE.y/2.0f);
    // glm::vec2 startingTileCenter = glm::vec2(13.0f * TILE_SIZE + TILE_SIZE/2.0f, 23.0f * TILE_SIZE + TILE_SIZE/2.0f + TOP_BORDER_HEIGHT);
    // glm::vec2 playerPos = glm::vec2(startingTileCenter.x - TILE_SIZE/2.0f,  startingTileCenter.y-PLAYER_SIZE.y/2.0f);
    
    // I never delete these pointers. I think this makes them dangling pointers
    std::vector<Texture2D*> playerSprites = {
        &ResourceManager::GetTexture("pacman1"),
        &ResourceManager::GetTexture("pacman3"),
    };


    CreateReady();

    // create or reset player
    //// first life 
    if (!Player) {
        Player = new PlayerObject(playerPos, PLAYER_SIZE, TILE_SIZE, PLAYER_VELOCITY, ResourceManager::GetTexture("pacman1"), 0.25f, std::string("player"));
        Player->AnimationSprites = playerSprites;
    }
    //// all other lives
    else {
        std::cout << "setting player position in reset charactesrs" << std::endl;
        // set object properties
        Player->Position = playerPos;
        Player->Rotation = 180.0f;
        Player->isDead = false;
        Player->hasStarted = false;
        Player->AnimationSprites = playerSprites;
        Player->CycleLength = 0.25f;
        Player->Velocity = GetSpec("playerSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
        
        // scatter chase timings
        Player->ScatterTimings = GetScatterTimings(CurrentLevel);
        Player->CurrentMode = NORMAL;
        Player->HuntingTimer = 0.0f;
        Player->ModeIdxReference = 0;
        Player->ModeTimerReference = Player->ScatterTimings[0];
        Player->GhostModeReference = SCATTER;

        // special case timers
        timeSinceDeath = 0.0f;
        timeSinceStart = 0.0f;

        

    }
    Player->QueuedDirection = LEFT;
    // create or reset ghosts
    //// first life
    if (Ghosts.size() == 0) {


        std::array<std::string, 4> ghostColors = { "red", "pink", "blue", "orange"};

        for (std::string c : ghostColors) {

            Ghosts.push_back(new Ghost(c , Player, map, TILE_SIZE));
        }
    }
    //// all other lives
    else {
        for (Ghost* g : Ghosts) {
            g->Reset(Player);
        }
    }
    


}

void Game::ResetDots() {

    // delete existing dots if any
    for (GameObject*& d: dots) {
        delete d;
    }
    for (GameObject*& pd: powerDots) {
        delete pd;
    }



    remainingDots = 240 + 4;
    BuildDots();

    std::cout << "dots size: " << dots.size() << std::endl;
    BuildPowerDots();

}
void Game::Init() {

    LoadAllTextures();

    std::array<std::string, 10> digits = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for (int i = 0; i < digits.size(); i++) {
        digitTextures.push_back(&ResourceManager::GetTexture(digits[i].c_str()));
    }
    // these 2 do the same thing (remove please!)
    // for (int i = 1; i <= 10; i++) {
    //     std::string deathTexName = "death" + std::to_string(i);
    //     deathTextures.push_back(ResourceManager::GetTexture(deathTexName));
    // }

    for (int i = 1; i <= 10; i++) {
        std::string s = "death" + std::to_string(i);;
        std::cout << "s=" << s << std::endl;
        deathSprites.push_back(&ResourceManager::GetTexture(s));
    }

    // create player display
    CreateHighScoreLetters();
    CreateHighScoreNumbers();
    CreateScoreNumbers();

    CreateLives(2);
    Create1Up();

    // build map, walls, and dots
    //std::vector<Texture2D> wallSprites = { ResourceManager::GetTexture("map224x248")};
    //std::vector<Texture2D> mapTilesSprite = { ResourceManager::GetTexture("mapTiles")};

    Texture2D& wallSprites = ResourceManager::GetTexture("map224x248");
    Texture2D& mapTilesSprite = ResourceManager::GetTexture("mapTiles");
    glm::vec2 mapSize = glm::vec2(224.0f * RESOLUTION_SCALE, 248.0f * RESOLUTION_SCALE);// - TOP_BORDER_HEIGHT - BOTTOM_BORDER_HEIGHT);// - TOP_BORDER_HEIGHT);
    glm::vec2 mapPos = glm::vec2(this->Width/2.0f - mapSize.x/2.0f, TOP_BORDER_HEIGHT);//TOP_BORDER_HEIGHT);
    Background = new GameObject(mapPos, mapSize, TILE_SIZE, wallSprites, 1.0f, glm::vec3(1.0f), 1.0f, 0.0f, std::string("wall"));
    MapTiles = new GameObject(mapPos, mapSize, TILE_SIZE, mapTilesSprite, 1.0f, glm::vec3(1.0f), 0.25f, 0.0f, std::string("wall"));
    BuildWalls();
    BuildPowerDots();
    BuildDots();
    map = BuildMap();

    // create player sensors
    Texture2D& pxSprite = ResourceManager::GetTexture("px");
    CurrentSensor = new GameObject(glm::vec2(0.0f), glm::vec2(1.0f), TILE_SIZE, pxSprite, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f, "upSensor");
    QueuedSensor = new GameObject(glm::vec2(0.0f), glm::vec2(1.0f), TILE_SIZE, pxSprite, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f), 1.0f, 0.0f, "upSensor");


    // creates players and ghosts
    ResetCharacters();
    
    
    // open gl setup
    //// configure shaders
    ResourceManager::LoadShader("shaders/default.vert", "shaders/default.frag", nullptr, "sprite"); 
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height+this->BorderHeight), 0.0f, -1.0f, 1.0f);
    //// update uniform variables in shader
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
    //// create renderer
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

}

std::tuple<glm::vec2, glm::vec2> Game::AssembleWallInfo(float x1, float y1, float x2, float y2) {
    float wallWidth = 4.0f;
    bool isHorizontal = (y1 == y2);
    glm::vec2 size;
    if (isHorizontal) {
        size = glm::vec2(x2-x1,wallWidth);
    }
    else {
        size = glm::vec2(wallWidth, y2-y1);
    }
    glm::vec2 pos(x1, y1);

    return std::make_tuple(pos, size);


}

void Game::CreateHighScoreLetters() {
    // Draw high score letters
    float digitSize = 7.0f * RESOLUTION_SCALE;
    float letterPosX = 9.0f * TILE_SIZE;
    float letterPosY = 0.0f * TILE_SIZE + TILE_SIZE/4.0f;
    std::array<std::string, 9> letters = {"h", "i", "g", "h", "s", "c", "o", "r", "e"};

    
    float diff = 0;
    for (int i = 0; i < letters.size(); i++)  {
        std::string s = "textures/emulogicFont/" + letters[i] + ".png";
        Texture2D& letterSprite = ResourceManager::GetTexture(letters[i].c_str());
        GameObject* g = new GameObject(glm::vec2(letterPosX + diff, letterPosY), glm::vec2(digitSize), TILE_SIZE, letterSprite);
        //highScoreLetters.push_back(g);
        highScoreLetters[i] = g;
        diff += TILE_SIZE;
        // extra tile space between "high" and "score"
        if (i == 3) {
            diff += TILE_SIZE;
        }
    }
}

void Game::CreateHighScoreNumbers() {
    float digitSize = 7.0f * RESOLUTION_SCALE;
    float numberPosX = 10.0f * TILE_SIZE;
    float numberPosY = 1.0f * TILE_SIZE + TILE_SIZE/4.0f;

    float diff = 0;
    Texture2D& zeroSprite = ResourceManager::GetTexture("0");
    for (int i = 0; i < 7; i++) {
        GameObject* g = new GameObject(glm::vec2(numberPosX + diff, numberPosY), glm::vec2(digitSize), TILE_SIZE, zeroSprite);
        //highScoreNumbers.push_back(g);
        highScoreNumbers[i] = g;
        diff += TILE_SIZE;
    }
}
void Game::CreateScoreNumbers() {
    float digitSize = 7.0f * RESOLUTION_SCALE;
    float numberPosX = 1.0f * TILE_SIZE;
    float numberPosY = 1.0f * TILE_SIZE + TILE_SIZE/4.0f;
    Texture2D& zeroSprite = ResourceManager::GetTexture("0");
    float diff = 0;
    for (int i = 0; i < 7; i++) {
        
        GameObject* g = new GameObject(glm::vec2(numberPosX + diff, numberPosY), glm::vec2(digitSize), TILE_SIZE, zeroSprite);
        //scoreNumbers.push_back(g);
        scoreNumbers[i] = g;
        diff += TILE_SIZE;
    }

}

void Game::CreateGameOver() {
    std::vector<std::string> letters = {"g", "a", "m", "e", "o", "v", "e", "r"};
    float readyPosX = 9.0f * TILE_SIZE;
    float readyPosY = 20.0f * TILE_SIZE;
    float diff = 0.0f;
    
    for (int i = 0; i < letters.size(); i++) {
        
        std::string l = letters[i];
        
        Texture2D& tex = ResourceManager::GetTexture(l);
        GameObject* g = new GameObject(glm::vec2(readyPosX + diff, readyPosY), glm::vec2(TILE_SIZE), TILE_SIZE, tex);
        g->Color = glm::vec3(1.0f, 0.0f, 0.0f);
        
        gameOverLetters[i] = g;

        diff += TILE_SIZE * 1.1f;
        // extra tile space between "game" and "over"
        if (i == 3) {
            diff += TILE_SIZE;
        }
    }

}

void Game::CreateLives(int numLives) {

    for (GameObject*& life : extraLives) {
        delete life;
        extraLives.pop_back();
    }

    float diff = 0;
    float livesPosX = 2.0f * TILE_SIZE;
    float livesPosY = 31.0f * TILE_SIZE + TOP_BORDER_HEIGHT + 5.0f;
    Texture2D& lifeSprite = ResourceManager::GetTexture("pacman1");

    for (int i = 0; i < numLives; i++ ) {
        GameObject* g = new GameObject(glm::vec2(livesPosX + diff, livesPosY), glm::vec2(PLAYER_SIZE), TILE_SIZE, lifeSprite);
        g->Rotation = 180.0f;
        extraLives.push_back(g);
        diff += 2.0f * TILE_SIZE; 
    }
}
void Game::CreateReady() {

    std::vector<std::string> letters = {"r", "e", "a", "d", "y", "!"};
    float readyPosX = 11.0f * TILE_SIZE;
    float readyPosY = 20.0f * TILE_SIZE;
    float diff = 0.0f;
    
    for (int i = 0; i < letters.size(); i++) {
        
        std::string l = letters[i];
        
        Texture2D& tex = ResourceManager::GetTexture(l);
        GameObject* g = new GameObject(glm::vec2(readyPosX + diff, readyPosY), glm::vec2(TILE_SIZE), TILE_SIZE, tex);
        g->Color = glm::vec3(1.0f, 1.0f, 0.0f);

        readyLetters[i] = g;
        //readyLetters.push_back(g);
        diff += TILE_SIZE * 1.1f;

    }


}

void Game::Create1Up() {
    std::vector<std::string> letters = {"1", "u", "p"};
    float digitSize = 7.0f * RESOLUTION_SCALE;
    float letterPosX = 1.0f * TILE_SIZE;
    float letterPosY = 0.0f * TILE_SIZE + TILE_SIZE/4.0f;
    float diff = 0.0f;

    for (std::string l : letters) {
        
        Texture2D& tex = ResourceManager::GetTexture(l);
        GameObject* g = new GameObject(glm::vec2(letterPosX + diff, letterPosY), glm::vec2(digitSize), TILE_SIZE, tex);
        oneUpLetters.push_back(g);
        diff += TILE_SIZE * 1.1f;

    }
}

void Game::DeleteReady() {

    for (int i = 0; i < readyLetters.size(); i++) {
        delete readyLetters[i];
        readyLetters[i] = nullptr;
    }
    
}

void Game::Update(float dt) {
    //std::cout << "laggginggggingingngiginging" << std::endl;
    ProcessInput(dt);
    PrintFPS(dt);
    if (GameOver && this->Keys[GLFW_KEY_ENTER]) {
        ResetGame();
        this->Keys[GLFW_KEY_ENTER] = false;
    }
    //std::cout << "Player hunting timer: " << Player->HuntingTimer << std::endl;

    //std::cout << "sensorY - playerY = " << CurrentSensor->Position.y - Player->Position.y << std::endl;
    //std::cout << "player y pos : " << Player->Position.y << std::endl;

    //std::cout << "time since win: " << timeSinceWin << std::endl;
    //std::cout << " player speed: " << Player->Velocity << std::endl;
    // std::cout << "time since start" << timeSinceStart << std::endl;
    // std::cout << "player has started" << Player->hasStarted << std::endl;
    // std::cout << "player has won" << Player->hasWon << std::endl;
    // std::cout << "player is dead" << Player->isDead << std::endl;
    //std::cout << "time since death: " << timeSinceDeath << std::endl;

    // std::cout << "Speed ratio: " << std::endl;
    // std::cout << "TILE size: " << TILE_SIZE << std::endl;
    // std::cout << "player speed ratio" << LevelSpecs["playerSpeed"][1] << std::endl;
    // std::cout << "Total speed: " << PLAYER_VELOCITY << std::endl;
    
    
    if (!Player->hasStarted) {
        HandleStartSequence(dt);
    }

    if (Player->hasWon) {
        HandleWinSequence(dt);
        return;
    }

    if (Player->isDead) {
        HandleDeathSequence(dt);
    }
    
    


    
    
    for (Ghost* ghost : Ghosts) {

        if (ghost->Name == "blueGhost" || ghost->Name == "orangeGhost") {
            ghost->Update(dt, Player, Ghosts.at(0));
        }
        else {
            ghost->Update(dt, Player);
        }


    }

    if (!Player->hasStarted) {
        return;
    }

    Player->Update(dt, 224.0f * RESOLUTION_SCALE);

    if (fruit) {
        fruit->Update(dt);
        if (fruit->Lifespan >= fruit->DeathTimer) {
            std::cout << " Fruit destroyed naturally after " << fruit->DeathTimer << " seconds." << std::endl;
            fruit = nullptr;

        }
    }

    if (eatDisplayObject) {
        eatDisplayObject->Update(dt);

        if (eatDisplayObject->Lifespan > 1.0f) {
            eatDisplayObject = nullptr; // memory leak ? 

        }
    }

    //NewGhost->Update(dt, Player);
    // RedGhost->Update(dt, Player);
    // RedGhostEyes->Update(dt);
    // PinkGhost->Update(dt, Player);
    // PinkGhostEyes->Update(dt);

    //std::cout << "Ghost: " << RedGhost->Position.x << " " << RedGhost->Position.y << std::endl;
    //std::cout << CurrentSensor->Position.x << " " << CurrentSensor->Position.y << std::endl;
    //std::cout << TILE_SIZE * << std::endl;

    //Wall->Update(dt);
    if (!Player->isDead) { // fix jank and update player death logic
        HandleCollisions();
    }

    //Player->Move(dt);


}

void Game::HandleStartSequence(float deltaTime) {
    timeSinceStart += deltaTime;

    if (timeSinceStart >= 2.0f) {
        Player->hasStarted = true;
        timeSinceStart = 0.0f;
        DeleteReady();
    }
}

void Game::HandleWinSequence(float deltaTime) {
    timeSinceWin += deltaTime;

    if (timeSinceWin >= 2.0f) {
        CurrentLevel += 1;

        ResetCharacters();
        ResetDots();
        
        Player->hasWon = false;
        timeSinceWin = 0.0f;
        
        
    }
}

void Game::HandleDeathSequence(float deltaTime) {

    assert(Player->isDead);

    timeSinceDeath += deltaTime;

    if (timeSinceDeath < 2.0f) {
        Player->CycleLength = 5.0f; // makes him stop animating for at least 2.5 seconds

    }
    else if (timeSinceDeath >= 2.0f && timeSinceDeath < 4.0f) {
        Player->Rotation = 0.0f;
        Player->AnimationSprites = deathSprites;
        Player->CycleLength = 2.0f;
    }
    else if (timeSinceDeath >= 4.0f) {

        if (extraLives.size() > 0) {
            delete extraLives.back();
            extraLives.pop_back();
            ResetCharacters();
            
        }
        else {
            //std::cout << " game over!!!!!!!!" << std::endl;
            CreateGameOver();
            GameOver = true;
        }
        //std::cout << "start restart sequence" << std::endl;
    }
    
}

void Game::HandleCollisions() {
    std::map<Direction, float> dirToRot = {
        {RIGHT, 0.0f},
        {UP, 270.0f},
        {LEFT, 180.0f},
        {DOWN, 90.0f}
    };

    //bool collision = CheckPlayerWallCollision();
    HandleDotCollisions();
    HandlePowerDotCollisions();
    HandleGhostCollisions();

    if (fruit) {
        HandleFruitCollision();
    }

    bool queuedCollision = CheckPlayerWallCollision(Player->QueuedDirection);
    bool currentCollision = CheckPlayerWallCollision(Player->CurrentDirection);
    
    Player->Stuck = currentCollision;

    bool snapBothAxes;
    if (!queuedCollision) {
        
        // if ((Player->CurrentDirection != Player->QueuedDirection) &&
        //     (dirToVec(Player->CurrentDirection) != -dirToVec(Player->QueuedDirection)))
        // {   
        if (Player->CurrentDirection == Player->QueuedDirection) {
            
            SnapPlayerToGrid(Player->QueuedDirection, snapBothAxes = false);
        }
        //}
        Player->CurrentDirection = Player->QueuedDirection;
        //Player->QueuedDirection = NONE;
        
        //std::cout << "updating rotation inside handle collision" << std::endl;
        Player->Rotation = dirToRot[Player->CurrentDirection];
        Player->Facing = Player->CurrentDirection;

    }

    if (queuedCollision) {
        //std::cout << "Collision with queued direction at " << Player->QueuedDirection << std::endl;
    }


    if (currentCollision) {
        //std::cout << "Current collision with player going direction " << Player->CurrentDirection << std::endl;
        // std::cout << " Current collision " << std::endl;
        //std::cout << "Previous position = " << Player->Position.x << " " << Player->Position.y << std::endl;
        SnapPlayerToGrid(Player->CurrentDirection, snapBothAxes = true);
        //std::cout << "Snapped position = " << Player->Position.x << " " << Player->Position.y << std::endl;
                
        // // "unstick" player
        // std::cout << "Previous position = " << Player->Position.x << " " << Player->Position.y << std::endl;
        // std::cout << " Jumped back " << std::endl;
        // std::cout << "subtracting " << dirToVec(Player->CurrentDirection).x  << " " << dirToVec(Player->CurrentDirection).y << std::endl;
        // Player->Position -= dirToVec(Player->CurrentDirection);
        // std::cout << "Jumped position = " << Player->Position.x << " " << Player->Position.y << std::endl;

        Player->CurrentDirection = NONE;

        
        // SNAP PLAYER CENTER to grid, therefore updating current sensor
        // calc diff in player center and tile center, then offset player pos by that amount

        // use floor to determine which row we are on


    }
    // if (!currentCollision) {
    //     Player->CurrentDirection = NONE;
    // }



}

void Game::ActivatePowerMode() {
    std::cout << "Power mode activated" << std::endl;
    playerEatStreak = 0;
    Player->CurrentMode = HUNTING;
    Player->HuntingTimer = GetSpec("frightenedTime", CurrentLevel);
    Player->Velocity = GetSpec("frightenedPlayerSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
    // Go through every ghost and change their color
    for (Ghost* ghost : Ghosts) {
        GhostMode m = ghost->CurrentMode;
        if (m != SCATTER && m != CHASE && m != FRIGHTENED) {
            continue;
        }
        ghost->EnterFrightened(Player);

    }
    //std::vector<Ghost*> Ghosts;
    
}

void Game::SpawnEatDisplay(glm::vec2 position, int value) {

    
    //int eatDisplayScore = pow(2, playerEatStreak) * 100;
    //std::string texString = "eat";
    std::string texString = std::to_string(value);    

    std::cout << "texString" <<  texString << std::endl;
    
    //std::vector<Texture2D> tex = {ResourceManager::GetTexture(texString)};

    if (value == 200 || value == 400 || value == 800 || value == 1600) {
        eatDisplayObject = new GameObject(position+TILE_SIZE/2.0f, glm::vec2((15.0f/7.0f)*TILE_SIZE,TILE_SIZE)/2.0f, TILE_SIZE, ResourceManager::GetTexture(texString));
        eatDisplayObject->Color = glm::vec3(1.0f, 1.0f, 0.0f);
    }
    else {
        eatDisplayObject = new GameObject(position+TILE_SIZE/2.0f, glm::vec2((20.0f/7.0f)*TILE_SIZE,TILE_SIZE)/2.0f, TILE_SIZE, ResourceManager::GetTexture(texString));
        eatDisplayObject->Color = glm::vec3(0.0f, 1.0f, 1.0f);
    }

    
}
void Game::HandleGhostCollisions() {
    bool collision;
    for (Ghost* ghost : Ghosts) {


        collision =  (Player->Col == ghost->Col && Player->Row == ghost->Row);
        if (!collision) {
            continue;
        }
        if (ghost->CurrentMode == FRIGHTENED && !ghost->HasBeenEatenThisDot) {
            ghost->EnterEaten();
            Player->EnterEatingState();
            FreezeAllGhosts(1.0f, ghost->Name);
            playerEatStreak += 1;
            int pointsGained = pow(2, playerEatStreak) * 100;

            IncreaseScore(pointsGained);
            SpawnEatDisplay(ghost->Position, pointsGained);


        }
        else if (ghost->CurrentMode == SCATTER || ghost->CurrentMode == CHASE) {

            if (!Player->isDead) {
                KillPlayer(); // debug
            }


        }

    }
    
}
void Game::KillPlayer() {
    std::cout << "= = = = = = = = = = = =" << std::endl;
    std::cout << "P L A Y E R   D I E D :(" << std::endl;
    std::cout << "= = = = = = = = = = = =" << std::endl;
    Player->isDead = true;
    Player->Lifespan = 0.0f;
    
    // Player->FreezeFrameTimer = 2.0f;
    // FreezeAllGhosts(2.0f);
    //Player->Sprites = {*deathTextures};
}
void Game::FreezeAllGhosts(float duration, std::string ghostNameException) {
    for (Ghost* ghost : Ghosts) {
        if (ghost->CurrentMode == EATEN && ghost->Name != ghostNameException)
            continue;
        ghost->FreezeTimer = duration;
    }
}
void Game::HandlePowerDotCollisions() {
bool collision;
    glm::vec2 playerCenterBoxPos(Player->Position + (Player->Size / 4.0f));
    glm::vec2 playerHalfSize = (Player->Size / 2.0f);
    for (int i = 0; i < powerDots.size(); i++) {
        collision = CheckCollision(playerCenterBoxPos, playerHalfSize, powerDots[i]->Position+TILE_SIZE/2.0f, powerDots[i]->Size/2.0f);
        if (collision && !powerDots[i]->Destroyed) {
            powerDots[i]->Destroyed = true;
            ActivatePowerMode();
            IncreaseScore(50);
            remainingDots -= 1;
            Player->FreezeFrameTimer = 3.0f/60.0f;
            HandleDotCheckpoints();
        }
    }
}
void Game::HandleDotCollisions() {
    bool collision;
    glm::vec2 playerCenterBoxPos(Player->Position + (Player->Size / 4.0f));
    glm::vec2 playerHalfSize = (Player->Size / 2.0f);
    for (int i = 0; i < dots.size(); i++) {
        collision = CheckCollision(playerCenterBoxPos, playerHalfSize, dots[i]->Position, dots[i]->Size);
        if (collision && !dots[i]->Destroyed) {
            dots[i]->Destroyed = true;
            IncreaseScore(10);
            remainingDots -= 1;
            Player->FreezeFrameTimer = 1.0f/60.0f;
            HandleDotCheckpoints();
        }
    }
}
void Game::HandleDotCheckpoints() {
    // fruit
    if ((remainingDots == (244 - 70)) || (remainingDots == (244 - 170))) {
        SpawnFruit();
    }

    if (Ghosts[0]->CurrentMode != FRIGHTENED) { // only activate elroy when not frightened
        // elroy 2
        if (remainingDots <= GetSpec("elroyDots2", CurrentLevel)) {
            std::cout << " elroy 2 activated " << std::endl;
            Ghosts[0]->Velocity = GetSpec("elroySpeed2", CurrentLevel) * speedRatio * TILE_SIZE;
        }
        // elroy 1
        else if (remainingDots <= GetSpec("elroyDots1", CurrentLevel)) {
            std::cout << " elroy 1 activated " << std::endl;
            Ghosts[0]->Velocity = GetSpec("elroySpeed1", CurrentLevel) * speedRatio * TILE_SIZE;
        }
    }
    
    // win
    if (remainingDots <= 0) {
        Player->hasWon = true;
    }

}
void Game::HandleFruitCollision() {
    assert(fruit && "Checking fruit collisions with no spawned fruit.");
    glm::vec2 playerCenterBoxPos(Player->Position + (Player->Size / 4.0f));
    glm::vec2 playerHalfSize = (Player->Size / 2.0f);
    if (CheckCollision(playerCenterBoxPos, playerHalfSize, fruit->Position, fruit->Size)) {
        
        std::cout << "player collided with fruit" << std::endl;
        float score = GetSpec("bonusPoints", CurrentLevel);
        IncreaseScore(score);
        SpawnEatDisplay(fruit->Position, 100);
        fruit = nullptr;
    }
}

void Game::SpawnFruit() {
    std::cout << "Spawned fruit!" << std::endl;
    glm::vec2 fruitSpawn = glm::vec2(13.5f, 19.5f) * TILE_SIZE;
    //std::vector<Texture2D> tex = {ResourceManager::GetTexture("cherries")};
    fruit = new GameObject(fruitSpawn, glm::vec2(TILE_SIZE)*(14.0f/8.0f), TILE_SIZE, ResourceManager::GetTexture("cherries"));
    fruit->DeathTimer = static_cast<float>(rand())/static_cast<float>(RAND_MAX) + 9.0f;
    std::cout << "random death timer = " << fruit->DeathTimer << std::endl;
}
void Game::IncreaseScore(int points) {

    int prevScore = score;
    score += points;
    if (score >= highscore) {
        highscore = score;
    }
    //std::cout << "SCORE:     " << score << std::endl;
    //std::cout << "HIGH SCORE:" << highscore << std::endl;
    // extra life 
    int prevNumTenThousands = floor(prevScore / 10000);
    int currNumTenThousands = floor(score / 10000);
    if (score >= 10000 && (prevNumTenThousands != currNumTenThousands)) {
        CreateLives(extraLives.size() + 1);
    }
    
    Texture2D tex;
    for (int i = 0; i < scoreNumbers.size(); i++) {

        int thisDigit = (int)floor(score / pow(10.0f, i)) % 10;
        int displayDigit = scoreNumbers.size()-1 - i;

        if (displayDigit >= 0) {
            tex = *digitTextures[thisDigit];
        }
        else {
            tex = ResourceManager::GetTexture("px");
        }
        scoreNumbers[displayDigit]->Sprite = tex;

        if (score >= highscore) {
            highScoreNumbers[displayDigit]->Sprite = tex;
        }
    }
    // // 4 7 5
    // std::vector<Texture2D> tex;
    // int scoreSize = scoreAsString.size();
    // for (int i = 0; i < scoreNumbers.size(); i++) {

    //     int scoreDigit = scoreAsString.size() - i - 1;
    //     std::cout << "score digit = " << scoreDigit << std::endl;
    //     if (scoreDigit >= 0) {
    //         std::cout << " texture should be " << scoreAsString[scoreDigit] << std::endl;
    //         char displayDigit = scoreAsString[scoreDigit];
    //         std::cout << "display digit=" << displayDigit << std::endl;
    //         std::cout << "score digit=" << scoreDigit << std::endl;

    //         tex = {*digitTextures[scoreDigit]};
    //     }
    //     else {
    //         tex = {ResourceManager::GetTexture("px")};
    //     }
    //     int displayScoreDigit = scoreNumbers.size() - i - 1;
    //     std::cout << "display digit = " << displayScoreDigit << std::endl;

    //     scoreNumbers[displayScoreDigit]->Sprites = tex;


        //texVec = {*digitTextures[scoreAsString[scoreAsString.size() - 1]]};

    
    //std::cout << " End call " << std::endl;

    // texVec = {*digitTextures[points % 10]};
    // scoreNumbers[6]->Sprites = texVec;

    // texVec = {*digitTextures[points floor(points/10)]};

    //scoreNumbers[scoreNumbers.size()-1].Sprites = digitTextures[score% 10];

}


void Game::SnapPlayerToGrid(Direction direction, bool bothAxes) {

    
    glm::vec2 prevPlayerPosition = Player->Position;
    // snap player to correct position for tile
    glm::vec2 currTilePos = glm::vec2(Player->Col * TILE_SIZE, Player->Row * TILE_SIZE + TOP_BORDER_HEIGHT);
    glm::vec2 currTileCenter = glm::vec2(currTilePos.x+TILE_SIZE/2.0f, currTilePos.y+TILE_SIZE/2.0f);

    if (direction == UP || direction == DOWN || bothAxes) {
        Player->Position = glm::vec2(currTileCenter.x-Player->Size.x/2.0f, Player->Position.y);// + dirToVec(Player->CurrentDirection);

    }
    if (direction == LEFT || direction == RIGHT || bothAxes) {
        Player->Position = glm::vec2(Player->Position.x, currTileCenter.y-Player->Size.y/2.0f);// + dirToVec(Player->CurrentDirection);

    }
    //Player->Position = glm::vec2(currTileCenter.x-Player->Size.x/2.0f, currTileCenter.y-Player->Size.y/2.0f);// + dirToVec(Player->CurrentDirection);


    // if (prevPlayerPosition != Player->Position) {
    //     std::cout << "Snap Direction: " << direction << std::endl;
    //     std::cout << "Snapped from: " << prevPlayerPosition.x << " " << prevPlayerPosition.y << std::endl;
    //     std::cout << "Snapped to: " << Player->Position.x << " " << Player->Position.y << std::endl;

    // }
}
// function will return true or false if there is a collision in that sensor direction
bool Game::CheckPlayerWallCollision(Direction direction) {

    glm::vec2 sensorPos;
    glm::vec2 sensorSize;
    float sensorWidth = 1.0f;
    glm::vec2 playerCenter(Player->Position.x + Player->Size.x/2.0f, Player->Position.y + Player->Size.y/2.0f);
    float halfTile = TILE_SIZE / 2.0f;
    float sensorPad = 2.0f;
    if (direction == RIGHT) { 
        sensorPos = glm::vec2(playerCenter.x + halfTile, playerCenter.y - halfTile + sensorPad);
        sensorSize = glm::vec2(sensorWidth, TILE_SIZE - sensorPad*2);
    }
    else if (direction == UP) {
        sensorPos = glm::vec2(playerCenter.x - halfTile + sensorPad, playerCenter.y - halfTile);
        sensorSize = glm::vec2(TILE_SIZE-sensorPad*2, sensorWidth);
    }
    else if (direction == LEFT) {
        sensorPos = glm::vec2(playerCenter.x - halfTile, playerCenter.y - halfTile + sensorPad);
        sensorSize = glm::vec2(sensorWidth, TILE_SIZE -sensorPad*2);
    }
    else if (direction == DOWN) {
        sensorPos = glm::vec2(playerCenter.x - halfTile + sensorPad, playerCenter.y + halfTile);
        sensorSize = glm::vec2(TILE_SIZE-sensorPad*2, sensorWidth);
    }

    if (direction == Player->CurrentDirection) {
        CurrentSensor->Position = sensorPos;
        CurrentSensor->Size = sensorSize;
    }
    if (direction == Player->QueuedDirection) {
        QueuedSensor->Position = sensorPos;
        QueuedSensor->Size = sensorSize;
    }

    // loop through all walls
    for (int i = 0; i < walls.size(); i ++) {
        if (CheckCollision(sensorPos, sensorSize, walls[i]->Position, walls[i]->Size)) {
            return true;
        }
    }
    return false;
}

bool Game::CheckCollision(glm::vec2 onePos, glm::vec2 oneSize, glm::vec2 twoPos, glm::vec2 twoSize) {
    return (((
        (onePos.x + oneSize.x >= twoPos.x) &&
        (twoPos.x + twoSize.x >= onePos.x) &&
        (onePos.y + oneSize.y >= twoPos.y) &&
        (twoPos.y + twoSize.y >= onePos.y))));
}


void Game::ProcessInput(float dt) {

    if (this->State != GAME_ACTIVE) {
        return;
    }

    bool right = this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT];
    bool up = this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP];
    bool left = this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT];
    bool down = this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN];

    float velocity = PLAYER_VELOCITY * dt;


    if (right) {
        Player->QueuedDirection = RIGHT;
        //Player->Rotation = 0.0f;
    }
    if (up) {
        Player->QueuedDirection = UP;
        //Player->Rotation = 270.0f;
    }
    if (left) {
        Player->QueuedDirection = LEFT;
        //Player->Rotation = 180.0f;
    }
    if (down) {
        Player->QueuedDirection = DOWN;
        //Player->Rotation = 90.0f;
    }

    if (this->Keys[GLFW_KEY_K]) {
        std::cout << " Killing player .. " << std::endl;
        Player->isDead = true;
        this->Keys[GLFW_KEY_K] = false;

    }

}

void Game::Render() {

    if (this->State != GAME_ACTIVE) {
        return;
    }

    Background->Draw(*Renderer);
    //MapTiles->Draw(*Renderer);


    // for (int i = 0; i < walls.size(); i++) {
    //     walls[i]->Draw(*Renderer);
    // }
    for (int i = 0; i < dots.size(); i++) {
        if (!dots[i]->Destroyed) {
            dots[i]->Draw(*Renderer);

        }
    }

    if (!GameOver) {
        for (int i = 0; i < powerDots.size(); i++) {
            if (powerDots[i]->Destroyed) {
                continue;
            }
            if ((int)(Player->Lifespan*3.0f) % 2 == 1) {
                powerDots[i]->Draw(*Renderer);
            }
        }
    }

    if (fruit && !GameOver) {
        fruit->Draw(*Renderer);
    }

    
    //     // QueuedSensor->Draw(*Renderer);
    //     // CurrentSensor->Draw(*Renderer);
    // player frame 0 = open mouth, 1 = closed mouth
    // only stop animation when (A) against a wall or (B) start of game
    if (!Player->hasStarted) {
        Player->Animate(*Renderer, 0);
    }
    else if (Player->Stuck && !Player->isDead) {
        Player->Animate(*Renderer, 0);
    }
    else if (!GameOver){
        //std::cout << "Animating dead player?" << std::endl;
        Player->Animate(*Renderer);
    }


    
    if (timeSinceDeath < 2.0f) {
        for (Ghost* ghost : Ghosts) {

            ghost->Animate(*Renderer);
            //ghost->Draw(*Renderer);

            if (ghost->FreezeTimer <= 0.0f || ghost->CurrentMode != EATEN) {
                ghost->EyesObj->Animate(*Renderer, ghost->EyesObj->currSprite);
            }
            //}
            //ghost->DebugMapTarget->Draw(*Renderer);
        }

    }
    
    for (int i = 0; i < highScoreLetters.size(); i++) {
        highScoreLetters[i]->Draw(*Renderer);
    }
    for (int i = 0; i < scoreNumbers.size(); i++ ) {
        scoreNumbers[i]->Draw(*Renderer);
    }
    for (int i = 0; i < highScoreNumbers.size(); i++ ) {
        highScoreNumbers[i]->Draw(*Renderer);
    }
    for (int i = 0; i < extraLives.size(); i++) {
        extraLives[i]->Draw(*Renderer);
    }

    // draw ready letters if they exist
    if (readyLetters[0]) {
        for (int i = 0; i < readyLetters.size(); i++) {
            readyLetters[i]->Draw(*Renderer);   

        }
    }

    // draw game over letters
    if (GameOver && gameOverLetters[0]) {
        for (int i = 0; i < gameOverLetters.size(); i++) {
            gameOverLetters[i]->Draw(*Renderer);
        }
    }

    for (int i = 0; i < oneUpLetters.size(); i++) {
        // oscillate letters on and off
        if ((int)(Player->Lifespan*3.0f) % 2 == 0) {
            oneUpLetters[i]->Draw(*Renderer);
        }
    }

    if (eatDisplayObject) {
        //std::cout << "drawing eat display object " << std::endl;
        eatDisplayObject->Draw(*Renderer);
    }
  
   
}


void Game::LoadAllTextures() {
    ResourceManager::LoadTexture("textures/pacmanNew1.png", true, "pacman1");
    ResourceManager::LoadTexture("textures/finalPacman2.png", true, "pacman2");
    ResourceManager::LoadTexture("textures/pacmanNew2.png", true, "pacman3");

    ResourceManager::LoadTexture("textures/map.png", false, "walls");
    ResourceManager::LoadTexture("textures/mapTiles.png", true, "mapTiles");

    ResourceManager::LoadTexture("textures/grayWall.png", true, "grayWall");
    ResourceManager::LoadTexture("textures/px.png", true, "px");
    ResourceManager::LoadTexture("textures/map224x248.png", true, "map224x248");
    ResourceManager::LoadTexture("textures/powerDot.png", true, "powerDot");

    ResourceManager::LoadTexture("textures/ghost.png", true, "ghost");
    ResourceManager::LoadTexture("textures/ghost2.png", true, "ghost2");
    ResourceManager::LoadTexture("textures/eyesRight.png", true, "eyesRight");
    ResourceManager::LoadTexture("textures/eyesUp.png", true, "eyesUp");
    ResourceManager::LoadTexture("textures/eyesLeft.png", true, "eyesLeft");
    ResourceManager::LoadTexture("textures/eyesDown.png", true, "eyesDown");
    ResourceManager::LoadTexture("textures/eyesFrightened.png", true, "eyesFrightened");
    std::cout << " eyes textures loaded.." << std::endl;
    // load font textures
    std::array<std::string, 26> letters = {
        "h", "i", "g", "s", "c", "o", "r", "e", 
        "a", "d", "y", "!",
        "u", "p",
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "m", "v" // last letters needed for game over
    };
    std::string s;
    for (int i = 0; i < letters.size(); i++)  {
        s = "textures/emulogicFont/" + letters[i] + ".png";
        ResourceManager::LoadTexture(s.c_str(), true, letters[i].c_str());
    }

    std::array<std::string, 8> fruits = {
        "cherries", "strawberry", "peach", "grapes", "galaxian", "bell", "key"
    };

    for (std::string fruit : fruits) {
        s = "textures/fruits/" + fruit + ".png";
        ResourceManager::LoadTexture(s.c_str(), true, fruit.c_str());
    }

    std::array<std::string, 12> points = {
        "100", "200", "300", "400", "500", "700", 
        "800", "1000", "1600", "2000", "3000", "5000"
    };

    for (int i = 1; i <= 10; i++) {
        s = "textures/death/death" + std::to_string(i) + ".png";
        ResourceManager::LoadTexture(s.c_str(), true, ("death" + std::to_string(i)).c_str());
    }

    for (std::string point : points) {
        s = "textures/numbers/" + point + ".png";
        ResourceManager::LoadTexture(s.c_str(), true, point.c_str());
    }


    // ResourceManager::LoadTexture("textures/numbers/200.png", true, "200");
    // ResourceManager::LoadTexture("textures/numbers/400.png", true, "400");
    // ResourceManager::LoadTexture("textures/numbers/800.png", true, "800");
    // ResourceManager::LoadTexture("textures/numbers/1600.png", true, "1600");

    
}

void Game::PrintFPS(float dt) {

    FrameCount++;
    FrameTimer += dt;
    float displayRate = 0.5f;
    if (FrameTimer >= displayRate) {
        std::cout << FrameCount * (1.0f / displayRate) << " fps" << std::endl;


        if (FrameCount * (1.0f / displayRate) <= 1200) {
            numFrames++;
            framesSum += FrameCount * (1.0f / displayRate);
        }

        FrameTimer = 0.0f;
        FrameCount = 0;
    }

    
    std::cout << "avg " << framesSum / (numFrames+0.0000001f) << std::endl;

    // display fps
    //std::cout << (1.0f / dt) << std::endl;
    // float frames = (1.0/dt);
    // fpsQueue.push(frames);
    // queueSum += frames;

    // if (fpsQueue.size() > 600) {
    //     float f = fpsQueue.front();
    //     fpsQueue.pop();
    //     queueSum -= f;
    // }
    // std::cout << queueSum / fpsQueue.size() << std::endl;
}
