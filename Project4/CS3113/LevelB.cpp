#include "LevelB.h"
#include <iostream>

LevelB::LevelB()                                      : Scene { {0.0f}, nullptr   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelB::~LevelB() {  }

void LevelB::initialise()
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
      {mOrigin.x - 650.0f, mOrigin.y - 200.0f}, // position
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


   Entity* jumpBlock = new Entity {
      {mOrigin.x, 4*TILE_DIMENSION},
      {TILE_DIMENSION, TILE_DIMENSION},
      "assets/jumpBlock.png",
      BLOCK
   };
   jumpBlock->setEntityID("jumpBlock");

   Entity* key = new Entity {
      {mOrigin.x, 3*TILE_DIMENSION},
      {TILE_DIMENSION, TILE_DIMENSION},
      "assets/key.png",
      NPC
   };
   key->setCheckCollision(false);
   key->deactivate();

   Entity* lock = new Entity {
      {mOrigin.x+650, 6*TILE_DIMENSION},
      {TILE_DIMENSION, TILE_DIMENSION},
      "assets/lock.png",
      BLOCK
   };

   mGameState.collidableEntities.push_back(jumpBlock);
   mGameState.collidableEntities.push_back(key);
   mGameState.collidableEntities.push_back(lock);

   
   std::map<Direction, std::vector<int>> walkerAnimationAtlas
      = {
         {LEFT,  { 10, 11 }},
         {RIGHT, { 10, 11 }},
   };
   
  for (int i = 0; i < 3; ++i) {
      Entity* walker = new Entity(
         {mOrigin.x + (6*i-8)*TILE_DIMENSION + 50, 6*TILE_DIMENSION}, // position
         {50.0f, 50.0f},                       // scale
         "assets/tilemap-characters.png",                // texture file address
         ATLAS,                                  // single image or atlas?
         {3, 9},                       // atlas dimensions
         walkerAnimationAtlas,                    // actual atlas
         NPC                                     // entity type
      );
      walker->setAIType(WANDERER);
      walker->setAIState(WALKING);
      walker->setSpeed(Entity::DEFAULT_SPEED * 0.10f);
      walker->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY}); // Add gravity!
      walker->setColliderDimensions({
         walker->getScale().x / 2.0f,
         walker->getScale().y / 1.5f
      });
      walker->setDirection(RIGHT);
      walker->render(); // calling render once at the beginning to switch ghost's direction
   
      mGameState.collidableEntities.push_back(walker);
  }
   
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

void LevelB::update(float deltaTime)
{

   mGameState.xochitl->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      mGameState.collidableEntities              // col. entity count
   );
   
   // std::cout << mGameState.xochitl->getEntityState() << std::endl;

   if (mGameState.xochitl->isCollidingTop()) {
      if (mGameState.xochitl->getLastCollidedEntity() == mGameState.collidableEntities[0] ) {
               mGameState.collidableEntities[0]->deactivate();
               mGameState.collidableEntities[1]->activate();
               PlaySound(mGameState.aquireSound);
               mGameState.collidableEntities[1]->setPosition(
                  {mGameState.xochitl->getPosition().x - 100,
                     mGameState.xochitl->getPosition().y+TILE_DIMENSION}
               );
               mGameState.collidableEntities[1]->setPosition({
                  mGameState.xochitl->getPosition().x - 60,
                  mGameState.xochitl->getPosition().y});
            }
   }

   for (int i = 3; i < 6; ++i) {
      if (mGameState.xochitl->getEntityState() == ATTACK) {
         float charX = mGameState.xochitl->getPosition().x;
         float walkerX = mGameState.collidableEntities[i]->getPosition().x;
         if (((charX -walkerX) < 70 && mGameState.xochitl->getDirection() == LEFT)
               || ((walkerX - charX) < 70 && mGameState.xochitl->getDirection() == RIGHT))
             mGameState.collidableEntities[i]->deactivate();
      }
      else if (mGameState.xochitl->getLastCollidedEntity() == mGameState.collidableEntities[i]) {
         --lives;
         if (lives > 0) mGameState.nextSceneID = 1;
            else mGameState.nextSceneID = 4;
      }
   }

   if (mGameState.collidableEntities[1]->isActive())  
      mGameState.collidableEntities[1]->setPosition({
         mGameState.xochitl->getPosition().x - 100,
         mGameState.xochitl->getPosition().y
      });

   
   // mGameState.collidableEntities[0]->update(deltaTime, mGameState.xochitl, mGameState.map, {});
   for (Entity* entity : mGameState.collidableEntities) {
      entity->update(
         deltaTime,
         mGameState.xochitl,
         mGameState.map,
         {}
      );
   }

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mOrigin.y-90 };

   if (mGameState.xochitl->getPosition().y > 800.0f) {
      --lives;
      if (lives > 0) mGameState.nextSceneID = 1;
      else mGameState.nextSceneID = 4;
   }
   if (mGameState.xochitl->getLastCollidedEntity() == mGameState.collidableEntities[2] 
         && mGameState.collidableEntities[1]->isActive()) {
            mGameState.nextSceneID = 2;
         }

   // printf("%f, %f\n", mGameState.xochitl->getPosition().x, mGameState.xochitl->getPosition().y);


   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelB::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.xochitl->render();
   for (int i = 0; i < mGameState.collidableEntities.size(); ++i) {
      mGameState.collidableEntities[i]->render();
   }
   
   mGameState.map->render();
}

void LevelB::renderUI()
{
   for (int i = 0; i < 3; ++i) {
      mGameState.hearts[i]->render();
   }
}

void LevelB::shutdown()
{
   printf("LevelB::shutdown()");
   Scene::shutdown();
}