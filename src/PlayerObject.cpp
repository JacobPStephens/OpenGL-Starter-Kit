#include "PlayerObject.h"
#include <iostream>
#include "LevelSpecs.h"

// PlayerObject::PlayerObject()
//     : GameObject() {};

std::array<float,8> ScatterTimings; 

PlayerObject::PlayerObject(glm::vec2 position, glm::vec2 size, const float tileSize, float velocity, Texture2D& sprite, float cycleLength, std::string name)
    : GameObject(position, size, tileSize, sprite, cycleLength, glm::vec3(1.0f), 1.0f, velocity, name) {
        this->CurrentDirection = LEFT;
        this->Rotation = 180.0f;
        //std::array<float, 8> scatTime = GetScatterTimings(1);
        ScatterTimings = GetScatterTimings(1);
        ModeTimerReference = ScatterTimings[0];
        GhostModeReference = SCATTER;

    };



void PlayerObject::Update(float dt, float screenWidth) {
    GameObject::Update(dt);

    //std::cout << "Player velocity: " << this->Velocity << std::endl;
    //std::cout << "PLAYER="<< ModeToString(GhostModeReference) << " " << ModeTimerReference << std::endl;

    if (this->isDead) {
        return;
    }
    //std::cout << this->CurrentMode << std::endl;

    
    // exit frightened
    if (this->CurrentMode == HUNTING) {
        this->HuntingTimer -= dt;
        //std::cout << "hunting timer " << this->HuntingTimer << std::endl;
        if (this->HuntingTimer <= 0.0f) {
            //std::cout << " hunting timer over" << std::endl;
            this->CurrentMode = NORMAL;
            this->Velocity = GetSpec("playerSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
            std::cout << "reset speed" << std::endl;

            
        }
    }

    if (this->CurrentMode != HUNTING && this->CurrentMode != EATING) {
        ModeTimerReference -= dt;
    }
    if (ModeTimerReference <= 0.0f) {
        HandleSwitchMode();
    }


    if (this->EatTimer > 0.0f) {
        this->EatTimer -= dt;
    }
    else if (this->CurrentMode == EATING) {
        ExitEatingState();
    }

    this->Move(dt, screenWidth);
    glm::vec2 playerCenter = glm::vec2(this->Position.x + this->Size.x / 2.0f, this->Position.y + this->Size.y/2.0f);
    this->Row = floor(playerCenter.y / this->TILE_SIZE) - (TOP_BORDER_HEIGHT / TILE_SIZE);
    this->Col = floor(playerCenter.x / this->TILE_SIZE);
    //std::cout << "player is on row, col" << this->Row << "," << this->Col << std::endl; // off by one thing
    //std::cout << this->CurrentDirection << " " << dirToVec(this->CurrentDirection).x << " " << dirToVec(this->CurrentDirection).y << std::endl;

    this->PreviousMode = CurrentMode;
}

void PlayerObject::HandleSwitchMode() {
    if (ModeIdxReference + 1 < ScatterTimings.size()) {
        ModeIdxReference += 1;
        GhostModeReference = switchMode(GhostModeReference);
        ModeTimerReference = ScatterTimings[ModeIdxReference];
    }
    else {
        ModeTimerReference = ScatterTimings[ScatterTimings.size()-1];
    }  

    std::cout << "Player Mode idx reference " << this->ModeIdxReference << std::endl;
}

void PlayerObject::EnterEatingState() {
    this->CurrentMode = EATING;
    this->EatTimer = 1.0f;
    this->Alpha = 0.0f;
    
}

void PlayerObject::ExitEatingState() {
    this->CurrentMode = HUNTING;
    this->Alpha = 1.0f;
}

glm::vec2 PlayerObject::Move(float dt, float screenWidth) {

    if (this->Stuck) {
        return glm::vec2(this->Position.x, this->Position.y);
    }
    if (this->FreezeFrameTimer > 0) {
        this->FreezeFrameTimer -= dt;

        //std::cout << "Skipped one mvmt frame due to freeze frames: " << this->FreezeFrameTimer << std::endl;

        return glm::vec2(this->Position.x, this->Position.y);
    }

    if (this->CurrentMode == EATING) {
        return glm::vec2(this->Position.x, this->Position.y);
    }

    //std::cout << Sprites << std::endl;
    this->Position += dirToVec(this->CurrentDirection) * dt * this->Velocity;
    //std::cout << "player moved" << std::endl;
    // this->Position.x = (float)fmod((double)this->Position.x, (double)screenWidth);

    // wrapping
    if (this->Position.x < 0.0f-this->Size.x) {
        this->Position.x = screenWidth;
    }
    if (this->Position.x > screenWidth) {
        this->Position.x = 0.0f-this->Size.x;
    }

    return glm::vec2(this->Position.x, this->Position.y);

}

