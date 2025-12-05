#include "OE2.h"
#include "Dialoge.h"
#include <iostream>
#include <algorithm>
#include <string>

// OE2::OE2()                                      : Scene { {0.0f}, nullptr,   } {}
OE2::OE2(Vector2 origin, const char *bgHexCode, Music bgMusic) : Scene { origin, bgHexCode, bgMusic } {}

OE2::~OE2() {  }

void OE2::initialise()
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
   /*
      ----------- UI -----------
   */
//    mGameState.UIs["eKey"] = LoadTexture("assets/E.png");

   /*
      ----------- DIALOGES -----------
   */
   // opening
   std::vector<std::string> lines = {
      "We sit at the table and smile like nothing is wrong",
      "Their eyes are getter more red, but I pretend not to notice.",
      "If I act normal, maybe they will stay the way they are.",
      "Maybe they are still my parents, maybe not.",
      "Either way, we are going to stay together happily forever."
   };

   Dialoge* dialoge = new Dialoge(lines, "character");
   mGameState.dialogues["OE"] = dialoge;
   mGameState.activeDialogue = dialoge;

   /*
      ----------- EFFECT -----------
   */
   mGameState.effect = new Effects(mOrigin, (float) GetScreenWidth() * 2.0f, (float) GetScreenHeight() * 2.0f);
   mGameState.effect->setEffectSpeed(0.15f);
   mGameState.effect->start(FADEIN);

}

Vector2 OE2::getGridPos(int row, int col) {
   return {mGameState.maps[0]->getLeftBoundary() + (float) (row*TILE_DIMENSION), 
      mGameState.maps[0]->getTopBoundary() + (float)(col*TILE_DIMENSION)};
}

void OE2::update(float deltaTime)
{
    // update music 
   if (mGameState.bgm.frameCount > 0) UpdateMusicStream(mGameState.bgm);
   
   // update effect
   mGameState.effect->update(deltaTime, nullptr);
   
    // if (mGameState.activeDialogue->isDialogueComplete())
   // update dialoges
   if (mGameState.activeDialogue) {
      mGameState.activeDialogue->update(deltaTime);
      if (IsKeyPressed(KEY_SPACE)) {
         if (!mGameState.activeDialogue->isLineComplete()) {
            mGameState.activeDialogue->skipTyping();  
         } else {
            mGameState.activeDialogue->nextLine(); 
         }
     }

    
   }
}

void OE2::displayEnding() {
    const char* text = "OPEN ENDING";
   int length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y - 50, 50, WHITE);
   
   text = "HAPPY FAMILY FOREVER";
   length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y + 50, 50, WHITE);
}

void OE2::render()
{
   ClearBackground(BLACK);
   
}

void OE2::renderUI()
{
   Scene::renderUI();
   
  // display dialoge
  if (mGameState.activeDialogue) {
    printf("redering dialogue\n");
      mGameState.activeDialogue->render();
   }

   mGameState.effect->render();

   if (mGameState.dialogues["OE"]->isDialogueComplete() )  {
        mGameState.activeDialogue = nullptr;
        displayEnding();
    }
   
}

void OE2::displayImage(Texture2D texture, bool displayclosed) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   int instructionWidth = MeasureText("Press E to close", 20);
   if (!displayclosed)
    DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void OE2::shutdown()
{
   Scene::shutdown();
}