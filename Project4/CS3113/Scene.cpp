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
}

void Scene::input(KeyboardKey key) {
    mGameState.key = key;
} 

Scene::~Scene() { }

void Scene::shutdown() {
    printf("Scene::shutdown()\n");
    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;
    printf("here1\n");
    for (size_t i = 0; i < mGameState.collidableEntities.size(); ++i) {
        delete mGameState.collidableEntities[i];
    }
    printf("here2\n");
    mGameState.collidableEntities.clear();  

    for (size_t i = 0; i < mGameState.hearts.size(); ++i) {
        delete mGameState.hearts[i];
    }
    mGameState.hearts.clear();
    printf("here3\n");
    
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
    
    printf("finished Scene::shutdown()\n");
}

