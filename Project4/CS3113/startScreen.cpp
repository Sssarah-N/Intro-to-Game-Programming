#include "StartScreen.h"
#include <iostream>

StartScreen::StartScreen()                                      : Scene { {0.0f}, nullptr   } {}
StartScreen::StartScreen(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

StartScreen::~StartScreen() {  }

void StartScreen::initialise()
{
   Scene::initialise();
   mGameState.nextSceneID = -1;
}

void StartScreen::update(float deltaTime)
{
   if (IsKeyPressed(KEY_ENTER)) mGameState.nextSceneID = 0;
}

void StartScreen::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   
}

void StartScreen::renderUI() {
   Scene::renderUI();
   const char* text = "Press Enter to Start";
   DrawText(text, mOrigin.x-5.5*70, mOrigin.y-70, 70, ColorFromHex("#002e53ff"));
   text = "Your mission is to grab the key\n      and get to the lock\n\n      Left click to attack\n\n";
   DrawText(text, mOrigin.x - 300, mOrigin.y+50, 40, ColorFromHex("#002e53ff"));
}

void StartScreen::shutdown()
{ 
   Scene::shutdown();
}