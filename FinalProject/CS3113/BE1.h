#ifndef BE1_H
#define BE1_H

#include "Scene.h"
#include "Dialoge.h"
#include "LevelB.h"

class BE1 : public Scene {
private:
    void displayEnding();
    void displayImage(Texture2D texture, bool displayclosed = false);

public:
    static constexpr float TILE_DIMENSION       = 55.0f,
                            ACCELERATION_OF_GRAVITY = 981.0f,
                            END_GAME_THRESHOLD      = 800.0f;

    // BE1();
    BE1(Vector2 origin, const char *bgHexCode, Music bgm);
    ~BE1();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void renderUI() override;
    void shutdown() override;
    Vector2 getGridPos(int row, int col);
    
};

#endif