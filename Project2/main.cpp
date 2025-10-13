/**
* Author: Sarah Niu
* Assignment: Pong Clone
* Date due: 2025-10-13, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"
#include <iostream>

// Global Constants
constexpr int SCREEN_WIDTH  = 1600 / 2,
              SCREEN_HEIGHT = 900 / 2,
              FPS           = 120,
              BALL_SPEED    = 200;
              

constexpr char    BG_COLOUR[] = "#000000ff";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
Vector2 paddleScale = {100, 100};
Vector2 ballScale = { 50.0f, 50.0f };

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;

std::vector<Entity*> balls;
Entity *leftPaddle = nullptr;
Entity *rightPaddle = nullptr;
bool singleMode = false;
bool rightPaddleDirection = 0; // move up = 0
Texture2D youWon;
Texture2D youLost;
Texture2D player1Won;
Texture2D player2Won;
Texture2D bg;
int status = 0; //still playing = 0; right won = 1; left won = 2

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
void drawEndScene(const Texture2D& texture);
void adjustBalls(int n);

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Animation / Entities");

    // Assets from @see https://9e0.itch.io/witches-pack
    balls.push_back(new Entity(
        ORIGIN,                       // position
        ballScale,           // scale
        "assets/ball.png",       // texture file addresses
        BALL_SPEED,
        {-1, -1}
    ));

    leftPaddle = new Entity(
        {ORIGIN.x-300, ORIGIN.y},     // position
        paddleScale,           // scale
        "assets/left.png",        // texture file addresses
        400
    );

    rightPaddle = new Entity(
        {ORIGIN.x+300, ORIGIN.y},     // position
        paddleScale,           // scale
        "assets/right.png",       // texture file addresses
        400
    );

    youWon = LoadTexture("assets/youWon.png");
    youLost = LoadTexture("assets/youLost.png");
    player1Won = LoadTexture("assets/player1Won.png");
    player2Won = LoadTexture("assets/player2Won.png");
    bg = LoadTexture("assets/bg.jpg");

    SetTargetFPS(FPS);
}

void adjustBalls(int n) {
    while (balls.size() > n) {
        delete balls.back();
        balls.pop_back();
    }

    while (balls.size() < n) {
        Vector2 offset = {(float)(rand()%40 - 20), (float)(rand()%40-20)};
        Vector2 randomDirection = {(float)(rand()%2 == 0) ? -1.0f : 1.0f, (float)(rand()%2 == 0) ? -1.0f : 1.0f};
        balls.push_back(new Entity(
            {ORIGIN.x-offset.x, ORIGIN.y-offset.y},
            ballScale,
            "assets/ball.png",
            BALL_SPEED,
            randomDirection
        ));
    }
}

void processInput() 
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
    
    if (IsKeyPressed(KEY_T) && status == 0) {
        singleMode = !singleMode;
    }

    float left_y = leftPaddle->getPosition().y;

    if (IsKeyDown(KEY_W) && left_y > paddleScale.y/2-30)
    {
        leftPaddle->moveUp();
    }
    else if (IsKeyDown(KEY_S) && left_y < SCREEN_HEIGHT-paddleScale.y/2) {
        leftPaddle->moveDown();
    }
    else leftPaddle->resetMovement();

    float right_y = rightPaddle->getPosition().y;
    
    if (singleMode) {
        if (right_y < paddleScale.y/2 || right_y> SCREEN_HEIGHT-paddleScale.y/2) rightPaddleDirection = !rightPaddleDirection;
        if (rightPaddleDirection) rightPaddle-> moveDown();
        else rightPaddle->moveUp();
    }
    else {
        if (IsKeyDown(KEY_UP) && right_y > paddleScale.y/2-30) {
            rightPaddle->moveUp();
        }
        else if (IsKeyDown(KEY_DOWN) && right_y < SCREEN_HEIGHT-paddleScale.y/2) {
            rightPaddle->moveDown();
        }
        else rightPaddle->resetMovement();
    }

    if (IsKeyPressed(KEY_ONE)) {
        adjustBalls(1);
    }
    if (IsKeyPressed(KEY_TWO)) {
        adjustBalls(2);
    }
    if (IsKeyPressed(KEY_THREE)) {
        adjustBalls(3);
    }
}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;
    
    if (status == 0) {
        for (Entity* ball : balls) {
            Vector2 ballPos = ball->getPosition();
            Vector2 ballMovement = ball->getMovement();
            if (ballPos.x < ballScale.x/2) {
                status = 1;
                break;
            }
            if (ballPos.x > SCREEN_WIDTH-ballScale.x/2) {
                status = 2;
                break;
            } 
            if (ballPos.y < ballScale.y/2 || ballPos.y > SCREEN_HEIGHT-ballScale.y/2) {
                ball->setMovement({ballMovement.x, ballMovement.y*-1});
            }
            else if (ball->isColliding(leftPaddle)) {
                ball->setMovement({1, ballMovement.y});
                leftPaddle->bump();
            }
            else if (ball->isColliding(rightPaddle)) {
                ball->setMovement({-1, ballMovement.y});
                rightPaddle->bump();
            }

            ball->update(deltaTime);
        }
        leftPaddle->update(deltaTime);
        rightPaddle->update(deltaTime);
    }


    if (status == 1) {
        if (singleMode) {
            drawEndScene(youLost);
        } else {
            drawEndScene(player2Won);
        }
    }
    else if (status == 2) {
        if (singleMode) {
            drawEndScene(youWon);
        } else {
            drawEndScene(player1Won);
        }
    }

}


void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    
    DrawTexturePro(
        bg,
        {0.0f, 0.0f, (float)bg.width, (float)bg.height},
        {ORIGIN.x, ORIGIN.y, SCREEN_WIDTH, SCREEN_HEIGHT},
        ORIGIN,
        0.0f, 
        Color{255, 255, 255, 100}
    );

    if (status == 0) {
        for (Entity* ball : balls) ball->render();
        leftPaddle->render();
        rightPaddle->render();
    }
    
    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();
    
}

void drawEndScene(const Texture2D& texture) {
    Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    Rectangle destination = {ORIGIN.x, ORIGIN.y, (float)texture.width, (float)texture.height};
    Vector2 theOrigin = {(float)texture.width/2, (float)texture.height/2};
    DrawTexturePro(
        texture,
        source,
        destination,
        theOrigin,
        0.0f,
        WHITE
    );
}

int main(void)
{
    srand(static_cast<unsigned int>(time(nullptr)));
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