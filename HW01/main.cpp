/**
* Author: Sarah Niu
* Assignment: Simple 2D Scene
* Date due: 2025-09-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "raylib.h"
#include <math.h>

// Enums
enum AppStatus { TERMINATED, RUNNING };

// Global Constants
constexpr int SCREEN_WIDTH        = 800 * 1.5f,
              SCREEN_HEIGHT       = 450 * 1.5f,
              FPS                 = 60,
              SIDES               = 3;

constexpr float EARTH_ORBIT_RADIUS          = 200.0f, // radius of the orbit
                MOON_ORBIT_RADIUS  = 50.0F,
                ORBIT_SPEED     = 0.01f,  // the speed at which the triangle will travel its orbit
                EARTH_BASE_SIZE       = 50,     // the size of the triangle when it's not being scaled
                MOON_BASE_SIZE       = 20,     // the size of the triangle when it's not being scaled
                EARTH_AMPLITUDE   = 20.0f,  // by how much the triangle will be expanding/contracting
                MOON_AMPLITUDE = 10.0F;

constexpr Vector2 ORIGIN = { 
    SCREEN_WIDTH  / 2, 
    SCREEN_HEIGHT / 2
};

// Global Variables
AppStatus gAppStatus = RUNNING;

// float gScaleFactor   = BASE_SIZE,
//       gAngle         = 0.0f,
//       gPulseTime     = 0.0f;

int frames = 0;
float gPreviousTicks = 0.0f;
float time = 0.0f;

Vector2 sunPos;
Texture2D sunText;
float sunSize = 100.0f;
float sunRot = 0.0f;

Vector2 earthPos;
Texture2D earthText;
float earthSize;
float earthRot = 0.0f;

Vector2 moonPos;
Texture2D moonText;
float moonSize;
float moonRot = 0.0f;

Vector2 astronautPos = {700,300};
Texture2D astronautText;
float astronautSize = 50;
float astronautRot = 0.0f;
float astronautLocation = 0.0f;

Vector2 starPos = ORIGIN;
Texture2D starText;
float starSize = 1.0f;

float gOrbitLocation = 0.0f;

Color bgColor = {20, 31, 56, 225};
int colorChange = 1;


//images
constexpr char sunTexture[] = "assets/sun.PNG";
constexpr char earthTexture[] = "assets/earth.PNG";
constexpr char moonTexture[] = "assets/moon.PNG";
constexpr char astronautTexture[] = "assets/astronaut.PNG";
constexpr char starTexture[] = "assets/star.PNG";

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

#include <stdio.h>
#include <stdlib.h>

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HW1");
    earthText = LoadTexture(earthTexture);
    moonText = LoadTexture(moonTexture);
    sunText = LoadTexture(sunTexture);
    astronautText = LoadTexture(astronautTexture);
    starText = LoadTexture(starTexture);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    time += deltaTime*1.0f;

    gOrbitLocation += ORBIT_SPEED;

    // scaling effect
    earthSize = EARTH_BASE_SIZE + EARTH_AMPLITUDE*sin(gOrbitLocation);
    moonSize = MOON_BASE_SIZE + MOON_AMPLITUDE*sin(gOrbitLocation);

    // rotation effect
    earthRot += 0.5f;
    moonRot += 0.5f;
    sunRot += 0.3f;

    // orbit effect
    earthPos.x = SCREEN_WIDTH/2 + EARTH_ORBIT_RADIUS*cos(gOrbitLocation);
    earthPos.y = SCREEN_HEIGHT/2 + EARTH_ORBIT_RADIUS*sin(gOrbitLocation);

    moonPos.x = earthPos.x + MOON_ORBIT_RADIUS*cos(gOrbitLocation);
    moonPos.y = earthPos.y + MOON_ORBIT_RADIUS*sin(gOrbitLocation);

    // astronaut movement
    //Vector2 astronautOrigin = {}
    astronautLocation += 0.05;
    astronautPos.x = 50*cos(time)+ 800;
    if (astronautPos.y > 800) {
        astronautPos.y = 200;
        astronautSize = 50;
    }
    else {
        astronautPos.y += 1;
        astronautSize += 0.5;
    } 
     

    // star movement
    Vector2 starPosOrigin = ORIGIN; 
    starPos.x -= 2;
    starPos.y -= 1;
    starSize += 1;
    if (starPos.x <= -200) {
        starPos = starPosOrigin;
        starSize = 1;
    }

    frames += 1;

}

void render()
{
    BeginDrawing();
    
    // background color changes based on frame time
    if (frames % 10 == 0) {
        bgColor.r += colorChange;
        if (bgColor.r >= 40) colorChange = -1;
        if (bgColor.r <= 15)  colorChange = 1;
    }

    ClearBackground(bgColor);
    
    // draw sun
    Rectangle sunSource = { 0.0f, 0.0f, (float)sunText.width, (float)sunText.height };
    Rectangle sunDest = { ORIGIN.x, ORIGIN.y, 2*sunSize, 2*sunSize };
    Vector2 sunOrigin = { sunSize, sunSize };
    DrawTexturePro(sunText, sunSource, sunDest, sunOrigin, sunRot, WHITE);

    // draw earth
    Rectangle earthSource = { 0.0f, 0.0f, (float)earthText.width, (float)earthText.height };
    Rectangle earthDest = { earthPos.x , earthPos.y, 2*earthSize, 2*earthSize };
    Vector2 earthOrigin = { earthSize, earthSize };
    DrawTexturePro(earthText, earthSource, earthDest, earthOrigin, earthRot, WHITE);

    // // draw moon
    Rectangle moonSource = { 0.0f, 0.0f, (float)moonText.width, (float)moonText.height };
    Rectangle moonDest = { moonPos.x , moonPos.y, 2*moonSize, 2*moonSize };
    Vector2 moonOrigin = { moonSize, moonSize };
    DrawTexturePro(moonText, moonSource, moonDest, moonOrigin, moonRot, WHITE);


     // // draw astronaut
    Rectangle astronautSource = { 0.0f, 0.0f, (float)astronautText.width, (float)astronautText.height };
    Rectangle astronautDest = { astronautPos.x , astronautPos.y, 2*astronautSize, 2*astronautSize };
    Vector2 astronautOrigin = { astronautSize, astronautSize };
    DrawTexturePro(astronautText, astronautSource, astronautDest, astronautOrigin, astronautRot, WHITE);

    
     // // draw star
    Rectangle starSource = { 0.0f, 0.0f, (float)starText.width, (float)starText.height };
    Rectangle starDest = { starPos.x , starPos.y, 2*starSize, 2*starSize };
    Vector2 starOrigin = { starSize, starSize };
    DrawTexturePro(starText, starSource, starDest, starOrigin, 0.0f, WHITE);



    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}