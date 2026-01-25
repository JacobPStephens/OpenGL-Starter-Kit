#include "Ghost.h"
#include "LevelSpecs.h"
#include <iostream>


const glm::vec2 OUTSIDE_HOUSE = glm::vec2(14.0f * TILE_SIZE, 11.0f * TILE_SIZE + TILE_SIZE/2.0f + TOP_BORDER_HEIGHT);
const glm::vec2 CENTER_HOUSE = glm::vec2(14.0f * TILE_SIZE, 14.0f * TILE_SIZE + TILE_SIZE/2.0f + TOP_BORDER_HEIGHT);
const glm::vec2 GHOST_SIZE_RATIO(14.0f/8.0f);
//const float GHOST_SPEED_RATIO(39.75f/ 8.0f);
const float GHOST_SPEED = GetSpec("ghostSpeed", 1) * speedRatio * TILE_SIZE;
// TODO: Just got the map implemented as a field of the game class. Use that in this GetAdjDirection function.
// Maybe clean up constuctor to only have what's necesary. I feel like I don't have to pass the map every update.
std::map<std::string, glm::vec2> Ghost::ScatterTile= {
    {"redGhost", glm::vec2(25.0f, -4.0f)},
    {"pinkGhost", glm::vec2(2.0f, -4.0f)},
    {"blueGhost", glm::vec2(27.0f, 31.0f)},
    {"orangeGhost", glm::vec2(0.0f, 31.0f)}
};



// std::array<float, 8> Ghost::GetScatterTimings(int level) {
//     assert(level >= 1 && "ERROR: Level must be at least 1");
//     if (level == 1) {
//         return std::array<float,8>{7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 20.0f, 5.0f, 20.0f};
//     }
//     else if (level >= 2 && level <= 4) {
//         return std::array<float,8>{7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 1033.0f, 0.0167f, 1000.0f};
//     }
//     return std::array<float,8>{5.0f, 20.0f, 5.0f, 20.0f, 5.0f, 1037.0f, 0.0167f, 1000.0f};
    
    
// };



static glm::vec3 GetGhostColor(std::string name) {
    glm::vec3 color;
    if (name == "red" || name == "redGhost")
        color = glm::vec3(1.0f, 0.0f, 0.0f);
    else if (name == "pink" || name == "pinkGhost")
        color = glm::vec3(1.0f, 0.72f, 1.0f);
    else if (name == "blue" || name == "blueGhost")
        color = glm::vec3(0.0f, 1.0f, 1.0f);
    else if (name == "orange" || name == "orangeGhost")
        color = glm::vec3(1.0f, 0.72f, 0.325f);
    return color;
}
static glm::vec2 GetGhostHome(std::string name) {
    glm::vec2 home;
    if (name == "red" || name == "redGhost")
        home = glm::vec2(13, 11);
    else if (name == "pink" || name == "pinkGhost")
        home = glm::vec2(13, 14);
    else if (name == "blue" || name == "blueGhost")
        home = glm::vec2(11, 14);
    else if (name == "orange" || name == "orangeGhost")
        home = glm::vec2(15, 14);
    return home;
}

std::array<float, 8> Ghost::ScatterTimings = GetScatterTimings(1);
float ModeTimer = Ghost::ScatterTimings[0];


Ghost::Ghost(std::string name, PlayerObject* player, std::vector<std::vector<int>>* map, const float tileSize, Ghost* redGhost) 
    : GameObject(
        glm::vec2(0.0f),
        glm::vec2(GHOST_SIZE_RATIO * tileSize),
        tileSize,
        ResourceManager::GetTexture("ghost"),
        1.0f,
        GetGhostColor(name),
        1.0f,
        GHOST_SPEED,
        name + std::string("Ghost")
    )
{
    this->TILE_SIZE = tileSize;
    this->Map = map;

    this->AnimationSprites = {
        &ResourceManager::GetTexture("ghost"),
        &ResourceManager::GetTexture("ghost2"),
    };
    this->CycleLength = 0.4f;
    
    Reset(player);

    if (this->Name == "redGhost") {
        SyncTimings(player);

    }

    // this->Row = GetGhostHome(this->Name).y;
    // this->Col = GetGhostHome(this->Name).x;


    

    // // if (this->Name == "redGhost") {
    // //     this->Col += 2;
    // // }
    // glm::vec2 tileCenter = glm::vec2(this->Col * tileSize + tileSize/2.0f, this->Row * tileSize + tileSize/2.0f + TOP_BORDER_HEIGHT);
    // this->Position = tileCenter - this->Size / 2.0f;
    // this->Position.x += TILE_SIZE/2.0f;

    // if (this->Name == "redGhost") {
    //     this->CurrentMode = SCATTER;
    //     this->Dir = LEFT;
    // }
    // else if (this->Name == "pinkGhost") {
    //     this->CurrentMode = HOME_EXIT;
    // }
    // else {
    //     this->HomeIdleTimer = (this->Name == "blueGhost" ? 3.0f: 6.0f);
    //     this->CurrentMode = HOME_IDLE;
    //     this->Dir = UP;
    // }

    // // this->Dir = RIGHT;
    // if (this->Name == "redGhost") {
    //     std::cout << "Init row/col: " << this->Row << " " << this->Col << std::endl;
    //     std::cout << "Init dir: " << this->Dir << std::endl;

    // }
    // this->AdjTargetPos = glm::vec2(this->Col, this->Row);
    // this->ModeTimer = player->ModeTimerReference;
    // this->MapTargetPos = player->Position;

    // /this->AdjTargetPos = glm::vec2(this->Row, this->Col);

    

    this->DebugMapTarget = new GameObject(
        glm::vec2(0.0f), // position
        glm::vec2(tileSize),
        tileSize,
        ResourceManager::GetTexture("px"),
        1.0f,
        GetGhostColor(name),
        1.0f, 
        0.0f,
        name + "redMapTarget"
    );
    //Eyes(Ghost* parentGhost, float tileSize, std::vector<Texture2D> sprites);

    std::cout << "eye obj created" << std::endl;
    this->EyesObj = new Eyes(this, this->TILE_SIZE);
    

}

void Ghost::TurnAround(Direction currentDir, glm::vec2 currentAdjTarget) {
    this->Dir = oppositeDirection(currentDir);
    this->AdjTargetPos = currentAdjTarget + dirToVec(this->Dir);

}

float Ghost::GetGhostVelocityAfterTunnel(std::string name) {

    if (this->CurrentMode == FRIGHTENED) {
        return GetSpec("frightenedGhostSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
    }
    if (name != "redGhost") {
        return GetSpec("ghostSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
    }
    // red ghost elroy cases
    if (remainingDots <= GetSpec("elroyDots2", CurrentLevel)) {
        return GetSpec("elroySpeed2", CurrentLevel) * speedRatio * TILE_SIZE;
    }
    if (remainingDots <= GetSpec("elroyDots1", CurrentLevel)) {
        return GetSpec("elroySpeed1", CurrentLevel) * speedRatio * TILE_SIZE;
    }
    return GetSpec("ghostSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
    /* note: in the event that pacman dies while elroy is active,
    elroy will not reactivate on the fresh life until orange ghost leaves home.
    it may be possible that a dormant elroy leaving the tunnel can re-activate faster
    than it takes for orange ghost to leave home */


}
void Ghost::Reset(PlayerObject* player) {

    this->Row = GetGhostHome(this->Name).y;
    this->Col = GetGhostHome(this->Name).x;
    std:: cout << "reseting ghosts to home " << std::endl;


    Ghost::ScatterTimings = GetScatterTimings(CurrentLevel);
    SyncTimings(player);
    
    std::cout << "first element of scatter timings = " << Ghost::ScatterTimings[0];


    float tileSize = TILE_SIZE;
    // if (this->Name == "redGhost") {
    //     this->Col += 2;
    // }
    glm::vec2 tileCenter = glm::vec2(this->Col * tileSize + tileSize/2.0f, this->Row * tileSize + tileSize/2.0f + TOP_BORDER_HEIGHT);
    this->Position = tileCenter - this->Size / 2.0f;
    this->Position.x += TILE_SIZE/2.0f;

    if (this->Name == "redGhost") {
        this->CurrentMode = SCATTER;
        this->Dir = LEFT;
    }
    else if (this->Name == "pinkGhost") {
        this->CurrentMode = HOME_EXIT;
    }
    else {
        this->HomeIdleTimer = (this->Name == "blueGhost" ? 4.0f: 15.0f);
        this->CurrentMode = HOME_IDLE;
        this->Dir = UP;
    }

    // this->Dir = RIGHT;
    if (this->Name == "redGhost") {
        std::cout << "Init row/col: " << this->Row << " " << this->Col << std::endl;
        std::cout << "Init dir: " << this->Dir << std::endl;

    }

    this->Alpha = 1.0f;
    this->Color = GetGhostColor(this->Name);
    this->Velocity = GetSpec("ghostSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
    this->HasBeenEatenThisDot = false; // probably not needed here

    std::cout << " new ghost speed is " << this->Velocity << std::endl;

    this->AdjTargetPos = glm::vec2(this->Col, this->Row);

    // make sure you reset player before ghost
    // this->ModeTimer = player->ModeTimerReference;
    // this->ModeIdx = player->ModeIdxReference;
    // this->CurrentMode = player->GhostModeReference;
    //this->ModeTimer = player->ModeTimerReference;
}
void Ghost::Update(float deltaTime, PlayerObject* player, Ghost* redGhost) {
    if (player->isDead || player->hasWon) {
        return;
    }

    if (this->Name == "redGhost") {
        
        //std::cout << "Red ghost has been eaten? " << this->HasBeenEatenThisDot << std::endl;
        // std::cout << "Current blue ghost mode: " << ModeToString(this->CurrentMode) << std::endl;
        // std::cout << "Frightened timer: " << this->FrightenedTimer << std::endl;
        //std::cout << "orange ghost idle timer: " << HomeIdleTimer << std::endl;
       //std::cout << this->Name << " " << ModeToString(this->CurrentMode) << " " << this->ModeTimer << std::endl; 
       //std::cout << this->Col << " " << this->Row << std::endl;
    }
    //std::cout << this->Name << " " << ModeToString(this->CurrentMode) << " " << this->ModeTimer << std::endl;

    // if (!player->hasStarted) {
    //     return;
    // }
    GameObject::Update(deltaTime);
    
    this->EyesObj->Update(deltaTime);

    this->Row = floor(this->CenterPos.y/ (int)this->TILE_SIZE) - (TOP_BORDER_HEIGHT / TILE_SIZE);
    this->Col = floor(this->CenterPos.x / (int)this->TILE_SIZE);

    if (!player->hasStarted) {
        return;
    }
    switch (CurrentMode) {
        case HOME_IDLE:
            HandleHomeIdle(deltaTime, redGhost, player); // set dir
        break;
        case HOME_ENTER:
            HandleHomeEnter(player, deltaTime);
        break;
        case HOME_EXIT:
            HandleHomeExit(player, deltaTime); // set dir
        break;
        case FRIGHTENED:
            HandleFrightened(deltaTime, player, redGhost); // set state
        break;
        case EATEN:
            HandleEaten(deltaTime, player, redGhost);
        break;
        default:
            HandleScatterChase(player, deltaTime, redGhost);
        break;
    }


    if (this->FreezeTimer >= 0.0f) {
        this->FreezeTimer -= deltaTime;

        if (this->FrightenedTimer > 0.0f) {
            this->FrightenedTimer += deltaTime;
        }
        return;
    }

    //if (player->hasStarted) {
    this->Position = Move(deltaTime, this->Position, this->Dir, this->Velocity);   

    //}

    float screenWidth = 224.0f * RESOLUTION_SCALE;
    // wrapping
    if (this->Position.x < 0.0f-this->Size.x) {
        this->Position.x = screenWidth;
        this->AdjTargetPos = glm::vec2(27.0f, 14.0f);
    }
    if (this->Position.x > screenWidth) {
        this->Position.x = 0.0f-this->Size.x;
        this->AdjTargetPos = glm::vec2(1.0f, 14.0f);
        std::cout << "wrapped and updated adj target pos" << std::endl;
    }

    // // if (CurrentMode != FRIGHTENED && CurrentMode != EATEN) {
    // //     ModeTimer -= deltaTime;
    // // }
    // // std::cout << this->Name << ": " << this->ModeTimer << std::endl;






    // if (ModeTimer <= 0.0f) {
    //     SwitchScatterChase();
    // }

    // this->MapTargetPos = GetMapTarget(player->Row, player->Col, player->Facing, this->Name, Ghost::CurrentMode, redGhost);

    // // if adj target reached, get new adj target and direction towards it
    // if (AdjTargetReached(glm::vec2(this->CenterPos.x, this->CenterPos.y), this->AdjTargetPos, this->TILE_SIZE)) {
    //     //std::cout << "adj target reached" << std::endl;
    //     this->AdjTargetPos = GetAdjTarget(this->Row, this->Col, this->Size, this->Dir, this->MapTargetPos, this->TILE_SIZE, this->Map);//this->Position, this->Size, this->Dir, this->MapTargetPos, tileSize, map);
    //     this->Dir = GetDirection(this->Row, this->Col, this->AdjTargetPos);
    // }



    // if (this->FreezeTimer > 0.0f) {
    //     this->FreezeTimer -= deltaTime;
    //     return;
    // }
    
    // this->Position = Move(deltaTime, this->Position, this->Dir, this->Velocity);   
    // if (CurrentMode == FRIGHTENED) {
    //     FrightenedTimer -= (deltaTime);
    //     if (FrightenedTimer <= 0.0f) {
    //         ExitFrightened();
    //     }
    // }
    // else if (ModeTimer >= 0.0f) {
    //     ModeTimer -= (deltaTime);
    // }

    // std::cout << ModeTimer << std::endl;

    // if (ModeTimer >= 0.0f) {
    //     ModeTimer -= (deltaTime);
    // }

    // if (ModeTimer <= 0.0f) {
    //     if (ModeIdx + 1 < Ghost::ScatterTimings.size()) {
    //         CurrentMode = switchMode(CurrentMode);
    //         TurnAround(this->Dir, this->AdjTargetPos);
    //         std::cout << "Entered " << CurrentMode << std::endl;
    //         ModeTimer = Ghost::ScatterTimings[ModeIdx+1];
    //     }
    //     else {
    //         ModeTimer = Ghost::ScatterTimings[Ghost::ScatterTimings.size()-1];
    //     }        
    // }

    // // set map target (ghost color dependent)
    // this->MapTargetPos = GetMapTarget(player->Row, player->Col, player->Facing, this->Name, Ghost::CurrentMode, redGhost);
    
    this->DebugMapTarget->Position = this->MapTargetPos * TILE_SIZE;//this->AdjTargetPos * this->TILE_SIZE;
    this->DebugMapTarget->Position.y += TOP_BORDER_HEIGHT;
    this->DebugMapTarget->Size = glm::vec2(this->TILE_SIZE);



}

void Ghost::HandleFrightened(float deltaTime, PlayerObject* player, Ghost* redGhost) {

    FrightenedTimer -= deltaTime;
    float flashRate = 0.15f;

    // determine cutoff based on num flashes in level spec table
    float numFlashes = GetSpec("numberOfFlashes", CurrentLevel);
    float flashCutoff = (numFlashes == 5.0f) ? 1.5f : 0.9f;
    
    if (FrightenedTimer <= flashCutoff) {
        int flrRes = std::floor(FrightenedTimer/flashRate);
        int shouldBeWhite = flrRes % 2;

        if (shouldBeWhite == 1) {
            this->Color = glm::vec3(1.0f);
        }
        else {
            this->Color = glm::vec3(0.129f, 0.129f, 1.0f);
        }

        if (FrightenedTimer <= 0.0f) {
            ExitFrightened(player);
        }
    }
    UpdateTargetAndDir(player, redGhost);
}

void Ghost::UpdateTargetAndDir(PlayerObject* player, Ghost* redGhost) {
    this->MapTargetPos = GetMapTarget(player->Row, player->Col, player->Facing, this->Name, Ghost::CurrentMode, redGhost);
    if (AdjTargetReached(glm::vec2(this->CenterPos.x, this->CenterPos.y), this->AdjTargetPos, this->TILE_SIZE)) {
        //std::cout << "ADJACENT TARGET REACHED" << std::endl;
        bool isFrightened = (this->CurrentMode == FRIGHTENED);
        this->AdjTargetPos = GetAdjTarget(this->Row, this->Col, this->Size, this->Dir, this->MapTargetPos, isFrightened, this->TILE_SIZE, this->Map);//this->Position, this->Size, this->Dir, this->MapTargetPos, tileSize, map);

        this->InTunnel = (IsTunnel(this->AdjTargetPos));
        if (this->InTunnel) {
            this->Velocity = GetSpec("ghostTunnelSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
        }
        else {
            this->Dir = GetDirection(this->Row, this->Col, this->AdjTargetPos);
        }

        
    }
    // exit tunnel 
    if ((this->AdjTargetPos == glm::vec2(5.0f, 14.0f) && this->Dir == RIGHT) ||
        (this->AdjTargetPos == glm::vec2(23.0f, 14.0f) && this->Dir == LEFT)) {
            this->Velocity = GetGhostVelocityAfterTunnel(this->Name);
    }
}
// void Ghost::HandleTunnel() {

// }

bool Ghost::IsTunnel(glm::vec2 target) {
    //Col = 14 is tunnel
    bool x, y;
    if (this->AdjTargetPos.x <= 4 || this->AdjTargetPos.x >= 24) {
        x = true;
    }
    if (this->AdjTargetPos.y == 14) {
        y = true;
    }
    return (x && y);

}

void Ghost::HandleEaten(float deltaTime, PlayerObject* player, Ghost* redGhost) {
    UpdateTargetAndDir(player, redGhost);

    //std::cout << this->Col << " " << this->Row << std::endl;
    if (AbsoluteTargetReached(this->Position, OUTSIDE_HOUSE-this->Size/2.0f, 0.5f)) {
        std::cout << "RED GHOST is in position" << std::endl;
        this->CurrentMode = HOME_ENTER;
        this->Velocity = GetSpec("ghostSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
        this->Dir = DOWN;
    }



}
void Ghost::HandleScatterChase(PlayerObject* player, float deltaTime, Ghost* redGhost) {

    if (player->CurrentMode != HUNTING) { // keep timer frozen if respawned
        ModeTimer -= deltaTime;
    }
    if (ModeTimer <= 0.0f) {
        SwitchScatterChase();
    }

    UpdateTargetAndDir(player, redGhost);

}

void Ghost::SwitchScatterChase() {
    TurnAround(this->Dir, this->AdjTargetPos);
    if (ModeIdx + 1 < Ghost::ScatterTimings.size()) {
        ModeIdx += 1;
        CurrentMode = switchMode(CurrentMode);
        std::cout << this->Name << " entered " << ModeToString(CurrentMode) << std::endl;
        ModeTimer = Ghost::ScatterTimings[ModeIdx];
    }
    else {
        ModeTimer = Ghost::ScatterTimings[Ghost::ScatterTimings.size()-1];
    }      
}

void Ghost::HandleHomeIdle(float dt, Ghost* redGhost, PlayerObject* player) {
    this->Velocity = GHOST_SPEED/2.0f;
    // do enter frightened when they leave home exit. Here just change sprite

    
    // switch to exit mode if timer expired
    if (this->HomeIdleTimer <= 0.0f) {
        this->Position.y = CENTER_HOUSE.y - this->Size.y/2.0f;

        this->CurrentMode = HOME_EXIT;
        if (this->Name == "blueGhost") {
            this->Dir = RIGHT;
        }
        else if (this->Name == "orangeGhost") {
            this->Dir = LEFT;            
            
            // re-activate elroy if necessary
            if (remainingDots <= GetSpec("elroyDots1", CurrentLevel)) {
                redGhost->Velocity = GetSpec("elroySpeed1", CurrentLevel) * speedRatio * TILE_SIZE;
            }
            else if (remainingDots <= GetSpec("elroyDots2", CurrentLevel)) {
                redGhost->Velocity = GetSpec("elroySpeed2", CurrentLevel) * speedRatio * TILE_SIZE;
            }

        }
        else {
            this->Dir = UP;
        }
        return;
    }    
    
    if (player->HuntingTimer > 0.0f) {
        this->Color = glm::vec3(0.129f, 0.129f, 1.0f);
        this->EyesObj->currSprite = 4;
    }
    else {
        this->HomeIdleTimer -= dt;
        this->Color = GetGhostColor(this->Name);
    }

    if (this->Dir == DOWN && this->Position.y > 385.0f - this->Size.y + TOP_BORDER_HEIGHT) {
        this->Dir = UP;
    }
    if (this->Dir == UP && this->Position.y < 310.0f + TOP_BORDER_HEIGHT) {
        this->Dir = DOWN;
    }
    //this->Position = Move(dt, this->Position, this->Dir, this->Velocity); 

}

void Ghost::HandleHomeEnter(PlayerObject* player, float dt) {
    //std::cout << "Hanlding home enter.." << std::endl;

    // if (this->Position.y < GetGhostHome(this->Name).y * TILE_SIZE) {
    //     this->Dir = DOWN;
    //         std::cout << "setting dir to down" << std::endl;
    //     }
    // }


    // if (this->Name == "blueGhost")  {
    //     std::cout << "my position: " << this->Position.x << " " << this->Position.y << std::endl;
    //     std::cout << "checking against: " << (GetGhostHome("blueGhost") * TILE_SIZE + TILE_SIZE/2.0f - this->Size/2.0f).x << " " << (GetGhostHome("blueGhost") * TILE_SIZE + TILE_SIZE/2.0f - this->Size/2.0f).y << std::endl;
    // }
    if (this->Dir == DOWN && AbsoluteTargetReached(this->Position, CENTER_HOUSE-this->Size/2.0f, 1.0f)) {

        if (this->Name == "redGhost" || this->Name == "pinkGhost") {
            this->CurrentMode = HOME_EXIT;
            this->Velocity = GHOST_SPEED;
            this->Alpha = 1.0f;
            this->Color = GetGhostColor(this->Name);
        }
        else if (this->Name == "blueGhost") {
            //std::cout << "My name is blue ghost and I hit the center target" << std::endl;
            this->Dir = LEFT;
            
            // if (AbsoluteTargetReached(this->Position, GetGhostHome("blueGhost") * TILE_SIZE + TILE_SIZE/2.0f - this->Size/2.0f, 1.0f)) {
            //     this->CurrentMode = HOME_EXIT;
            //     this->Velocity = GHOST_SPEED;
            //     this->Alpha = 1.0f;
            //     this->Color = GetGhostColor(this->Name);
            // }
        }
        else if (this->Name == "orangeGhost") {
            this->Dir = RIGHT;
        }
        // else if (this->Name == "orangeGhost") {
        //     this->Dir = RIGHT;
        //     if (AbsoluteTargetReached(this->Position, GetGhostHome("orangeGhost") * TILE_SIZE, 5.0f)) {
        //         this->CurrentMode = HOME_EXIT;
        //         this->Velocity = GHOST_SPEED;
        //         this->Alpha = 1.0f;
        //         this->Color = GetGhostColor(this->Name);
        //     }
        // }
    }

    if (this->Name == "blueGhost") {
        glm::vec2 targetPos = GetGhostHome("blueGhost") * TILE_SIZE + TILE_SIZE/2.0f - this->Size/2.0f;
        targetPos.y += TOP_BORDER_HEIGHT;
        targetPos.x += TILE_SIZE/2.0f; // padding
        if (AbsoluteTargetReached(this->Position, targetPos, 1.0f)) {
            //std::cout << "Blue ghost reached home tile" << std::endl;
            this->CurrentMode = HOME_EXIT;
            this->Velocity = GHOST_SPEED;
            this->Alpha = 1.0f;
            this->Color = GetGhostColor(this->Name);
            this->Dir = RIGHT;
        }
    }
    else if (this->Name == "orangeGhost") {
        glm::vec2 targetPos = glm::vec2(GetGhostHome("orangeGhost") * TILE_SIZE + TILE_SIZE/2.0f - this->Size/2.0f);
        targetPos.y += TOP_BORDER_HEIGHT;

        if (AbsoluteTargetReached(this->Position, targetPos, 1.0f)) {
            //std::cout << "Blue ghost reached home tile" << std::endl;
            this->CurrentMode = HOME_EXIT;
            this->Velocity = GHOST_SPEED;
            this->Alpha = 1.0f;
            this->Color = GetGhostColor(this->Name);
            this->Dir = LEFT;
        } 
    }
}

void Ghost::HandleHomeExit(PlayerObject* player, float dt) {
    
    // if (this->Name == "ghostRed" || this->Name == "ghostPink") {
    //     this->Dir = UP;
    // }

    bool isFrightened = player->HuntingTimer > 0.0f && !this->HasBeenEatenThisDot;

    if (this->Dir == UP) {
        if (AbsoluteTargetReached(this->Position, OUTSIDE_HOUSE-this->Size/2.0f, 0.5f)) {
            std::cout << "OUTSIDE HOUSE REACHED!" << std::endl;
            

            // either set state when leaving home to frightened or scatter/chase
            if (isFrightened) {
                this->EnterFrightened(player);
            }
            else {
                SyncTimings(player);
                this->Velocity = GHOST_SPEED;

            }
            
            
            this->AdjTargetPos = GetAdjTarget(this->Row, this->Col, this->Size, this->Dir, this->MapTargetPos, isFrightened, this->TILE_SIZE, this->Map);
            this->Dir = GetDirection(this->Row, this->Col, this->AdjTargetPos);

        }
    }
    else if (AbsoluteTargetReached(this->Position, CENTER_HOUSE-this->Size/2.0f, 0.5f)) {
        this->Dir = UP;
        if (this->Name == "blueGhost") {
            std::cout << " Checking to see if I reached middle of house " << std::endl;
        }
        //std::cout << "CENTER HOUSE REACHED!" << std::endl;
    }

    if (isFrightened) {
        this->Color = glm::vec3(0.129f, 0.129f, 1.0f);
        this->EyesObj->currSprite = 4;
    }
    else {
        this->Color = GetGhostColor(this->Name);
    }
    //this->Position = Move(dt, this->Position, this->Dir, this->Velocity); 

}

bool Ghost::AbsoluteTargetReached(glm::vec2 ghostPos, glm::vec2 absoluteTarget, float threshold) {
    float deltaX = (ghostPos.x - absoluteTarget.x);
    float deltaY = (ghostPos.y - absoluteTarget.y);

    deltaX = (deltaX >= 0) ? deltaX : -deltaX;
    deltaY = (deltaY >= 0) ? deltaY : -deltaY;

    //std::cout << "Delta X " << deltaX << std::endl;
    //std::cout << "Delta Y" << deltaY << std::endl;

    // error checking if goes past target:

    bool passedTarget = false;
    if (this->Dir == RIGHT && deltaY <= threshold && ghostPos.x > absoluteTarget.x) {
        //std::cout << " Fixed error with absolute target reached goign right as " << this->Name << std::endl;
        passedTarget = true;
    }

    else if (this->Dir == LEFT && deltaY <= threshold && ghostPos.x < absoluteTarget.x) {
        //std::cout << " Fixed error with absolute target reached goign left as " << this->Name << std::endl;
        passedTarget = true;
    }

    else if (this->Dir == DOWN && deltaX <= threshold && ghostPos.y > absoluteTarget.y) {
        //std::cout << " Fixed error with absolute target reached goign down as " << this->Name << std::endl;
        passedTarget = true;
    }

    else if (this->Dir == UP && deltaX <= threshold && ghostPos.y < absoluteTarget.y) {
        //std::cout << " Fixed error with absolute target reached goign up as " << this->Name << std::endl;
        passedTarget = true;
    }

    if (passedTarget == true) {
        this->Position = absoluteTarget;
        return true;
    }

    if (deltaX <= threshold && deltaY <= threshold) {
        this->Position = absoluteTarget;
        return true;
    }
    return false;
}

/* Returns new position of ghost using current position, speed, and direction*/
glm::vec2 Ghost::Move(float deltaTime, glm::vec2 pos, Direction dir, float speed) {

    return pos + dirToVec(dir) * speed * deltaTime;
}

glm::vec2 Ghost::GetMapTarget(int playerRow, int playerCol, Direction playerDir, std::string ghostName, GhostMode currentMode, Ghost* redGhost) {
    //std::cout << "Inside GetMapTarget with name = " << ghostName << std::endl;

    if (currentMode == SCATTER) {
        if (this->Name == "redGhost" && remainingDots <= GetSpec("elroyDots1", CurrentLevel)) {
            return glm::vec2(playerCol, playerRow);
        }
        return ScatterTile.at(ghostName);
    }
    if (currentMode == EATEN) {
        // go to outside of ghost house (red ghost's home)
        return GetGhostHome("redGhost");
    }

    if (ghostName == "redGhost") {
        return glm::vec2(playerCol, playerRow);
    }
    else if (ghostName == "pinkGhost") {
        glm::vec2 target = glm::vec2(playerCol, playerRow) + 4.0f * dirToVec(playerDir);
        if (playerDir == UP) // replicate overflow bug in original
            target += glm::vec2(-4.0f, 0.0f);
        return target;
    }
    else if (ghostName == "blueGhost") {
        int deltaX = redGhost->Col - playerCol;
        int deltaY = redGhost->Row - playerRow;
        glm::vec2 target = glm::vec2(playerCol - deltaX, playerRow - deltaY);
        if (playerDir == UP) // replicate overflow bug in original
            target += glm::vec2(-2.0f, -2.0f);
        return target;
    }
    else if (ghostName == "orangeGhost") {
        if (GetDistance(this->Col, this->Row, playerCol, playerRow) >= 8)
            return glm::vec2(playerCol, playerRow);

        //std::cout << EXAMPLE_INT << std::endl;
        return ScatterTile.at("orangeGhost");
    }
    std::cout << "ERROR:: INVALID GHOST NAME" << std::endl;
    return glm::vec2(0.0f);
}

/* Return if ghost has reached adjacent tile, given its center and target tile */
bool Ghost::AdjTargetReached(glm::vec2 ghostCenter, glm::vec2 adjTargetPos, float tileSize) {
    // get target coordinates in pixels from target tile
    glm::vec2 pixelTarget = (adjTargetPos * tileSize) + (tileSize/2.0f);
    pixelTarget.y += TOP_BORDER_HEIGHT;


    //std::cout << "pixel target: " << pixelTarget.x << " " << pixelTarget.y << std::endl;
    // if (this->Name == "redGhost") {
    //     std::cout << " Checking if adj target reached.. " << std::endl;
    //     std::cout << "Ghost center: " << ghostCenter.x << " " << ghostCenter.y << std::endl;
    //     std::cout << "Pixel target: " << pixelTarget.x << " " << pixelTarget.y << std::endl;
    // }
    // compare ghost center to pixel coordinate

    float xDiff = pixelTarget.x - ghostCenter.x;
    float yDiff = pixelTarget.y - ghostCenter.y;


    float absXDiff = (xDiff > 0.0f) ? xDiff : -xDiff;
    float absYDiff = (yDiff > 0.0f) ? yDiff : -yDiff; 

    float threshold = this->Threshold;

    bool xReached = absXDiff <= threshold;
    bool yReached = absYDiff <= threshold;

    bool passedTarget = false;
    if (this->Dir == RIGHT && yReached && (ghostCenter.x - pixelTarget.x > threshold)) {
        //std::cout << this->Name << " passed target going right" << std::endl;
        passedTarget = true;
    }
    else if (this->Dir == LEFT && yReached && (ghostCenter.x - pixelTarget.x < -threshold)) {
        //std::cout << this->Name << " passed target going left" << std::endl;
        passedTarget = true;
    }
    else if (this->Dir == UP && xReached && (ghostCenter.y - pixelTarget.y < -threshold)) {
        // std::cout << this->Name << " passed target going up" << std::endl;
        passedTarget = true;
    }
    else if (this->Dir == DOWN && xReached && (ghostCenter.y - pixelTarget.y > threshold)) {
        //std::cout << this->Name << " passed target going down" << std::endl;
        passedTarget = true;
    }

    if (passedTarget) {
    //        this->Position = pixelTarget - (GHOST_SIZE_RATIO * TILE_SIZE);
        glm::vec2 centerOfTarget = adjTargetPos * TILE_SIZE + (TILE_SIZE / 2.0f);
        this->Position = centerOfTarget - (this->Size / 2.0f);
        this->Position.y += TOP_BORDER_HEIGHT;

        //std::cout << " Corrected " << this->Name << " position." << std::endl;
        return true;

    }

    // std::cout << this->Name << " = = = = == = = = == = " << std::endl;
    // std::cout << "gc.x - px.x: " << ghostCenter.x - pixelTarget.x << std::endl;
    // std::cout << "gc.y - px.y: " << ghostCenter.y - pixelTarget.y << std::endl;
    // std::cout << "threshold: " << this->Threshold << std::endl;
    // std::cout << "xReached | yReached" << xReached << "|" << yReached << std::endl;
    // if (this->Name == "redGhost" && xReached) {
    //     std::cout << " = = = = = = = = = = = = = " << std::endl;
    //     std::cout << "xDiff: " << xDiff << std::endl;
    //     std::cout << "abs(xDiff): " << abs(xDiff) << std::endl;
    //     std::cout << "Custom abs xdiff: " << absXDiff << std::endl;

    // }
    // reached in both axes
    return xReached && yReached;
}

/* This function updates ghost direction and
returns adjacent target tile position */
Direction Ghost::GetDirection(int row, int col, glm::vec2 adjTargetPos) {
    float threshold = 0.001f;
    float deltaX = adjTargetPos.x - col;
    float deltaY = adjTargetPos.y - row;
    
    if (deltaX >= 0.0f && abs(deltaY) <= threshold) {
        return RIGHT;
    }
    else if (deltaX < 0.0f && abs(deltaY) <= threshold) {
        return LEFT;
    }
    else if (deltaY >= 0.0f && abs(deltaX) <= threshold) {
        return DOWN;
    }
    else if (deltaY < 0.0f && abs(deltaX) <= threshold) {
        return UP;
    }
    else {
        std::cout << "ERROR::Invalid Direction!!" << std::endl;
        return NONE;
    }
}

void Ghost::EnterFrightened(PlayerObject* player) {
    std::cout << "ENTERING FRIGHTENED FUNCTION WITH GHOST "<< this->Name << std::endl;
    TurnAround(this->Dir, this->AdjTargetPos);
    this->HasBeenEatenThisDot = false;
    this->PreviousModeBeforeFrightened = this->CurrentMode;
    // UNDID CHANGE WITH FRIGHTENED
    // if (this->CurrentMode != HOME_IDLE && this->CurrentMode != HOME_EXIT) {
    this->CurrentMode = FRIGHTENED;
    // }

    this->Color = glm::vec3(0.129f, 0.129f, 1.0f);
    this->EyesObj->currSprite = 4;
    this->Velocity = GetSpec("frightenedGhostSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
    //FrightenedTimer = GetSpec("frightenedTime", CurrentLevel);
    FrightenedTimer = player->HuntingTimer;
    // when exiting home, check if firghtened timer is >0. 
    // also make sure tomake frightend timer = 0 when eaten.
    

}

void Ghost::SyncTimings(PlayerObject* player) {
    this->ModeTimer = player->ModeTimerReference;
    this->ModeIdx = player->ModeIdxReference;
    this->CurrentMode = player->GhostModeReference;
}

void Ghost::ExitFrightened(PlayerObject* player) {
    // REMEMBER TO SYNC PLAYER REFERENCE TIMER  to GHOST TIMER when they exit frightened
    this->CurrentMode = this->PreviousModeBeforeFrightened;
    this->Color = GetGhostColor(this->Name);
    this->Velocity = GetSpec("ghostSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
    std::cout << this->Name << " Exit frightened" << std::endl;

    // BUG: does not work if all ghosts are eaten (then, nobody calls this line)
    // player->Velocity = GetSpec("playerSpeed", CurrentLevel) * speedRatio * TILE_SIZE;
    // player->CurrentMode = NORMAL;

    SyncTimings(player);
    
}   

void Ghost::EnterEaten() {
    this->CurrentMode = EATEN;
    this->HasBeenEatenThisDot = true;
    this->Alpha = 0.0f;
    this->Velocity = GetSpec("ghostSpeed", CurrentLevel) * speedRatio * TILE_SIZE * 2.0f;


}
// Remember adj target is in terms of tiles
glm::vec2 Ghost::GetAdjTarget(int row, int col, glm::vec2 ghostSize, Direction currDir, glm::vec2 mapTargetPos, bool isFrightened, float tileSize, std::vector<std::vector<int>>* map) {
    Direction dirs[4] = {LEFT, UP, RIGHT, DOWN};
    Direction randomDir;
    int tile;

    if (InTunnel) {
        return glm::vec2((float)col + dirToVec(this->Dir).x, (float)row + dirToVec(this->Dir).y);
    }
    if (isFrightened) {

        randomDir = dirs[rand() % (sizeof(dirs) / sizeof(dirs[0]))];
        tile = map->at(row+dirToVec(randomDir).y).at(col+dirToVec(randomDir).x);
        
        //std::cout << randomDir << std::endl;

        while (dirToVec(currDir) == -dirToVec(randomDir) || tile == 0) {
            //std::cout << "Current dir is " << currDir << "But rolled opposite dir " << randomDir << "... Retrying" << std::endl;
            randomDir = dirs[rand() % (sizeof(dirs) / sizeof(dirs[0]))];
            tile = map->at(row+dirToVec(randomDir).y).at(col+dirToVec(randomDir).x);

        }
        //std::cout << " returning adj target: " << (float)col + dirToVec(randomDir).x << " " << (float)row + dirToVec(randomDir).y << std::endl;

        return glm::vec2((float)col + dirToVec(randomDir).x, (float)row + dirToVec(randomDir).y);
    }

    Direction closestNodeToMapTarget;
    float minDist = tileSize * 9999.0f;
    for (Direction dir : dirs) {

        glm::vec2 dirVec = dirToVec(dir);
        if (dirVec == -dirToVec(currDir)) {
            //std::cout << "Filtered out direction (backtracking) " << dir << std::endl;
            continue;
        }


        int tile = map->at(row+dirVec.y).at(col+dirVec.x); // this line causes the bug
        if (tile == 0) {
            //std::cout << "Filtered out direction (wall)" << dir << std::endl;
            continue;
        }

        if (InSpecialZone(row, col) && dir == UP) {
            std::cout << this->Name << "IN SPECIAL ZONE at col= " << col << "row = " << row << std::endl;
            continue;
        }
        // TODO: Implement banned moves outlined in notes
        float dist = GetDistance(col + dirVec.x, row + dirVec.y, mapTargetPos.x, mapTargetPos.y);

        if (dist < minDist) {
            closestNodeToMapTarget = dir;
            minDist = dist;
        }
    }

    float adjTargetX = col + dirToVec(closestNodeToMapTarget).x;
    float adjTargetY = row + dirToVec(closestNodeToMapTarget).y;

    return glm::vec2(adjTargetX, adjTargetY);

}

float Ghost::GetDistance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2-x1, 2.0f) + pow(y2-y1,2.0f));
}


Ghost::~Ghost() {
    delete this->EyesObj;
    delete this->DebugMapTarget;
}

bool Ghost::InSpecialZone(int row, int col) {
    return(
        (col == 12 & row == 11) ||
        (col == 15 & row == 11) ||
        (col == 12 & row == 23) ||
        (col == 15 & row == 23)
    );
}