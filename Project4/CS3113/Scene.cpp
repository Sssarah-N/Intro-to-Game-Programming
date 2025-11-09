#include "Scene.h"

Scene::Scene() : mOrigin{{}} {
    ClearBackground(WHITE);
}

int Scene::lives = 3;

Scene::Scene(Vector2 origin, const char *bgHexCode) : mOrigin{origin}, mBGColourHexCode {bgHexCode}
{
    ClearBackground(ColorFromHex(bgHexCode));
}

void Scene::initialise() {  
   mGameState.jumpSound = LoadSound("assets/jump.wav");
   SetSoundVolume( mGameState.jumpSound, 0.5f);
   mGameState.attackSound = LoadSound("assets/attack.wav");
   mGameState.aquireSound = LoadSound("assets/acquire.wav");
   mGameState.bg = LoadTexture("assets/bg4.jpg");
}

void Scene::input(KeyboardKey key) {
    mGameState.key = key;
} 

void Scene::renderUI() {
    Rectangle source {
        0, 0, mGameState.bg.width, mGameState.bg.height
     };
     Rectangle destination {
        mOrigin.x, mOrigin.y, 1000, 600
     };
  
     DrawTexturePro(mGameState.bg, source, destination, mOrigin, 0.0f, ColorFromHex("#ffffffff"));
}

Scene::~Scene() { }

void Scene::shutdown() {
    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;
    for (size_t i = 0; i < mGameState.collidableEntities.size(); ++i) {
        delete mGameState.collidableEntities[i];
    }
    mGameState.collidableEntities.clear();  

    for (size_t i = 0; i < mGameState.hearts.size(); ++i) {
        delete mGameState.hearts[i];
    }
    mGameState.hearts.clear();

    UnloadTexture(mGameState.bg);
    
    if (mGameState.jumpSound.frameCount > 0) {
        StopSound(mGameState.jumpSound);
        UnloadSound(mGameState.jumpSound);
    }
    if (mGameState.attackSound.frameCount > 0) {
        StopSound(mGameState.attackSound);
        UnloadSound(mGameState.attackSound);
    }
    if (mGameState.aquireSound.frameCount > 0) {
        StopSound(mGameState.aquireSound);
        UnloadSound(mGameState.aquireSound);
    }
    
}

