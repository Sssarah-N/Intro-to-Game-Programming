#include "LevelB.h"
#include "Dialoge.h"
#include <iostream>
#include <algorithm>
#include <string>

// LevelB::LevelB()                                      : Scene { {0.0f}, nullptr,   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode, Music bgMusic) : Scene { origin, bgHexCode, bgMusic } {}

LevelB::~LevelB() {  }

void LevelB::initialise()
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

   mGameState.sounds["knock"] = LoadSound("assets/knockSound.wav");
   SetSoundVolume(mGameState.sounds["knock"], 2.0f);
   mGameState.sounds["stab"] = LoadSound("assets/stabSound.wav");
   mGameState.sounds["scream"] = LoadSound("assets/scream.wav");
   mGameState.sounds["heartbeat"] = LoadSound("assets/heartbeat.wav");
   SetSoundVolume(mGameState.sounds["heartbeat"], 4.0f);

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
      getGridPos(5, 7), // position
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
      ----------- INSPECTOR -----------
   */
  std::map<Direction, std::vector<int>> inspectorWalkAtlas = {
    {DOWN,  {  0, 1, 2}},
    {LEFT,  {  3, 4, 5}},
    {RIGHT, {  6, 7, 8}},
    {UP,    {  9, 10, 11}},
    };
 
    Entity* inspector = new Entity(
       getGridPos(12, 6), // position
       // {mGameState.maps[0]->getLeftBoundary()+TILE_DIMENSION, mGameState.maps[0]->getBottomBoundary()-TILE_DIMENSION}, // position
       {100.0f, 100.0f},             // scale
       "assets/inspector.png",                   // texture file address
       ATLAS,                                    // single image or atlas?
       { 4, 3 },                                 // atlas dimensions
       inspectorWalkAtlas,                    // actual atlas
       NPC                                    // entity type
    );
 
    inspector->setSpeed(20);
    inspector->setName("inspector");
    inspector->setDirection(DOWN);
    inspector->setColliderDimensions({
       inspector->getScale().x / 3.0f,
       inspector->getScale().y / 3.0f
    });
    inspector->setAIType(FOLLOWER);
    inspector->setAIState(WALKING);
    inspector->render(); 
    inspector->setTarget(mGameState.collidableEntities["mom"]);
    inspector->deactivate();
    mGameState.collidableEntities.insert({"inspector", inspector});


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
   mGameState.UIs["blueshirt"] = LoadTexture("assets/blueshirt.png");
   mGameState.UIs["blood1"] = LoadTexture("assets/blood1.png");
   mGameState.UIs["blood2"] = LoadTexture("assets/blood2.png");

   
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
      "Today will be a better day...",
      "Let's check if there is any updates on the news"
   };

   Dialoge* dialoge = new Dialoge(lines, "character");
   mGameState.activeDialogue = dialoge;
   mGameState.dialogues["opening"] = dialoge;

   // news
   lines = {
      "URGENT ALERT",
      "It has been confirmed that part of the guidelines that was published yesterday were corrupted.", 
      "Some of the rules were altered by the Mimics to mislead you",
      "We are still confirming which guidelines were corrupted, but most of them should be correct",
      "In the mean time, do not trust all the guidelines you received.",
      "Stay safe, and report any suspicious entities you observe to the government inspectors"
   };

   dialoge = new Dialoge(lines, "News");
   mGameState.dialogues["News"] = dialoge;


   // mom1

   lines = {
        "Mom looks the same as yesterday",
        "I should question her with a private memory like stated in the guidelines"
    };

    dialoge = new Dialoge(lines, "character");
    mGameState.dialogues["mom_morning1"] = dialoge;
    
    lines = {
        "The beach?",
        " Oh... it was that beautiful shell necklace, right? You wore it all the time."
    };

   dialoge = new Dialoge(lines, "Mom");
   mGameState.dialogues["mom_morning2"] = dialoge;   


   // dad1
    lines = {
        "Dad looks the same as yesterday",
        "I should question him with a private memory like stated in the guidelines"
    };

    dialoge = new Dialoge(lines, "character");
    mGameState.dialogues["dad_morning1"] = dialoge;

   lines = {
      "It was a beautiful stone right?",
      "We found it on the beach."
   };

   dialoge = new Dialoge(lines, "Dad");
   mGameState.dialogues["dad_morning2"] = dialoge;

   // after mom and dad
   lines = {
        "They have different answers..",
        "I don't actually remember the answer. Who was correct?",
        "Did they simply forgot or has one of them got replaced?",
        "I shouldn't think about it more."
    };

    dialoge = new Dialoge(lines, "character");
    mGameState.dialogues["momdad"] = dialoge;
   

   // inspector
   lines = {
        "It's a blue shirt guy. The news guidelines said they are government inspectors.",
        "But not all guidelines are real.",
        "I should talk to them to confirm it."
    };

    dialoge = new Dialoge(lines, "character");
    mGameState.dialogues["peephole"] = dialoge;


   lines = {
      "Hello, we are the government inspectors.",
      "We are here to help you.",
      "Did you observe any suspicious entities that are behaving like a Mimic?",
      "If yes then we could help you to test them."
   };

   dialoge = new Dialoge(lines, "Inspector");
   mGameState.dialogues["inspector1"] = dialoge;

   lines = {
        "Ok, I'm happy to hear that.",
        "If you think something if wrong with your family, please contact us immediately.",
        "Have a good night."
    };

    dialoge = new Dialoge(lines, "Inspector");
    mGameState.dialogues["inspectorLeft"] = dialoge;

    lines = {
        "Of course, I will examine your parents.",
        "......",
        "Your parents have been corrupted.",
        "They are not humans anymore, but Mimics that are trying to replace you as well.",
        "I will help you take care of them, please wait here for a moment.",
     };
  
     dialoge = new Dialoge(lines, "Inspector");
     mGameState.dialogues["inspectorCame"] = dialoge;

     lines = {
        "No...",
        "I shouldn't have opened the door... I shouldn't",
        "But he was the government inspector, I thought I could trust him",
        "Mom.. Dad... I'm sorry",
        "I trusted the wrong one",
        "I MUST RUN NOW",
        "ESCAPE FROM THE FRONT DOOR"
     };
  
     dialoge = new Dialoge(lines, "character");
     mGameState.dialogues["blood"] = dialoge;


     lines = {
        "I should have let him in..",
        "But he might be a mimic as well.",
        "I cannot trust anyone",
        "I must figure everything out by myself.",
        "I should go to sleep now"
     };
  
     dialoge = new Dialoge(lines, "character");
     mGameState.dialogues["rejected"] = dialoge;

   
   /*
      ----------- SHADER -----------
   */
   mGameState.shader.load("shaders/vertex.glsl", "shaders/fragment.glsl");
   mGameState.shaderEffect = 0;
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

Vector2 LevelB::getGridPos(int row, int col) {
   return {mGameState.maps[0]->getLeftBoundary() + (float) (row*TILE_DIMENSION), 
      mGameState.maps[0]->getTopBoundary() + (float)(col*TILE_DIMENSION)};
}

void LevelB::update(float deltaTime)
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
      Dialoge* completedDialogue = mGameState.activeDialogue; 
      bool setNewDialogue = false;
      
      if (mGameState.activeDialogue == mGameState.dialogues["mom_morning1"] && !mGameState.activeChoice) {
         mGameState.activeChoice = new ChoiceDialogue("mom_morning", {"Souvenir found during beach trip?"});
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["dad_morning1"] && !mGameState.activeChoice) {
         mGameState.activeChoice = new ChoiceDialogue("dad_morning", {"Souvenir found during beach trip?"});
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["mom_morning2"]) {
        mGameState.collidableEntities["mom"]->setAIState(WALKING);
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["dad_morning2"]) {
        mGameState.collidableEntities["dad"]->setAIState(WALKING);
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["momdad"]) {
        // else if (mGameState.activeDialogue == mGameState.dialogues["opening"]) {
        knock = true;
        mGameState.collidableEntities["maindoor"]->setInteractable(true);
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["inspector1"]) {
        mGameState.activeChoice = new ChoiceDialogue("inspector1", {"Could you please test my parents?", "No, everyone is fine here."});
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["inspectorLeft"]) {
        mGameState.collidableEntities["maindoor"]->setTexture("assets/door_closed.png");
        mGameState.collidableEntities["maindoor"]->setInteractable(false);
        mGameState.activeDialogue = mGameState.dialogues["rejected"];
        setNewDialogue = true;
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["inspectorCame"]) {
        mGameState.collidableEntities["inspector"]->activate();
        mGameState.collidableEntities["inspector"]->setAIState(WALKING);
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["blood"]) {
        displayingBlood = false;
        mGameState.collidableEntities["mom"]->deactivate();
        mGameState.collidableEntities["dad"]->deactivate();
         
        Vector2 inspectorPos = mGameState.collidableEntities["inspector"]->getPosition();
        Vector2 playerPos = mGameState.xochitl->getPosition();
        float distance = Vector2Distance(inspectorPos, playerPos);
         waitingForInspector = true;
         inspectorWaitTimer = 0.0f;
      }
      else if (mGameState.activeDialogue == mGameState.dialogues["rejected"]) {
        mGameState.collidableEntities["bed"]->setInteractable(true);
      }
      if (!setNewDialogue) {
          mGameState.activeDialogue = nullptr;
      }
     }
   }

   // Handle inspector wait timer
   if (waitingForInspector) {
     inspectorWaitTimer += deltaTime;
     if (inspectorWaitTimer >= 3.0f) {
       // Wait complete, start chasing
       mGameState.collidableEntities["inspector"]->setTarget(mGameState.xochitl);
       mGameState.collidableEntities["inspector"]->setSpeed(50);
       waitingForInspector = false;
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
   mGameState.collidableEntities["inspector"]->update(deltaTime, mGameState.xochitl, mGameState.maps, mGameState.collidableEntities);

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
   
   if (isAround(mGameState.collidableEntities["maindoor"]) && mGameState.collidableEntities["maindoor"]->isInteractable()
         && IsKeyPressed(KEY_E)) {
      knock = false;
      if(IsSoundPlaying(mGameState.sounds["knock"])) {
        StopSound(mGameState.sounds["knock"]);
      }
      displayingBlue = true;
      mGameState.activeDialogue = mGameState.dialogues["peephole"];
   }

   if ( mGameState.dialogues["peephole"]->isDialogueComplete()
             && IsKeyPressed(KEY_E) && !mGameState.dialogues["inspector1"]->isDialogueComplete()) {
        displayingBlue = false;
        mGameState.activeDialogue = mGameState.dialogues["inspector1"];
        mGameState.collidableEntities["maindoor"]->setTexture("assets/door_opened.png");
        mGameState.collidableEntities["maindoor"]->setInteractable(false);
   }

   
   // Handle choices
   if (mGameState.activeChoice) {
      mGameState.activeChoice->handleInput();
      
      if (mGameState.activeChoice->hasSelectedChoice()) {
         int choice = mGameState.activeChoice->getSelectedChoice();
         if (mGameState.activeChoice->getName() == "mom_morning") {
            printf("mom_morning");
            if (choice == 0) {
               mGameState.activeDialogue = mGameState.dialogues["mom_morning2"];
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
         else if (mGameState.activeChoice->getName() == "dad_morning") {
            if (choice == 0) {
                mGameState.activeDialogue = mGameState.dialogues["dad_morning2"];
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
         else if (mGameState.activeChoice->getName() == "inspector1") {
            if (choice == 0) {
                mGameState.activeDialogue = mGameState.dialogues["inspectorCame"];
            } 
            else if (choice == 1) {
                mGameState.activeDialogue = mGameState.dialogues["inspectorLeft"];
            } 
            delete mGameState.activeChoice;
            mGameState.activeChoice = nullptr;
         }
      }
   }

   if (mGameState.dialogues["mom_morning2"]->isDialogueComplete() 
         && mGameState.dialogues["dad_morning2"]->isDialogueComplete()
         && !mGameState.dialogues["momdad"]->isDialogueComplete()
         && !mGameState.activeDialogue) {
        mGameState.activeDialogue = mGameState.dialogues["momdad"];
   }

   if (knock && !IsSoundPlaying(mGameState.sounds["knock"])) {
        PlaySound(mGameState.sounds["knock"]);
   }


   mGameState.effect->update(deltaTime, nullptr);

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mGameState.xochitl->getPosition().y };

   if (mGameState.effectStarted && mGameState.effect->isEffectComplete()) {
      printf("switched scene");
      mGameState.nextSceneID = 2;
   }

   Vector2 mompos = mGameState.collidableEntities["mom"]->getPosition();
   Vector2 inspectorpos = mGameState.collidableEntities["inspector"]->getPosition();
   if ((abs(mompos.x - inspectorpos.x) < 50.0f) && (abs(mompos.y - inspectorpos.y) < 50.0f) 
            && !displayingBlood && !mGameState.dialogues["blood"]->isDialogueComplete()) {
        displayingBlood = true;
        startTime = 0.0f;
        PlaySound(mGameState.sounds["scream"]);
        mGameState.shaderEffect = 2;
   }
   
   if (displayingBlood) {
        startTime += deltaTime;
   }

   if (playHeartbeat && !IsSoundPlaying(mGameState.sounds["heartbeat"])) 
        PlaySound(mGameState.sounds["heartbeat"]);

     if (isAround(mGameState.collidableEntities["maindoor"]) && mGameState.dialogues["blood"]->isDialogueComplete() && !canRun) {
         canRun = true;
         spacePressCount = 0;
     }

     if (isAround(mGameState.collidableEntities["inspector"]) && mGameState.dialogues["blood"]->isDialogueComplete()
         && !waitingForInspector) {
        displayingBlood = true;
    }

   
   if (canRun && !hasRun) {
        if (IsKeyPressed(KEY_SPACE)) {
            spacePressCount++;
        }
        if (spacePressCount >= 30) {
            hasRun = true;
            canRun = false;
            mGameState.nextSceneID = 4;
        }
   }

   if (isAround(mGameState.collidableEntities["bed"]) && IsKeyPressed(KEY_E) 
        && mGameState.collidableEntities["bed"]->isInteractable() && !mGameState.effectStarted) {
        mGameState.effect->start(FADEOUT);
        mGameState.effectStarted = true;
    }
   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelB::render()
{
   mGameState.shader.begin();
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
   Entity* inspector = mGameState.collidableEntities["inspector"];
   Entity* xochitl = mGameState.xochitl;
   
   std::vector<Entity*> sortedEntities = {mom, dad, xochitl, inspector};
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

void LevelB::renderUI()
{
   Scene::renderUI();
   
   // displayTV
  if (TVstates.displayingTV) displayTV();
  if (displayingBoard) displayBoard();
  if (displayingMom) displayImage(mGameState.UIs["mom"]);
  if (displayingDad) displayImage(mGameState.UIs["dad"]);
  if (displayingBlue) displayImage(mGameState.UIs["blueshirt"]);
  if (displayingBlood) displayBlood();
  
  // Render run indicator
  if (canRun && !hasRun) {
      const char* text = "PRESS SPACE TO RUN";
      int fontSize = 40;
      int textWidth = MeasureText(text, fontSize);
      int x = GetScreenWidth() / 2 - textWidth / 2;
      int y = GetScreenHeight() / 2 + 200;
      DrawRectangle(x - 20, y - 10, textWidth + 40, fontSize + 20, ColorAlpha(BLACK, 0.7f));
      DrawText(text, x, y, fontSize, WHITE);
      
      std::string progressText = std::to_string(spacePressCount) + "/30";
      int progressWidth = MeasureText(progressText.c_str(), 30);
      DrawText(progressText.c_str(), GetScreenWidth() / 2 - progressWidth / 2, y + fontSize + 10, 30, YELLOW);
  }

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

bool LevelB::isAround(Entity* entity) {
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

void LevelB::drawUI(Texture2D texture, Vector2 pos, float scale, unsigned char opacity) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {pos.x-texture.width*scale/2.0f, pos.y-texture.height*scale/2.0f, 
      (float)texture.width * scale, (float)texture.height * scale};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, {255, 255, 255, opacity});
}

void LevelB::displayTV() {
   Rectangle source = { 0, 0, (float)mGameState.UIs["TVEffect"].width, (float)mGameState.UIs["TVEffect"].height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(mGameState.UIs["TVEffect"], source, dest, {0.0f, 0.0f}, 0.0f, WHITE);

    int instructionWidth = MeasureText("Press E to close", 20);
    DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
   
}

void LevelB::displayBoard() {
   Rectangle source = { 0, 0, (float)mGameState.UIs["board"].width, (float)mGameState.UIs["board"].height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(mGameState.UIs["board"], source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   DrawText(TVstates.tvText.c_str(), 150, 100, 20, BLACK);
   int instructionWidth = MeasureText("Press E to close", 20);
   DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));
}

void LevelB::displayImage(Texture2D texture, bool displayclosed) {
   Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
   Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
   DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
   if (!displayclosed) {
      int instructionWidth = MeasureText("Press E to close", 20);
      DrawText("Press E to close", GetScreenWidth() - instructionWidth - 20, GetScreenHeight() - 40, 20, ColorAlpha(WHITE, 0.8f));   
   }
}

void LevelB::displayBlood() {
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
        // protagonist died
        if (mGameState.dialogues["blood"]->isDialogueComplete()) {
            mGameState.nextSceneID = 5;
        }
        // parents died
        else mGameState.activeDialogue = mGameState.dialogues["blood"];
    }
}

void LevelB::shutdown()
{
   Scene::shutdown();
}