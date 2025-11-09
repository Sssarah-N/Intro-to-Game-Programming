#include "WonScene.h"
#include <iostream>

WonScene::WonScene()                                      : Scene { {0.0f}, nullptr   } {}
WonScene::WonScene(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

WonScene::~WonScene() { }

void WonScene::initialise()
{
   Scene::initialise();
   mGameState.nextSceneID = -1;
}

void WonScene::update(float deltaTime)
{
   if (IsKeyPressed(KEY_R)) mGameState.nextSceneID = 3;
}

void WonScene::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   
}

void WonScene::renderUI() {
   Scene::renderUI();
   const char* text = "\t\t\t\tYOU WON\n\n  Press R to Restart";
   DrawText(text, 100, mOrigin.y-80, 70, ColorFromHex("#002e53ff"));
}

void WonScene::shutdown()
{
   Scene::shutdown();
}