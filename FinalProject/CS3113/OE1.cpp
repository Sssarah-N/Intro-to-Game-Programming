#include "OE1.h"
#include "Dialoge.h"
#include <iostream>
#include <algorithm>
#include <string>

// OE1::OE1()                                      : Scene { {0.0f}, nullptr,   } {}
OE1::OE1(Vector2 origin, const char *bgHexCode, Music bgMusic) : Scene { origin, bgHexCode, bgMusic } {}

OE1::~OE1() {  }

void OE1::initialise()
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
      "You escaped from your house.",
      "You cannot go back - who knows if the inspector is waiting for you.",
      "You wander through the streets, and you can't tell who is human and who is mimic",
      "Every face feels wrong",
      "Maybe... you're the only one left."
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

Vector2 OE1::getGridPos(int row, int col) {
   return {mGameState.maps[0]->getLeftBoundary() + (float) (row*TILE_DIMENSION), 
      mGameState.maps[0]->getTopBoundary() + (float)(col*TILE_DIMENSION)};
}

void OE1::update(float deltaTime)
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

void OE1::displayEnding() {
    const char* text = "OPEN ENDING";
   int length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y - 50, 50, WHITE);
   
   text = "WANDERER";
   length = MeasureText(text, 50);
   DrawText(text, mOrigin.x-length/2, mOrigin.y + 50, 50, WHITE);
}

void OE1::render()
{
   ClearBackground(BLACK);
   
}

void OE1::renderUI()
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

void OE1::displayImage(Texture2D texture, bool displayclosed) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   int instructionWidth = MeasureText("Press E to close", 20);
   if (!displayclosed)
    DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void OE1::shutdown()
{
   Scene::shutdown();
}