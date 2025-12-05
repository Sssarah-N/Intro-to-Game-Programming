#include "LevelC.h"
#include "Dialoge.h"
#include <iostream>
#include <algorithm>
#include <string>

// LevelC::LevelC()                                      : Scene { {0.0f}, nullptr,   } {}
LevelC::LevelC(Vector2 origin, const char *bgHexCode, Music bgMusic) : Scene { origin, bgHexCode, bgMusic } {}

LevelC::~LevelC() {  }

void LevelC::initialise()
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
      getGridPos(3, 21), // position
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
      getGridPos(5, 8), // position
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
   maindoor->setInteractable(false);
   mGameState.collidableEntities.insert({"maindoor", maindoor});


   // knife
   Entity* knife = new Entity {
        {mGameState.maps[0]->getLeftBoundary() + 2.5f*TILE_DIMENSION, 
        mGameState.maps[0]->getTopBoundary() + 6*TILE_DIMENSION+21},
        {TILE_DIMENSION, TILE_DIMENSION*1.25},
        "assets/knife.png",
        BLOCK  
    };
    knife->setColliderDimensions({
        knife->getScale().x,
        knife->getScale().y 
    });
    knife->setName("knife");
    knife->setInteractable(false);
    mGameState.collidableEntities.insert({"knife", knife});

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
   door->setInteractable(false);
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
//    bed->setInteractable(false);
   bed->setName("bed");
   
   mGameState.collidableEntities.insert({"bed", bed});


   /*
      ----------- UI -----------
   */
   mGameState.UIs["eKey"] = LoadTexture("assets/E.png");
   mGameState.UIs["TVEffect"] = LoadTexture("assets/TVEffect.png");
   mGameState.UIs["board"] = LoadTexture("assets/boardrules.png");
   mGameState.UIs["mom"] = LoadTexture("assets/mom2.png");
   mGameState.UIs["dad"] = LoadTexture("assets/dad2.png");
   mGameState.UIs["blood1"] = LoadTexture("assets/blood1.png");
   mGameState.UIs["blood2"] = LoadTexture("assets/blood2.png");
   mGameState.UIs["knife"] = LoadTexture("assets/blade.png");
   mGameState.UIs["me"] = LoadTexture("assets/me.png");

   
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
      "......",
   };

   Dialoge* dialoge = new Dialoge(lines, "character");
   mGameState.activeDialogue = dialoge;
   mGameState.dialogues["opening"] = dialoge;

   // news
   lines = {
      "Ufnsp**fdds-wfpwogjh[pja]a",
      "zcmdpo gy249gy2fy]^qw[kded/a*&]",
      "NY#*Of)@fj)lEuishdf wpf%(U)"
   };

   dialoge = new Dialoge(lines, "News");
   mGameState.dialogues["News"] = dialoge;


   // mom1

   lines = {
        "wgI#RYFsfdNIq8hohf*%*^(&ou*ds)",
        "N#7y9&&G@(fnqpofs*^i0%.,hl",
        "fsf^*(@UDfjwnna0)*^&*@$sfB"
    };

    dialoge = new Dialoge(lines, "Mom");
    mGameState.dialogues["mom_morning1"] = dialoge;


   // dad1
    lines = {
        "@^&Tf%=##q5^%%weB%R*Ugufwdx",
        "fb8R*@8df8^%*9fdjo3"
    };

    dialoge = new Dialoge(lines, "Dad");
    mGameState.dialogues["dad_morning1"] = dialoge;

   // after mom and dad
   lines = {
        "What is happening...?",
        "Why did mom and dad look like that?",
        "Have they been replaced?",
        "fsfeg&Iw38(@rdfhs)#*!(3jdf3",
        "What am I talking about?",
        "I have to make a decision.",
        "I could examine them with the silver blade, like said in the last guideline.",
        "I need to grab the silver knife in the kitchen."
    };

    dialoge = new Dialoge(lines, "character");
    mGameState.dialogues["momdad"] = dialoge;

    // chose to believe
   lines = {
        "I trust my parents.",
        "They will never harm me, even if they have been replaced.",
        "It doesn't matter what happens in the outside world."
        "We will live together happily forever."
    };

    dialoge = new Dialoge(lines, "character");
    mGameState.dialogues["trust"] = dialoge;

    // chose to stab
   lines = {
        "Why are the wounds not closing?",
        "The rules said the wound would close, I followed the instructions perfectly.",
        "I did everything correct.",
        "But what have I done.",
        "Can someone just make this stop.",
        "Wait, I haven't looked at myself for a long while."
    };

    dialoge = new Dialoge(lines, "character");
    mGameState.dialogues["stab"] = dialoge;
    

   /*
      ----------- SHADER -----------
   */
    mGameState.shader.load("shaders/vertex.glsl", "shaders/fragment.glsl");
    mGameState.shaderEffect = 2;

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.xochitl->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom


   /*
      ----------- EFFECT -----------
   */
   mGameState.effect = new Effects(mOrigin, (float) GetScreenWidth() * 2.0f, (float) GetScreenHeight() * 2.0f);
   mGameState.effect->setEffectSpeed(0.15f);
   mGameState.effect->start(FADEIN);

}

Vector2 LevelC::getGridPos(int row, int col) {
   return {mGameState.maps[0]->getLeftBoundary() + (float) (row*TILE_DIMENSION), 
      mGameState.maps[0]->getTopBoundary() + (float)(col*TILE_DIMENSION)};
}

void LevelC::update(float deltaTime)
{
    // update music 
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
      bool setNewDialogue = false;
      
      if (mGameState.activeDialogue == mGameState.dialogues["mom_morning1"] && !mGameState.activeChoice) {
         mGameState.activeChoice = new ChoiceDialogue("mom_morning", {"Observe"});
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["dad_morning1"] && !mGameState.activeChoice) {
         mGameState.activeChoice = new ChoiceDialogue("dad_morning", {"Observe"});
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["momdad"]) {
        mGameState.collidableEntities["knife"]->setInteractable(true);
     }
      if (!setNewDialogue) {
          mGameState.activeDialogue = nullptr;
      }
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
      mGameState.activeDialogue = mGameState.dialogues["News"];
   }
   
   // Update TV typing effect
    if (TVstates.displayingTV && IsKeyPressed(KEY_E) && mGameState.dialogues["News"]->isDialogueComplete()) {
        TVstates.displayingTV = false;
        StopSound(mGameState.sounds["TVSound"]);
        mGameState.collidableEntities["TV"]->setInteractable(false);
        mGameState.collidableEntities["door"]->setInteractable(true);
        mGameState.collidableEntities["board"]->setInteractable(true);
        // mGameState.activeDialogue = mGameState.dialogues["character"];
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
         && IsKeyPressed(KEY_E) && !mGameState.dialogues["mom_morning1"]->isDialogueComplete()
         && !mGameState.activeDialogue && !grabbedKnife) {
      mGameState.activeDialogue = mGameState.dialogues["mom_morning1"];
      mGameState.collidableEntities["mom"]->setAIState(IDLE);
   }
   

   if (displayingMom && IsKeyPressed(KEY_E)) {
      displayingMom= false;
      mGameState.collidableEntities["mom"]->setInteractable(false);
      mGameState.collidableEntities["mom"]->setAIState(WALKING);
   }

   if (displayingDad && IsKeyPressed(KEY_E)) {
        displayingDad= false;
        mGameState.collidableEntities["dad"]->setInteractable(false);
        mGameState.collidableEntities["dad"]->setAIState(WALKING);
    }


   // dad interaction
   if (isAround(mGameState.collidableEntities["dad"]) && mGameState.collidableEntities["dad"]->isInteractable()
         && IsKeyPressed(KEY_E) && !mGameState.dialogues["dad_morning1"]->isDialogueComplete()
        && !mGameState.activeDialogue && !grabbedKnife) {
      mGameState.activeDialogue = mGameState.dialogues["dad_morning1"];
      mGameState.collidableEntities["dad"]->setAIState(IDLE);
   }
   
   
   if (isAround(mGameState.collidableEntities["knife"]) && mGameState.collidableEntities["knife"]->isInteractable()
         && IsKeyPressed(KEY_E) && !grabbedKnife) {
      displayingKnife = true;
      grabbedKnife = true;
      mGameState.collidableEntities["dad"]->setInteractable(true);
      mGameState.collidableEntities["mom"]->setInteractable(true);
   }
   else if (displayingKnife && IsKeyPressed(KEY_E)) {
        displayingKnife= false;
    }
    else if ((isAround(mGameState.collidableEntities["dad"]) || isAround(mGameState.collidableEntities["mom"]))
            && grabbedKnife && IsKeyPressed(KEY_E)) {
        mGameState.activeChoice = new ChoiceDialogue("stab", {"Perform the silver blade test", "I trust them, they will never harm me."});
    }


   
   // Handle choices
   if (mGameState.activeChoice) {
      mGameState.activeChoice->handleInput();
      
      if (mGameState.activeChoice->hasSelectedChoice()) {
         int choice = mGameState.activeChoice->getSelectedChoice();
         if (mGameState.activeChoice->getName() == "mom_morning") {
            if (choice == 0) {
                displayingMom = true;
                PlaySound(mGameState.sounds["jumpscare"]);
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
         else if (mGameState.activeChoice->getName() == "dad_morning") {
            if (choice == 0) {
                displayingDad = true;
                PlaySound(mGameState.sounds["jumpscare"]);
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
         else if (mGameState.activeChoice->getName() == "stab") {
            if (choice == 0) {
                displayingBlood = true;
                choseStab = true;
            } 
            else {
                mGameState.activeDialogue = mGameState.dialogues["trust"];
                choseStab = false;
            }
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
         else if (mGameState.activeChoice->getName() == "mirror") {
            if (choice == 0) {
                displayingMe = true;
                startTime = 0.0f;
                PlaySound(mGameState.sounds["jumpscare2"]);
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
      }
   }

   

   if (mGameState.dialogues["mom_morning1"]->isDialogueComplete() 
         && mGameState.dialogues["dad_morning1"]->isDialogueComplete()
         && !mGameState.dialogues["momdad"]->isDialogueComplete()
         && !mGameState.activeDialogue && !displayingDad && !displayingMom
        && !mGameState.activeChoice) {
        mGameState.activeDialogue = mGameState.dialogues["momdad"];
   }

   if (mGameState.dialogues["stab"]->isDialogueComplete() && !mGameState.activeChoice && !displayingMe) {
        mGameState.activeChoice = new ChoiceDialogue("mirror", {"Look in the mirror."});
   }

   mGameState.effect->update(deltaTime, nullptr);

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mGameState.xochitl->getPosition().y };

   if (mGameState.effectStarted && choseStab && !mGameState.dialogues["stab"]->isDialogueComplete()) {
        if (!mGameState.effect->isEffectComplete()) {
            displayingBlood = true;
        } 
        else {
            blackscreen = true;
            startTime += deltaTime;
            if (startTime > 3.0f) {
                blackscreen = false;
                mGameState.shaderEffect = 3;
                mGameState.effect->start(FADEIN);
                mGameState.activeDialogue = mGameState.dialogues["stab"];
                mGameState.collidableEntities["mom"]->deactivate();
                mGameState.collidableEntities["dad"]->deactivate();
                mGameState.effectStarted = false;
                startTime = 0.0f;
                displayingBlood = false;
            }
            else displayingBlood = true;
        }
   }
   
   if (displayingBlood) {
        startTime += deltaTime;
   }

   if (displayingMe) {
        startTime += deltaTime;
    }

    if (mGameState.dialogues["stab"]->isDialogueComplete() && mGameState.effectStarted 
        && mGameState.effect->isEffectComplete()) {
            mGameState.nextSceneID = 7;
        }

   if (mGameState.dialogues["trust"]->isDialogueComplete() && !mGameState.effectStarted) {
      mGameState.effect->start(FADEOUT);
      mGameState.effectStarted = true;
   }

   if (mGameState.dialogues["trust"]->isDialogueComplete() && mGameState.effectStarted
         && mGameState.effect->isEffectComplete()) {
      mGameState.nextSceneID = 6;
   }

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelC::render()
{
    mGameState.shader.begin();
    Vector2 charpos = mGameState.xochitl ? mGameState.xochitl->getPosition() : Vector2{0.0f, 0.0f};
    mGameState.shader.setVector2("lightPosition", charpos);
    mGameState.shader.setInt("effect", mGameState.shaderEffect);
   // ClearBackground(ColorFromHex(mBGColourHexCode));
   for (size_t i = 0; i < mGameState.maps.size(); ++i) mGameState.maps[i]->render();
   
   for (const auto& pair : mGameState.collidableEntities)
   {
        if (!pair.second->isActive()) continue;
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
    if (!entity->isActive()) continue;
      entity->render();
      if ((entity->getName() == "mom" || entity->getName() == "dad") && isAround(entity) && entity->isInteractable())
         drawUI(mGameState.UIs["eKey"], entity->getPosition(), 2.2f, 230);
   }
   
    mGameState.shader.end();
   
}

void LevelC::renderUI()
{
   Scene::renderUI();
   
   // displayTV
  if (TVstates.displayingTV) displayTV();
  if (displayingBoard) displayBoard();
  if (displayingMom) displayImage(mGameState.UIs["mom"]);
  if (displayingDad) displayImage(mGameState.UIs["dad"]);
  if (displayingBlood) displayBlood();
  if (displayingKnife) displayKnife();
  if (displayingMe) displayMe();

  // display dialoge
  if (mGameState.activeDialogue) {
      mGameState.activeDialogue->render();
   }

  // display choice dialogue
  if (mGameState.activeChoice) {
      mGameState.activeChoice->render();
   }
   if (blackscreen) DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
   mGameState.effect->render();
   
}

bool LevelC::isAround(Entity* entity) {
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

void LevelC::drawUI(Texture2D texture, Vector2 pos, float scale, unsigned char opacity) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {pos.x-texture.width*scale/2.0f, pos.y-texture.height*scale/2.0f, 
      (float)texture.width * scale, (float)texture.height * scale};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, {255, 255, 255, opacity});
}

void LevelC::displayTV() {
   Rectangle source = { 0, 0, (float)mGameState.UIs["TVEffect"].width, (float)mGameState.UIs["TVEffect"].height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(mGameState.UIs["TVEffect"], source, dest, {0.0f, 0.0f}, 0.0f, WHITE);

    int instructionWidth = MeasureText("Press E to close", 20);
    DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
   
}

void LevelC::displayBoard() {
   Rectangle source = { 0, 0, (float)mGameState.UIs["board"].width, (float)mGameState.UIs["board"].height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(mGameState.UIs["board"], source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   DrawText(TVstates.tvText.c_str(), 150, 100, 20, BLACK);
   int instructionWidth = MeasureText("Press E to close", 20);
   DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void LevelC::displayImage(Texture2D texture, bool displayclosed) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   int instructionWidth = MeasureText("Press E to close", 20);
   if (!displayclosed)
    DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void LevelC::displayBlood() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorFromHex("#330603"));
    
    if (startTime > 3.0f) {
        displayImage(mGameState.UIs["blood1"], true);
        if (!IsSoundPlaying(mGameState.sounds["stab"])) PlaySound(mGameState.sounds["stab"]);
        playHeartbeat = true;
    }
    if (startTime > 6.0f) {
        displayImage(mGameState.UIs["blood2"], true);
    }
    if (startTime > 9.0f) {
        displayingBlood = false;
        startTime = 0.0f;                 
        mGameState.effect->start(FADEOUT);
        mGameState.effectStarted = true;  
    }
}

void LevelC::displayKnife() {
    displayImage(mGameState.UIs["knife"], true);
    int instructionWidth = MeasureText("Press E to grab knife", 20);
    DrawText("Press E to grab knife", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void LevelC::displayMe() {
    displayImage(mGameState.UIs["me"], true);
    if (!mGameState.effectStarted && startTime > 3.0f) {
        mGameState.effect->start(FADEOUT);
        mGameState.effectStarted = true;
    }
}

void LevelC::shutdown()
{
   Scene::shutdown();
}