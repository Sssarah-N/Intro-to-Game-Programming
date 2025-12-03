#include "Entity.h"
#include "Dialoge.h"
#include "ChoiceDialogue.h"
#include "Effects.h"
#include <map>

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity *xochitl = nullptr;
    std::vector<Map*> maps = {};
    std::map<std::string, Entity*> collidableEntities;
    std::map<std::string, Dialoge*> dialogues;
    int state = 0;
    Dialoge* activeDialogue;
    ChoiceDialogue* activeChoice;
    std::map<std::string, Texture2D> UIs;

    Music bgm;
    std::map<std::string, Sound> sounds;

    Effects* effect;
    bool effectStarted = false;

    Camera2D camera;
    KeyboardKey key = KEY_F1;
    int nextSceneID;
};


struct TVstates
{
    bool displayingTV = false;
    std::string tvText;
    size_t tvRevealedChars = 0;
    float tvTypingTimer = 0.0f;
    float tvCharsPerSecond = 30.0f;  
    bool tvTextFullyRevealed = false;
    bool startDisplay = false;
};


class Scene 
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";
    

public:
    static int lives;
    // Scene();
    Scene(Vector2 origin, const char *bgHexCode, Music bgMusic = {});

    virtual ~Scene();
    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void renderUI() = 0;
    virtual void shutdown() = 0;
    virtual void input(KeyboardKey key); 
    int getLives() { return lives; }
    
    GameState   getState()           const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif