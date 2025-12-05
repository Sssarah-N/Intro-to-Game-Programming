/**
* Author: Sarah Niu
* Assignment: Three days
* Date due: 12/5, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

/*
    Levels:
    0. Day 1
    1. Day 2
    2. Day 3
    3. Start screen
    4. Opening ending1: escaped from home, what now?
    5. bad ending1: killed by mimic
    6. opening ending2: chose to trust parents, fake happy ending
    7. end screen
*/

#include "CS3113/LevelC.h"
#include "CS3113/startScreen.h"
#include "CS3113/OE1.h"
#include "CS3113/BE1.h"
#include "CS3113/OE2.h"
#include "CS3113/EndScene.h"
#include "CS3113/Effects.h"

// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120,
              NUMBER_OF_LEVELS = 3;

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};
Effects* gEffect = nullptr;

LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;
StartScreen *gStartScreen = nullptr;
OE1 *gOE1 = nullptr;
BE1 *gBE1 = nullptr;
OE2 *gOE2 = nullptr;
EndScene* gEndScene = nullptr;

Music bgm;
Sound levelCompletedSound = LoadSound("assets/levelCompleted.wav");

// Function Declarations
void switchToScene(Scene *scene, bool levelSwitch);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene, bool levelSwitch)
{
    if (gCurrentScene != nullptr) gCurrentScene->shutdown();
    
    gCurrentScene = scene;
    gCurrentScene->initialise();
}


void initialiseScene() {
    
    gLevelA = new LevelA(ORIGIN, "#C0897E", LoadMusicStream("assets/SCP-x7x.mp3"));
    gLevelB = new LevelB(ORIGIN, "#C0897E", LoadMusicStream("assets/SCP-x7x.mp3"));
    gLevelC = new LevelC(ORIGIN, "#C0897E", LoadMusicStream("assets/SCP-x1x.mp3"));
    gStartScreen = new StartScreen(ORIGIN, "#000000", LoadMusicStream("assets/SCP-x7x.mp3"));
    gOE1 = new OE1(ORIGIN, "#000000", LoadMusicStream("assets/SCP-x7x.mp3"));
    gBE1 = new BE1(ORIGIN, "#000000", LoadMusicStream("assets/SCP-x7x.mp3"));
    gOE2 = new OE2(ORIGIN, "#000000", LoadMusicStream("assets/SCP-x7x.mp3"));
    gEndScene = new EndScene(ORIGIN, "#000000", LoadMusicStream("assets/SCP-x7x.mp3"));

    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    gLevels.push_back(gStartScreen);
    gLevels.push_back(gOE1);
    gLevels.push_back(gBE1);
    gLevels.push_back(gOE2);
    gLevels.push_back(gEndScene);

    switchToScene(gLevels[3], false);
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();
    initialiseScene();
    SetTargetFPS(FPS);
    gEffect = new Effects(ORIGIN, (float) GetScreenWidth() * 1.5f, (float) GetScreenHeight() * 1.5f);
}


void processInput() 
{
    if (gCurrentScene->getState().xochitl != nullptr)
    {
        gCurrentScene->getState().xochitl->resetMovement();
        if      (IsKeyDown(KEY_A)) gCurrentScene->getState().xochitl->moveLeft();
        else if (IsKeyDown(KEY_D)) gCurrentScene->getState().xochitl->moveRight();
        if      (IsKeyDown(KEY_W)) gCurrentScene->getState().xochitl->moveUp();
        else if (IsKeyDown(KEY_S)) gCurrentScene->getState().xochitl->moveDown();

    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{

    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        // UpdateMusicStream(bgm);
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
}
void render()
{
    BeginDrawing();
    ClearBackground(BLACK);
    
    
    if (gCurrentScene->getState().xochitl != nullptr) {
        BeginMode2D(gCurrentScene->getState().camera);
    }

    gCurrentScene->render();  

    if (gCurrentScene->getState().xochitl != nullptr) {
        EndMode2D();
    }
    gCurrentScene->renderUI(); 
    
    
    
    EndDrawing();
}

void shutdown() 
{
    for (size_t i = 0; i < gLevels.size(); ++i) delete gLevels[i];
    gLevels.clear();
    // UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID >= 0)
        {
            int id = gCurrentScene->getState().nextSceneID;
            // if (gLevels[id] == gCurrentScene) PlaySound(levelCompletedSound);
            if (id == 3) {
                gCurrentScene = nullptr;
                for (size_t i = 0; i < gLevels.size(); ++i) delete gLevels[i];
                gLevels.clear();
                initialiseScene();
                continue;
            }
            else if (id == 1 || id ==2) switchToScene(gLevels[id], true);
            else switchToScene(gLevels[id], false);
        }

        render();
    }

    shutdown();

    return 0;
}