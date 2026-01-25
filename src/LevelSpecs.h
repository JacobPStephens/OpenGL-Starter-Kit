#ifndef LEVEL_SPECS_H
#define LEVEL_SPECS_H
#include <map>
#include <string>
#include <iostream>

inline const float RESOLUTION_SCALE = 3.0f;
inline const float TILE_SIZE = 8.0f * RESOLUTION_SCALE;
inline const float TOP_BORDER_HEIGHT = 3.0f * TILE_SIZE;
inline const float speedRatio = 9.469697025f;
inline int CurrentLevel = 1;
inline int remainingDots = 240 + 4;

inline std::map<std::string, std::map<int, float>> LevelSpecs = 
{   
    {"bonusPoints",  {
        {1, 100.0f},
        {2, 300.0f},
        {3, 500.0f},
        {5, 700.0f},
        {7, 1000.0f},
        {9, 2000.0f},
        {11, 3000.0f},
        {13, 5000.0f},
    }},
    {"playerSpeed",  {
        {1, 0.80f},
        {2, 0.90f},
        {5, 1.00f},
        {21, 0.90f},
    }},

    {"ghostSpeed",  {
        {1, 0.75f},
        {2, 0.85f},
        {5, 0.95f},

    }},
    {"ghostTunnelSpeed",  {
        {1, 0.40f},
        {2, 0.45f},
        {5, 0.50f},

    }},
    {"elroyDots1",  {
        {1, 20.0f},
        {2, 30.0f},
        {3, 40.0f},
        {6, 50.0f},
        {9, 60.0f},
        {12, 80.0f},
        {15, 100.0f},
        {19, 120.0f},

    }},
    {"elroySpeed1",  {
        {1, 0.80f},
        {2, 0.90f},
        {5, 1.00f},

    }},
    {"elroyDots2",  {
        {1, 10.0f},
        {2, 10.0f},
        {3, 20.0f},
        {6, 25.0f},
        {9, 30.0f},
        {12, 40.0f},
        {15, 50.0f},
        {19, 60.0f},

    }},
    {"elroySpeed2",  {
        {1, 0.85f},
        {2, 0.95f},
        {5, 1.05f},

    }},
    {"frightenedPlayerSpeed",  {
        {1, 0.90f},
        {2, 0.95f},
        {5, 1.00f},
    }},
    {"frightenedGhostSpeed",  {
        {1, 0.50f},
        {2, 0.55f},
        {5, 0.60f},
    }},
    {"frightenedTime",  {
        {1, 6.0f},
        {2, 5.0f},
        {3, 4.0f},
        {4, 3.0f},
        {5, 2.0f},
        {6, 5.0f},
        {7, 2.0f},
        {9, 1.0f},
        {10, 5.0f},
        {11, 2.0f},
        {12, 1.0f},
        {14, 3.0f},
        {15, 1.0f},

    }},
    {"numberOfFlashes",  {
        {1, 5.0f},
        {9, 3.0f},
        {10, 5.0f},
        {12, 3.0f},
        {14, 5.0f},
        {15, 3.0f},

    }},


};

inline float GetSpec(std::string subMapName, int level) {
    std::map<int, float>* subMap = &LevelSpecs[subMapName];
    int key = level;
    while (subMap->count(key) == 0) {
        key -= 1;
    }
    //std::cout << "returning key|value " << key << "|" << subMap->at(key) << " for map named " << subMapName << " with original level lookup " << level << std::endl;
    return subMap->at(key);




};

inline std::map<int, std::string> LevelToFruit = {
    {1, "cherries"},
    {2, "strawberry"},
    {3, "peach"},
    {5, "apple"},
    {7, "grapes"},
    {9, "galaxian"},
    {11, "bell"},
    {13, "key"},
};

inline std::array<float, 8> GetScatterTimings(int level) {
    assert(level >= 1 && "ERROR: Level must be at least 1");
    if (level == 1) {
        //return std::array<float,8>{4.0f, 5.0f, 4.0f, 5.0f, 5.0f, 20.0f, 5.0f, 20.0f};
        return std::array<float,8>{7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 20.0f, 5.0f, 20.0f};
    }
    else if (level >= 2 && level <= 4) {
        return std::array<float,8>{7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 1033.0f, 0.0167f, 10000.0f};
    }
    return std::array<float,8>{5.0f, 20.0f, 5.0f, 20.0f, 5.0f, 1037.0f, 0.0167f, 10000.0f};
    
    
};


#endif