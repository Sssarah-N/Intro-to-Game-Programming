/**
* Author: Sarah Niu
* Assignment: Lunar Lander
* Date due: 2025-10-13, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

enum gameStatus { INPROGRESS, WON, LOST };

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char BG_COLOUR[]    = "#87CEEB";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr int   NUMBER_OF_TILES         = 23;
constexpr float TILE_DIMENSION          = 65.0f,
                // in m/ms², since delta time is in ms
                ACCELERATION_OF_GRAVITY = 10.0f,
                FRICTION                = 10.0f,  
                FIXED_TIMESTEP          = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

gameStatus gGameStatus = INPROGRESS;

Entity *gXochitl = nullptr;
Entity *gTiles  = nullptr;
Entity* gFlag = nullptr;
int fuel = 3000;


// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Physics");

    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> animationAtlas = {
        {DOWN,  { 24, 25, 26 }},
        {LEFT,  { 24, 25, 26 }},
        {UP,    { 24, 25, 26 }},
        {RIGHT, { 24, 25, 26 }},
    };

    float sizeRatio  = 48.0f / 64.0f;

    // Assets from @see https://sscary.itch.io/the-adventurer-female
    gXochitl = new Entity(
        {50.0f, 50.0f}, // position
        {70.0f, 70.0f},  // scale
        "assets/game/tilemap-characters.png",        // texture file address
        ATLAS,                         // single image or atlas?
        { 3, 9 },                      // atlas dimensions
        animationAtlas                 // actual atlas
    );

    gXochitl->setEntityType(PLAYER);
    gXochitl->setColliderDimensions({
        gXochitl->getScale().x / 4.0f,
        gXochitl->getScale().y / 4.0f
    });
    gXochitl->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    /*
        ----------- TILES -----------
    */
    gTiles = new Entity[NUMBER_OF_TILES];
    
    
    gFlag = new Entity();

    for (int i = 0; i < NUMBER_OF_TILES-1; i++) 
    {
        // assets from @see https://kenney.nl/assets/pixel-platformer-industrial-expansion

        // target block
        if (i == 10) {
            gTiles[i].setTexture("assets/game/target.png");
            gTiles[i].setEntityType(TARGET);
            gFlag->setTexture("assets/game/flag.png");
            gFlag->setScale({TILE_DIMENSION, TILE_DIMENSION});
            gFlag->setColliderDimensions({TILE_DIMENSION, TILE_DIMENSION});
            gFlag->setPosition({
                i * TILE_DIMENSION, 
                SCREEN_HEIGHT - TILE_DIMENSION*2
            });
        }
        // other tiles
        else {
            gTiles[i].setTexture("assets/game/floor.png");
            gTiles[i].setEntityType(PLATFORM);
        }
        
        gTiles[i].setScale({TILE_DIMENSION, TILE_DIMENSION});
        gTiles[i].setColliderDimensions({TILE_DIMENSION, TILE_DIMENSION});
        gTiles[i].setPosition({
            i * TILE_DIMENSION, 
            SCREEN_HEIGHT - TILE_DIMENSION
        });
        

        
    }

    gTiles[NUMBER_OF_TILES-1].setTexture("assets/game/tile.png");
    gTiles[NUMBER_OF_TILES-1].setScale({TILE_DIMENSION, TILE_DIMENSION});
    gTiles[NUMBER_OF_TILES-1].setColliderDimensions({TILE_DIMENSION, TILE_DIMENSION});
    gTiles[NUMBER_OF_TILES-1].setPosition({ORIGIN.x, ORIGIN.y - TILE_DIMENSION});
    gTiles[NUMBER_OF_TILES-1].moveLeft();
    gTiles[NUMBER_OF_TILES-1].setEntityType(BLOCK);
    gTiles[NUMBER_OF_TILES-1].setSpeed(40);

    SetTargetFPS(FPS);
}

void processInput() 
{
    gXochitl->resetMovement();
    Vector2 acc = gXochitl->getAcceleration();
    
    // set acc for x directions
    if (IsKeyDown(KEY_A) && fuel > 0) {
        gXochitl->setAcceleration({-5, acc.y});
        --fuel;
    }
    else if (IsKeyDown(KEY_D) && fuel > 0) {
        gXochitl->setAcceleration({5, acc.y});
        --fuel;
    }
    else {
        // add friction to stop when key is not pressed
        float currentVelX = gXochitl->getVelocity().x;
        if (currentVelX > 0.1f) {
            gXochitl->setAcceleration({-FRICTION, acc.y});
        } else if (currentVelX < -0.1f) {
            gXochitl->setAcceleration({FRICTION, acc.y});
        }
    }

    // Get acceleration after modifying X
    acc = gXochitl->getAcceleration();
    
    if (IsKeyDown(KEY_W) && fuel > 0) {
        gXochitl->setAcceleration({acc.x, -20});
        --fuel;
    }
    else gXochitl->setAcceleration({acc.x, ACCELERATION_OF_GRAVITY});

    // to avoid faster diagonal speed
    if (GetLength(gXochitl->getMovement()) > 1.0f) 
        gXochitl->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        if (gXochitl->isCollidingBottom()) {
            if (gXochitl->getCollidingType() == TARGET) gGameStatus = WON;
            else gGameStatus = LOST;
        } 
        gXochitl->update(FIXED_TIMESTEP, gTiles, NUMBER_OF_TILES);
        // make sure character doesn't go off screen
        Vector2 currPos = gXochitl->getPosition();
        if (currPos.x < 0) gXochitl->setPosition({0, currPos.y}); 
        else if (currPos.x > SCREEN_WIDTH) gXochitl->setPosition({SCREEN_WIDTH, currPos.y}); 
        currPos = gXochitl->getPosition();
        if (currPos.y < 0) gXochitl->setPosition({currPos.x, 0}); 

        // update moving block
        if (gTiles[NUMBER_OF_TILES-1].getPosition().x >= SCREEN_WIDTH - TILE_DIMENSION) 
            gTiles[NUMBER_OF_TILES-1].moveLeft();
        
        if (gTiles[NUMBER_OF_TILES-1].getPosition().x <= TILE_DIMENSION)
            gTiles[NUMBER_OF_TILES-1].moveRight();

        // update all tiles
        for (int i = 0; i < NUMBER_OF_TILES; i++) 
            gTiles[i].update(FIXED_TIMESTEP, nullptr, 0);

        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    
    if (gGameStatus == INPROGRESS) {
        std::string fuelText = "Fuel left: " + std::to_string(fuel);
        const char* text = fuelText.c_str();
        DrawText(text, SCREEN_WIDTH-250, 10, 20, BLACK);

        std::string xVelocity = "Horizontal speed: " + std::to_string(static_cast<int>(gXochitl->getVelocity().x));
        const char* xVelocitytext = xVelocity.c_str();
        DrawText(xVelocitytext, SCREEN_WIDTH-250, 50, 20, BLACK);

        std::string yVelocity = "Vertical speed: " + std::to_string(static_cast<int>(gXochitl->getVelocity().y));
        const char* yVelocitytext = yVelocity.c_str();
        DrawText(yVelocitytext, SCREEN_WIDTH-250, 90, 20, BLACK);
        
        gFlag->render();
        gXochitl->render();
        // gBlock->render();
        for (int i = 0; i < NUMBER_OF_TILES; i++) gTiles[i].render();
    }
    else if (gGameStatus == LOST) {
        const char* text = "Mission Failed";
        DrawText(text, 250, ORIGIN.y-35, 70, RAYWHITE);
    }
    else {
        //std::string fuelText = "Fuel left: " + std::to_string(fuel);
        const char* text = "Mission Accomplished";
        DrawText(text, 140, ORIGIN.y-35, 70, RAYWHITE);
    }

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}