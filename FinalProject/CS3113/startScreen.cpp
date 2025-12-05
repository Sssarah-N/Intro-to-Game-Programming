#include "StartScreen.h"
#include <iostream>

StartScreen::StartScreen()                                      : Scene { {0.0f}, nullptr   } {}
StartScreen::StartScreen(Vector2 origin, const char *bgHexCode, Music bgm) : Scene { origin, bgHexCode, bgm } {}

StartScreen::~StartScreen() {  }

void StartScreen::initialise()
{
   Scene::initialise();
   mGameState.nextSceneID = -1;
}

void StartScreen::update(float deltaTime)
{
   if (IsKeyPressed(KEY_ENTER)) mGameState.nextSceneID = 0;
    // update music 
    if (mGameState.bgm.frameCount > 0) UpdateMusicStream(mGameState.bgm);
}

void StartScreen::render()
{
   ClearBackground(BLACK);
}

void StartScreen::renderUI() {
   Scene::renderUI();
   const char* text = "THREE DAYS";
   int length = MeasureText(text, 60);
   DrawText(text, mOrigin.x-length/2, mOrigin.y - 100, 60, WHITE);
   
   text = "WASD - move";
   length = MeasureText(text, 35);
   DrawText(text, mOrigin.x-length/2, mOrigin.y, 35, WHITE);
   
   text = "SPACE - skip line/next line";
   length = MeasureText(text, 35);
   DrawText(text, mOrigin.x-length/2, mOrigin.y + 50, 35, WHITE);
   
   text = "E - interact";
   length = MeasureText(text, 35);
   DrawText(text, mOrigin.x-length/2, mOrigin.y + 100, 35, WHITE);
   
   text = "Press Enter to Start";
   length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y + 150, 50, WHITE);
}

void StartScreen::shutdown()
{ 
   Scene::shutdown();
}