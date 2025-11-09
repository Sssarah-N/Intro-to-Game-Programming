#include "LoseScene.h"
#include <iostream>

LoseScene::LoseScene()                                      : Scene { {0.0f}, nullptr   } {}
LoseScene::LoseScene(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LoseScene::~LoseScene() { }

void LoseScene::initialise()
{
   Scene::initialise();
   mGameState.nextSceneID = -1;
}

void LoseScene::update(float deltaTime)
{
   if (IsKeyPressed(KEY_R)) mGameState.nextSceneID = 3;
}

void LoseScene::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   
}

void LoseScene::renderUI() {
   Scene::renderUI();
   const char* text = "\t\t\t\tYOU LOST\n\n  Press R to Restart";
   DrawText(text, 100, mOrigin.y-80, 70, ColorFromHex("#002e53ff"));
}

void LoseScene::shutdown()
{
   Scene::shutdown();
}