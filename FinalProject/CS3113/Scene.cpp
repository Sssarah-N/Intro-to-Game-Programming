#include "Scene.h"

// Scene::Scene() : mOrigin{{}} {
//     ClearBackground(WHITE);
// }


Scene::Scene(Vector2 origin, const char *bgHexCode, Music bgMusic) : mOrigin{origin}, mBGColourHexCode {bgHexCode}
{
    mGameState.bgm = bgMusic;
    ClearBackground(ColorFromHex(bgHexCode));
}

void Scene::initialise() {  
   mGameState.sounds["doorSound"] = LoadSound("assets/doorSound.wav");
   SetSoundVolume( mGameState.sounds["doorSound"], 1.0f);
   mGameState.sounds["typingSound"] = LoadSound("assets/OSD.flac");
   mGameState.sounds["interactSound"] = LoadSound("assets/interact.wav");
   SetSoundVolume(mGameState.sounds["interactSound"], 4.0f);
   mGameState.sounds["errorSound"] = LoadSound("assets/errorSound.wav");
   mGameState.sounds["TVSound"] = LoadSound("assets/tvNoise.wav");

   mGameState.activeChoice = nullptr;
   
   // Play background music if valid
   if (IsMusicValid(mGameState.bgm)) {
      PlayMusicStream(mGameState.bgm);
   }
}

void Scene::input(KeyboardKey key) {
    mGameState.key = key;
} 

void Scene::renderUI() {
    
}

Scene::~Scene() { }

void Scene::shutdown() {
    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;
    for (size_t i = 0; i < mGameState.maps.size(); ++i) delete mGameState.maps[i];
    mGameState.maps.clear();
    for (const auto& pair : mGameState.collidableEntities)
    {
        delete pair.second;
    }
    mGameState.collidableEntities.clear();  
    
    for (auto& pair : mGameState.sounds) {
        if (pair.second.frameCount > 0) {
            StopSound(pair.second);
            UnloadSound(pair.second);
        }
    }
    // Stop and unload background music if valid
    if (IsMusicValid(mGameState.bgm)) {
        StopMusicStream(mGameState.bgm);
        UnloadMusicStream(mGameState.bgm);
    }
}

