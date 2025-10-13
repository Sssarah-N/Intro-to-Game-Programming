#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Direction  { LEFT, UP, RIGHT, DOWN }; // For walking

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Direction mDirection;

    Vector2 mScale;
    Vector2 mColliderDimensions;
    
    Texture2D mTexture;
    
    int mFrameSpeed;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    int mSpeed;
    float mAngle;

    void animate(float deltaTime);

public:
    static const int DEFAULT_SIZE        = 250;
    static const int DEFAULT_SPEED       = 200;
    static const int DEFAULT_FRAME_SPEED = 8;

    Entity();;
    Entity(Vector2 position, Vector2 scale, const char* textureFilepaths, 
        int speed = DEFAULT_SPEED, Vector2 movement = {0.0f, 0.0f});
    ~Entity();

    void update(float deltaTime);
    void render();
    void normaliseMovement() { Normalise(&mMovement); };
    bool isColliding(Entity *other) const;
    void bump();

    void moveUp()    { mMovement.y = -1; mDirection = UP;    }
    void moveDown()  { mMovement.y =  1; mDirection = DOWN;  }
    void moveLeft()  { mMovement.x = -1; mDirection = LEFT;  }
    void moveRight() { mMovement.x =  1; mDirection = RIGHT; }

    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mScale;                 }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getAngle()                 const { return mAngle;                 }


    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                }
    void setScale(Vector2 newScale)
        { mScale = newScale;                      }
    void setColliderDimensions(Vector2 newDimensions) 
        { mColliderDimensions = newDimensions;    }
    void setSpeed(int newSpeed)
        { mSpeed  = newSpeed;                     }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                 }
    void setAngle(float newAngle) 
        { mAngle = newAngle;                      }
    
};

#endif // ENTITY_CPP