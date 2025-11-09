#include "LevelA.h"
#include <iostream>

LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() {  }

void LevelA::initialise()
{
   Scene::initialise();
   mGameState.nextSceneID = -1;

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/tilemap1.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      16, 7,                        // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> xochitlWalkAtlas = {
      {DOWN,  {  9, 10, 11, 12, 13, 14, 15, 16}},
      {LEFT,  {  9, 10, 11, 12, 13, 14, 15, 16}},
      {UP,    {  9, 10, 11, 12, 13, 14, 15, 16}},
      {RIGHT, {  9, 10, 11, 12, 13, 14, 15, 16}},
   };

   std::map<EntityState, std::vector<int>> xochitlAttackAtlas = {
      {ATTACK,  {  18, 19, 20, 21, 22, 23}},
   };

   float sizeRatio  = 48.0f / 64.0f;

   // Assets from @see https://sscary.itch.io/the-adventurer-female
   mGameState.xochitl = new Entity(
      {mOrigin.x - 500.0f, mOrigin.y - 200.0f}, // position
      {350.0f, 350.0f},             // scale
      "assets/soldier.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 7, 9 },                                 // atlas dimensions
      xochitlWalkAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.xochitl->setAttackAnimations(xochitlAttackAtlas);

   
   mGameState.xochitl->setJumpingPower(450.0f);
   mGameState.xochitl->setColliderDimensions({
      mGameState.xochitl->getScale().x / 6.5f,
      mGameState.xochitl->getScale().y / 6.5f
   });
   mGameState.xochitl->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   // Walker NPC
   std::map<Direction, std::vector<int>> walkerAnimationAtlas
    = {
      {LEFT,  { 7, 8 }},
      {RIGHT, { 7, 8 }},
  };

  Entity* walker = new Entity(
      {mOrigin.x + 400.0f, 6*TILE_DIMENSION}, // position
      {50.0f, 50.0f},                       // scale
      "assets/tilemap-characters.png",                // texture file address
      ATLAS,                                  // single image or atlas?
      {3, 9},                       // atlas dimensions
      walkerAnimationAtlas,                    // actual atlas
      NPC                                     // entity type
  );

  walker->setAIType(FOLLOWER);
  walker->setAIState(IDLE);
  walker->setSpeed(Entity::DEFAULT_SPEED * 0.20f);
  walker->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY}); // Add gravity!

  walker->setColliderDimensions({
      walker->getScale().x / 2.0f,
      walker->getScale().y
  });
  walker->setDirection(LEFT);
  walker->render(); // calling render once at the beginning to switch ghost's direction

  mGameState.collidableEntities.push_back(walker);

  // jump block
   Entity* jumpBlock = new Entity {
      {mOrigin.x-5.5f*TILE_DIMENSION, 3*TILE_DIMENSION},
      {TILE_DIMENSION, TILE_DIMENSION},
      "assets/jumpBlock.png",
      BLOCK
   };
   jumpBlock->setEntityID("jumpBlock");

   // key
   Entity* key = new Entity {
      {mOrigin.x-5.5f*TILE_DIMENSION, 2*TILE_DIMENSION},
      {TILE_DIMENSION, TILE_DIMENSION},
      "assets/key.png",
      NPC
   };
   key->setCheckCollision(false);
   key->deactivate();

   // lock
   Entity* lock = new Entity {
      {mOrigin.x+600, 6*TILE_DIMENSION},
      {TILE_DIMENSION, TILE_DIMENSION},
      "assets/lock.png",
      BLOCK
   };

   mGameState.collidableEntities.push_back(jumpBlock);
   mGameState.collidableEntities.push_back(key);
   mGameState.collidableEntities.push_back(lock);

   int currLives = lives;
   for (int i = 0; i < 3; ++i) {
      if (currLives > 0) {
         mGameState.hearts.push_back (new Entity (
            {50 + TILE_DIMENSION*i, 50},
            {TILE_DIMENSION, TILE_DIMENSION},
            "assets/heartFull.png",
            BLOCK
         ));
      }
      else {
         mGameState.hearts.push_back (new Entity (
            {50 + TILE_DIMENSION*i, 50},
            {TILE_DIMENSION, TILE_DIMENSION},
            "assets/heartEmpty.png",
            BLOCK
         ));
      }
      --currLives;
   }

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.xochitl->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom
}

void LevelA::update(float deltaTime)
{
   // UpdateMusicStream(mGameState.bgm);

   mGameState.xochitl->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      mGameState.collidableEntities              // col. entity count
   );
   
   // std::cout << mGameState.xochitl->getEntityState() << std::endl;

   if (mGameState.xochitl->isCollidingTop()) {
      if (mGameState.xochitl->getLastCollidedEntity() == mGameState.collidableEntities[1] ) {
               mGameState.collidableEntities[1]->deactivate();
               mGameState.collidableEntities[2]->activate();
               PlaySound(mGameState.aquireSound);
               mGameState.collidableEntities[2]->setPosition(
                  {mGameState.xochitl->getPosition().x - 100,
                     mGameState.xochitl->getPosition().y+TILE_DIMENSION}
               );
               mGameState.collidableEntities[2]->setPosition({
                  mGameState.xochitl->getPosition().x - 60,
                  mGameState.xochitl->getPosition().y});
            }
   }

   if (mGameState.xochitl->getLastCollidedEntity() == mGameState.collidableEntities[0]) {
         if (mGameState.xochitl->getEntityState() == ATTACK) mGameState.collidableEntities[0]->deactivate();
         else {
            --lives;
            if (lives > 0) mGameState.nextSceneID = 0;
            else mGameState.nextSceneID = 4;
         }
   }

   if (mGameState.collidableEntities[2]->isActive())  
      mGameState.collidableEntities[2]->setPosition({
         mGameState.xochitl->getPosition().x - 100,
         mGameState.xochitl->getPosition().y
      });

   
   mGameState.collidableEntities[0]->update(deltaTime, mGameState.xochitl, mGameState.map, {});

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mOrigin.y-90 };

   // fall out the map, -1 live
   if (mGameState.xochitl->getPosition().y > 800.0f) {
      --lives;
      if (lives > 0) mGameState.nextSceneID = 0;
            else mGameState.nextSceneID = 4;
   }

   // finished level, move to next level
   if (mGameState.xochitl->getLastCollidedEntity() == mGameState.collidableEntities[3] 
         && mGameState.collidableEntities[2]->isActive()) {
            mGameState.nextSceneID = 1;
         }
         

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelA::render()
{
   // ClearBackground(ColorFromHex(mBGColourHexCode));
   

   mGameState.xochitl->render();
   for (int i = 0; i < mGameState.collidableEntities.size(); ++i) {
      mGameState.collidableEntities[i]->render();
   }
   
   mGameState.map->render();
}

void LevelA::renderUI()
{
   Scene::renderUI();
   for (int i = 0; i < 3; ++i) {
      mGameState.hearts[i]->render();
   }
}

void LevelA::shutdown()
{
   Scene::shutdown();
}