#include "LevelA.h"
#include "Dialoge.h"
#include <iostream>
#include <algorithm>

// LevelA::LevelA()                                      : Scene { {0.0f}, nullptr,   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode, Music bgMusic) : Scene { origin, bgHexCode, bgMusic } {}

LevelA::~LevelA() {  }

void LevelA::initialise()
{
   
   Scene::initialise();
   mGameState.nextSceneID = -1;

   /*
      ----------- MAP -----------
   */
   Map* map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData1, // grid data
      "assets/interior.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      48, 16,                        // texture cols & rows
      mOrigin                      // in-game origin
   );
   mGameState.maps.push_back(map);

   map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData2, // grid data
      "assets/interior.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      48, 16,                        // texture cols & rows
      mOrigin                      // in-game origin
   );
   mGameState.maps.push_back(map);

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> xochitlWalkAtlas = {
      {DOWN,  {  0, 1, 2}},
      {LEFT,  {  3, 4, 5}},
      {RIGHT, {  6, 7, 8}},
      {UP,    {  9, 10, 11}},
   };

   float sizeRatio  = 48.0f / 64.0f;

   mGameState.xochitl = new Entity(
      getGridPos(3, 20), // position
      {100.0f, 100.0f},             // scale
      "assets/character.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 4, 3 },                                 // atlas dimensions
      xochitlWalkAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.xochitl->setSpeed(150);
   mGameState.xochitl->setName("character");
   mGameState.xochitl->setColliderDimensions({
      mGameState.xochitl->getScale().x / 3.0f,
      mGameState.xochitl->getScale().y / 3.0f
   });
   mGameState.xochitl->render(); 


   /*
      ----------- MOM -----------
   */
   std::map<Direction, std::vector<int>> momWalkAtlas = {
      {DOWN,  {  0, 1, 2}},
      {LEFT,  {  3, 4, 5}},
      {RIGHT, {  6, 7, 8}},
      {UP,    {  9, 10, 11}},
   };

   Entity* mom = new Entity(
      getGridPos(10, 10), // position
      {100.0f, 100.0f},             // scale
      "assets/mom.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 4, 3 },                                 // atlas dimensions
      momWalkAtlas,                    // actual atlas
      NPC                                    // entity type
   );

   mom->setSpeed(20);
   mom->setName("mom");
   mom->setColliderDimensions({
      mom->getScale().x / 3.0f,
      mom->getScale().y / 3.0f
   });
   mom->setAIType(WANDERER);
   mom->setAIState(WALKING);
   mom->render(); 
   displayingMom = false;
   mGameState.collidableEntities.insert({"mom", mom});

   /*
      ----------- DAD -----------
   */
  std::map<Direction, std::vector<int>> dadWalkAtlas = {
   {DOWN,  {  0, 1, 2}},
   {LEFT,  {  3, 4, 5}},
   {RIGHT, {  6, 7, 8}},
   {UP,    {  9, 10, 11}},
   };

   Entity* dad = new Entity(
      getGridPos(10, 7), // position
      // {mGameState.maps[0]->getLeftBoundary()+TILE_DIMENSION, mGameState.maps[0]->getBottomBoundary()-TILE_DIMENSION}, // position
      {100.0f, 100.0f},             // scale
      "assets/dad.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 4, 3 },                                 // atlas dimensions
      dadWalkAtlas,                    // actual atlas
      NPC                                    // entity type
   );

   dad->setSpeed(20);
   dad->setName("dad");
   dad->setColliderDimensions({
      dad->getScale().x / 3.0f,
      dad->getScale().y / 3.0f
   });
   dad->setAIType(WANDERER);
   dad->setAIState(WALKING);
   dad->render(); 
   displayingDad = false;
   mGameState.collidableEntities.insert({"dad", dad});


   /*
      ----------- INTERACTABLES -----------
   */

   //main door
  Entity* maindoor = new Entity {
      {mGameState.maps[0]->getLeftBoundary() + 12.5f*TILE_DIMENSION, 
         mGameState.maps[0]->getTopBoundary() + 5*TILE_DIMENSION},
      {TILE_DIMENSION, 2*TILE_DIMENSION},
      "assets/door_closed.png",
      BLOCK  
   };
   maindoor->setColliderDimensions({
      maindoor->getScale().x,
      maindoor->getScale().y/4.0f
   });
   maindoor->setName("maindoor");
   maindoor->setInteractable(true);
   mGameState.collidableEntities.insert({"maindoor", maindoor});


   // door
   Entity* door = new Entity {
      {mGameState.maps[0]->getLeftBoundary() + 7.5f*TILE_DIMENSION, 
         mGameState.maps[0]->getTopBoundary() + 16*TILE_DIMENSION},
      {TILE_DIMENSION, 2*TILE_DIMENSION},
      "assets/door_closed.png",
      BLOCK  
   };
   door->setColliderDimensions({
      door->getScale().x,
      door->getScale().y / 2.5f
   });
   door->setName("door");
   // door->setInteractable(false);
   mGameState.collidableEntities.insert({"door", door});


   // TV
   Entity* TV = new Entity {
      {mGameState.maps[0]->getLeftBoundary() + 2.0f*TILE_DIMENSION, 
         mGameState.maps[0]->getTopBoundary() + 17*TILE_DIMENSION},
      {TILE_DIMENSION, 2*TILE_DIMENSION},
      "assets/TV.png",
      BLOCK  
   };
   TV->setColliderDimensions({
      TV->getScale().x,
      TV->getScale().y / 2.0f
   });
   TV->setName("TV");
   
   mGameState.collidableEntities.insert({"TV", TV});


   // Board
   Entity* board = new Entity {
      {mGameState.maps[0]->getLeftBoundary() + 4.5f*TILE_DIMENSION, 
         mGameState.maps[0]->getTopBoundary() + 17*TILE_DIMENSION},
      {2*TILE_DIMENSION, 2*TILE_DIMENSION},
      "assets/board.png",
      BLOCK  
   };
   board->setColliderDimensions({
      board->getScale().x,
      board->getScale().y / 2.0f
   });
   board->setName("board");
   board->setInteractable(false);
   
   mGameState.collidableEntities.insert({"board", board});
   displayingBoard = false;


   // bed
   Entity* bed = new Entity {
      {mGameState.maps[0]->getLeftBoundary() + 1.5f*TILE_DIMENSION, 
         mGameState.maps[0]->getTopBoundary() + 21*TILE_DIMENSION+11},
      {TILE_DIMENSION, 2*TILE_DIMENSION},
      "assets/bed.png",
      BLOCK  
   };
   bed->setInteractable(false);
   bed->setName("bed");
   
   mGameState.collidableEntities.insert({"bed", bed});


   /*
      ----------- UI -----------
   */
   mGameState.UIs["eKey"] = LoadTexture("assets/E.png");
   mGameState.UIs["TVEffect"] = LoadTexture("assets/TVEffect.png");
   mGameState.UIs["board"] = LoadTexture("assets/boardrules.png");
   mGameState.UIs["mom"] = LoadTexture("assets/momProtrait.png");
   mGameState.UIs["dad"] = LoadTexture("assets/dadPortrait.png");
   
   // TV text
   TVstates.tvText = "1. Observe if people have a natural facial expression \nwhen they talk or smile\n\n"
                     "2. Make sure to lock every door and window at \nnight time\n\n"
                     "3. Government inspectors wearing BLUE uniform are here \nto help you, ask for help if you suspect there is a mimic\n\n"
                     "4. If you suspect someone is a mimic, question them \nabout a private memory\n\n"
                     "5. The eyes of a mimic is more red compared to human\n\n"
                     "6. If you want to test if someone has been converted, \ncut on a subject's arm. A mimic's wound will close rapidly";

   /*
      ----------- DIALOGES -----------
   */
   // opening
   std::vector<std::string> lines = {
      "It's just another day...",
      "I should probably check the TV"
   };

   Dialoge* dialoge = new Dialoge(lines, "character");
   mGameState.activeDialogue = dialoge;
   mGameState.dialogues["opening"] = dialoge;

   // news
   lines = {
      "URGENT ALERT",
      "Good morning. Officials have confirmed the presence of entities known as Mimics within several residential areas.",
      "Mimics are capable of replicating human appearance, voice, and daily habits with unnerving precision.",
      "Although they look identical, mimics often struggle with natural emotional responses and subtle social cues.",
      "Witnesses report brief delays in speech, unusual posture, or reactions that feel slightly 'out of sync'.",
      "These mimics could be a stranger on the road, or your family and friends.",
      "Please read and strictly follow the following guidelines, report any suspicious entities as soon as possible."
   };

   dialoge = new Dialoge(lines, "News");
   mGameState.dialogues["News"] = dialoge;

   lines = {
      "...",
      "Is this actually happening?",
      "Anyways, I should jot the guidelines down on the board."
   };

   dialoge = new Dialoge(lines, "character");
   mGameState.dialogues["afterNews"] = dialoge;


   // mom1
   lines = {
      "Good morning sweet heart, slept well?",
      "Did you see the news this morning? I know it is concerning.",
      "At least we are safe in our home."
   };

   dialoge = new Dialoge(lines, "Mom");
   mGameState.dialogues["mom_morning1"] = dialoge;

   lines = {
      "Did mom look like this before?",
      "I must be overthinking"
   };

   dialoge = new Dialoge(lines, "character");
   mGameState.dialogues["mom_morning2"] = dialoge;


   // dad1
   lines = {
      "You heard about the news in the morning?",
      "Don't worry, we are all save at home"
   };

   dialoge = new Dialoge(lines, "Dad");
   mGameState.dialogues["dad_morning1"] = dialoge;

   lines = {
      "I'm ok, just a little tired"
   };

   dialoge = new Dialoge(lines, "Dad");
   mGameState.dialogues["dad_morning2"] = dialoge;

   

   // maindoor
   lines = {
      "There are a few people wandering on the street.",
      "But are they actually humans...",
      "Probably not a good idea to go outside."
   };

   dialoge = new Dialoge(lines, "character");
   mGameState.dialogues["maindoor"] = dialoge;


   // bed
   lines = {
      "Today has been a little weird",
      "I've already locked my door, and I will go to bed for now.",
      "Hopefully everything will be fine tomorrow."
   };

   dialoge = new Dialoge(lines, "character");
   mGameState.dialogues["bed"] = dialoge;



   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.xochitl->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom

   mGameState.effect = new Effects(mOrigin, (float) GetScreenWidth() * 2.0f, (float) GetScreenHeight() * 2.0f);
   mGameState.effect->setEffectSpeed(0.15f);
   mGameState.effect->start(FADEIN);
}

Vector2 LevelA::getGridPos(int row, int col) {
   return {mGameState.maps[0]->getLeftBoundary() + row*TILE_DIMENSION, 
      mGameState.maps[0]->getTopBoundary() +col*TILE_DIMENSION};
      
}

void LevelA::update(float deltaTime)
{
   if (mGameState.bgm.frameCount > 0) UpdateMusicStream(mGameState.bgm);
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
     if (mGameState.activeDialogue->isDialogueComplete() )  {
      if (mGameState.activeDialogue == mGameState.dialogues["News"]) {
         TVstates.startDisplay = true;
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["mom_morning1"]) {
         mGameState.activeChoice = new ChoiceDialogue("mom_morning", {"Observe"});
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["dad_morning1"]) {
         mGameState.activeChoice = new ChoiceDialogue("dad_morning", {"Observe"});
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["dad_morning2"]) {
         mGameState.collidableEntities["dad"]->setAIState(WALKING);
      }
      
      mGameState.activeDialogue = nullptr;
     }
   }

   mGameState.xochitl->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.maps, // map
      mGameState.collidableEntities              // col. entity count
   );

   mGameState.collidableEntities["mom"]->update(deltaTime, mGameState.xochitl, mGameState.maps, mGameState.collidableEntities);
   mGameState.collidableEntities["dad"]->update(deltaTime, mGameState.xochitl, mGameState.maps, mGameState.collidableEntities);

   Vector2 charPos = mGameState.xochitl->getPosition();
   Vector2 doorpos = mGameState.collidableEntities["door"]->getPosition();
   
   if (isAround(mGameState.collidableEntities["door"]) && IsKeyPressed(KEY_E)
         && mGameState.collidableEntities["door"]->isInteractable()) {
      if (charPos.y > doorpos.y) {
         mGameState.xochitl->setPosition({doorpos.x, doorpos.y - TILE_DIMENSION});
      }
      else {
         mGameState.xochitl->setPosition({doorpos.x, doorpos.y + 2.0f*TILE_DIMENSION});
      }
      PlaySound(mGameState.sounds["doorSound"]);
   }

   // TV interaction
   if (isAround(mGameState.collidableEntities["TV"]) && IsKeyPressed(KEY_E)
          && !TVstates.displayingTV && mGameState.collidableEntities["TV"]->isInteractable()) {
      PlaySound(mGameState.sounds["TVSound"]);
      TVstates.displayingTV = true;
      TVstates.tvRevealedChars = 0;
      TVstates.tvTypingTimer = 0.0f;
      TVstates.tvTextFullyRevealed = false;
      mGameState.activeDialogue = mGameState.dialogues["News"];
   }
   
   // Update TV typing effect
   if (TVstates.displayingTV && TVstates.startDisplay) {
      if (!TVstates.tvTextFullyRevealed) {
         TVstates.tvTypingTimer += deltaTime;
         float charsToReveal = TVstates.tvTypingTimer * TVstates.tvCharsPerSecond;
         TVstates.tvRevealedChars = (size_t)charsToReveal;
         
         if (TVstates.tvRevealedChars >= TVstates.tvText.length()) {
            TVstates.tvRevealedChars = TVstates.tvText.length();
            TVstates.tvTextFullyRevealed = true;
         }
      }
      
      if (IsKeyPressed(KEY_E) && !TVstates.tvTextFullyRevealed) {
            TVstates.tvRevealedChars = TVstates.tvText.length();
            TVstates.tvTextFullyRevealed = true;
      }
      else if (IsKeyPressed(KEY_E) && TVstates.tvTextFullyRevealed) {
         TVstates.displayingTV = false;
         StopSound(mGameState.sounds["TVSound"]);
         mGameState.collidableEntities["TV"]->setInteractable(false);
         mGameState.collidableEntities["board"]->setInteractable(true);
         mGameState.collidableEntities["door"]->setInteractable(true);
         mGameState.activeDialogue = mGameState.dialogues["afterNews"];
      }
   }

   if (isAround(mGameState.collidableEntities["board"]) && mGameState.collidableEntities["board"]->isInteractable()
         && IsKeyPressed(KEY_E) && !displayingBoard ) {
      displayingBoard = true;
      PlaySound(mGameState.sounds["interactSound"]);
   }
   else if (displayingBoard && IsKeyPressed(KEY_E)) {
      displayingBoard = false;
   }


   // mom interaction
   if (isAround(mGameState.collidableEntities["mom"]) && mGameState.collidableEntities["mom"]->isInteractable()
         && IsKeyPressed(KEY_E) && !mGameState.dialogues["mom_morning1"]->isDialogueComplete()) {
      mGameState.activeDialogue = mGameState.dialogues["mom_morning1"];
      mGameState.collidableEntities["mom"]->setAIState(IDLE);
      mGameState.collidableEntities["mom"]->setInteractable(false);
   }
   

   if (displayingMom && IsKeyPressed(KEY_E)) {
      displayingMom= false;
      mGameState.collidableEntities["mom"]->setAIState(WALKING);
      mGameState.activeDialogue = mGameState.dialogues["mom_morning2"];
   }


   // dad interaction
   if (isAround(mGameState.collidableEntities["dad"]) && mGameState.collidableEntities["dad"]->isInteractable()
         && IsKeyPressed(KEY_E) && !mGameState.dialogues["dad_morning1"]->isDialogueComplete()) {
      mGameState.activeDialogue = mGameState.dialogues["dad_morning1"];
      mGameState.collidableEntities["dad"]->setAIState(IDLE);
      mGameState.collidableEntities["dad"]->setInteractable(false);
   }
   

   if (displayingDad && IsKeyPressed(KEY_E)) {
      displayingDad= false;
      // mGameState.activeDialogue = mGameState.dialogues["mom_morning2"];
      mGameState.activeChoice = new ChoiceDialogue("dad_morning2", {"Are you ok? Your eyes are kind of red"});
   }

   if (isAround(mGameState.collidableEntities["maindoor"]) && mGameState.collidableEntities["maindoor"]->isInteractable()
         && IsKeyPressed(KEY_E)) {
      mGameState.activeDialogue = mGameState.dialogues["maindoor"];
      PlaySound(mGameState.sounds["doorSound"]);
   }

   if (mGameState.dialogues["mom_morning2"]->isDialogueComplete() && mGameState.dialogues["dad_morning2"]->isDialogueComplete()
               && mGameState.dialogues["maindoor"]->isDialogueComplete()) {
         mGameState.collidableEntities["bed"]->setInteractable(true);
      }

   if (isAround(mGameState.collidableEntities["bed"]) && mGameState.collidableEntities["bed"]->isInteractable()
         && IsKeyPressed(KEY_E)) {
      mGameState.activeDialogue = mGameState.dialogues["bed"];
   }

   
   // Handle choices
   if (mGameState.activeChoice) {
      mGameState.activeChoice->handleInput();
      
      if (mGameState.activeChoice->hasSelectedChoice()) {
         int choice = mGameState.activeChoice->getSelectedChoice();
         if (mGameState.activeChoice->getName() == "mom_morning") {
            if (choice == 0) {
               displayingMom = true;
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
         else if (mGameState.activeChoice->getName() == "dad_morning") {
            if (choice == 0) {
               displayingDad = true;
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
         else if (mGameState.activeChoice->getName() == "dad_morning2") {
            if (choice == 0) {
               mGameState.activeDialogue = mGameState.dialogues["dad_morning2"];
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
            mGameState.activeDialogue = mGameState.dialogues["dad_morning2"];
         }
      }
   }

   mGameState.effect->update(deltaTime, nullptr);

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mGameState.xochitl->getPosition().y };

   // finished day, move to next day
   if (mGameState.dialogues["bed"]->isDialogueComplete() && !mGameState.effectStarted)  {
      printf("bed dialogue ended\n");
      mGameState.effect->start(FADEOUT);
      mGameState.effectStarted = true;
   }   

   if (mGameState.effectStarted && mGameState.effect->isEffectComplete()) {
      // printf("switched scene");
      mGameState.nextSceneID = 1;
      // mGameState.shaderEffect = 3;
   }


   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelA::render()
{
   // ClearBackground(ColorFromHex(mBGColourHexCode));
   for (size_t i = 0; i < mGameState.maps.size(); ++i) mGameState.maps[i]->render();
   
   for (const auto& pair : mGameState.collidableEntities)
   {
      pair.second->render();
      if (isAround(pair.second) && pair.second->isInteractable()) {
         drawUI(mGameState.UIs["eKey"], pair.second->getPosition(), 2.2f, 230);
      }
   }

   Entity* mom = mGameState.collidableEntities["mom"];
   Entity* dad = mGameState.collidableEntities["dad"];
   Entity* xochitl = mGameState.xochitl;
   
   std::vector<Entity*> sortedEntities = {mom, dad, xochitl};
   std::sort(sortedEntities.begin(), sortedEntities.end(), [](Entity* a, Entity* b) {
      return a->getPosition().y < b->getPosition().y;
   });
   
   for (Entity* entity : sortedEntities) {
      entity->render();
      if ((entity->getName() == "mom" || entity->getName() == "dad") && isAround(entity) && entity->isInteractable())
         drawUI(mGameState.UIs["eKey"], entity->getPosition(), 2.2f, 230);
   }
   
   
}

void LevelA::renderUI()
{
   Scene::renderUI();
   
   // displayTV
  if (TVstates.displayingTV) displayTV();
  if (displayingBoard) displayBoard();
  if (displayingMom) displayImage(mGameState.UIs["mom"]);
  if (displayingDad) displayImage(mGameState.UIs["dad"]);

  // display dialoge
  if (mGameState.activeDialogue) {
      mGameState.activeDialogue->render();
   }

  // display choice dialogue
  if (mGameState.activeChoice) {
      mGameState.activeChoice->render();
   }

   mGameState.effect->render();
   
}

bool LevelA::isAround(Entity* entity) {
   Vector2 charPos = mGameState.xochitl->getPosition();
   charPos = {
      charPos.x-mGameState.xochitl->getColliderDimensions().x/2.0f, 
      charPos.y-mGameState.xochitl->getColliderDimensions().y/2.0f, 
   };
   Vector2 entityPos = entity->getPosition();
   entityPos = {
      entityPos.x-entity->getColliderDimensions().x/2.0f, 
      entityPos.y-entity->getColliderDimensions().y/2.0f, 
   };

   return (abs(entityPos.x - charPos.x) <= TILE_DIMENSION*1.3f) && (abs(entityPos.y - charPos.y) <= TILE_DIMENSION*1.3f);
}

void LevelA::drawUI(Texture2D texture, Vector2 pos, float scale, unsigned char opacity) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {pos.x-texture.width*scale/2.0f, pos.y-texture.height*scale/2.0f, 
      (float)texture.width * scale, (float)texture.height * scale};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, {255, 255, 255, opacity});
}

void LevelA::displayTV() {
   Rectangle source = { 0, 0, (float)mGameState.UIs["TVEffect"].width, (float)mGameState.UIs["TVEffect"].height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(mGameState.UIs["TVEffect"], source, dest, {0.0f, 0.0f}, 0.0f, WHITE);

   // Get only the revealed portion of the text
   std::string revealedText = TVstates.tvText.substr(0, TVstates.tvRevealedChars);
   
   // Text rendering settings
   int fontSize = 26;
   float padding = 150.0f;
   float startX = padding;
   float startY = 50.0f;
   
   // Draw text directly (DrawText supports \n)
   DrawText(revealedText.c_str(), (int)startX, (int)startY, fontSize, ColorFromHex("#7d0a0a"));
   
   // Show instruction at bottom
   if (TVstates.tvTextFullyRevealed) {
      int instructionWidth = MeasureText("Press E to close", 20);
      DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
   }
}

void LevelA::displayBoard() {
   Rectangle source = { 0, 0, (float)mGameState.UIs["board"].width, (float)mGameState.UIs["board"].height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(mGameState.UIs["board"], source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   DrawText(TVstates.tvText.c_str(), 150, 100, 20, BLACK);
   int instructionWidth = MeasureText("Press E to close", 20);
   DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void LevelA::displayImage(Texture2D texture) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   int instructionWidth = MeasureText("Press E to close", 20);
   DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void LevelA::shutdown()
{
   Scene::shutdown();
}