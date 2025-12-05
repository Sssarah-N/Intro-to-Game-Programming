#include "EndScene.h"
#include <iostream>

EndScene::EndScene()                                      : Scene { {0.0f}, nullptr   } {}
EndScene::EndScene(Vector2 origin, const char *bgHexCode, Music bgm) : Scene { origin, bgHexCode, bgm } {}

EndScene::~EndScene() { }

void EndScene::initialise()
{
   Scene::initialise();
   mGameState.nextSceneID = -1;
}

void EndScene::update(float deltaTime)
{
   if (mGameState.bgm.frameCount > 0) UpdateMusicStream(mGameState.bgm);
}

void EndScene::render()
{
   ClearBackground(BLACK);
   
}

void EndScene::renderUI() {
   const char* text = "TRUE ENDING";
   int length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y - 50, 50, WHITE);
   
   text = "?????";
   length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y + 50, 50, WHITE);
}

void EndScene::shutdown()
{
   Scene::shutdown();
}