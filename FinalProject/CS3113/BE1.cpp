#include "BE1.h"
#include "Dialoge.h"
#include <iostream>
#include <algorithm>
#include <string>

// BE1::BE1()                                      : Scene { {0.0f}, nullptr,   } {}
BE1::BE1(Vector2 origin, const char *bgHexCode, Music bgMusic) : Scene { origin, bgHexCode, bgMusic } {}

BE1::~BE1() {  }

void BE1::initialise()
{
   
   Scene::initialise();
   mGameState.nextSceneID = -1;

   // sound
   SetMusicVolume(mGameState.bgm, 1.5f);
   mGameState.sounds["stab"] = LoadSound("assets/stabSound.wav");
   mGameState.sounds["scream"] = LoadSound("assets/scream.wav");
   mGameState.sounds["heartbeat"] = LoadSound("assets/heartbeat.wav");
   SetSoundVolume(mGameState.sounds["heartbeat"], 4.0f);
   mGameState.sounds["jumpscare"] = LoadSound("assets/jumpScare.ogg");
   mGameState.sounds["talking"] = LoadSound("assets/talking.wav");
   mGameState.sounds["jumpscare2"] = LoadSound("assets/jumpScare2.wav");

   PlaySound(mGameState.sounds["scream"]);
   /*
      ----------- UI -----------
   */
//    mGameState.UIs["eKey"] = LoadTexture("assets/E.png");

   /*
      ----------- DIALOGES -----------
   */

   /*
      ----------- EFFECT -----------
   */
   mGameState.effect = new Effects(mOrigin, (float) GetScreenWidth() * 2.0f, (float) GetScreenHeight() * 2.0f);
   mGameState.effect->setEffectSpeed(0.15f);
   mGameState.effect->start(FADEIN);

}

Vector2 BE1::getGridPos(int row, int col) {
   return {mGameState.maps[0]->getLeftBoundary() + (float) (row*TILE_DIMENSION), 
      mGameState.maps[0]->getTopBoundary() + (float)(col*TILE_DIMENSION)};
}

void BE1::update(float deltaTime)
{
    // update music 
   if (mGameState.bgm.frameCount > 0) UpdateMusicStream(mGameState.bgm);
}

void BE1::displayEnding() {
    const char* text = "BAD ENDING";
   int length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y - 50, 50, WHITE);
   
   text = "KILLED BY \"INSPECTOR\"";
   length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y + 50, 50, WHITE);
}

void BE1::render()
{
   ClearBackground(BLACK);
   
}

void BE1::renderUI()
{
   Scene::renderUI();
    displayEnding();
   
}

void BE1::displayImage(Texture2D texture, bool displayclosed) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   int instructionWidth = MeasureText("Press E to close", 20);
   if (!displayclosed)
    DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void BE1::shutdown()
{
   Scene::shutdown();
}