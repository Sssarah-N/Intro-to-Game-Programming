#include "Entity.h"

Entity::Entity() : mPosition {0.0f, 0.0f}, mMovement {0.0f, 0.0f},
                   mScale {DEFAULT_SIZE, DEFAULT_SIZE},
                   mColliderDimensions {DEFAULT_SIZE, DEFAULT_SIZE}, 
                   mFrameSpeed {0} {}

Entity::Entity(Vector2 position, Vector2 scale, 
    const char* textureFilepaths, int speed, Vector2 movement) :
    mPosition {position}, 
    mMovement (movement), mScale {scale}, mColliderDimensions {scale}, 
    mFrameSpeed {DEFAULT_FRAME_SPEED}, mAngle { 0.0f }, 
    mSpeed (speed)
    {
        mTexture = LoadTexture(textureFilepaths);
        
    }

Entity::~Entity() 
{
    UnloadTexture(mTexture); 
};

/**
 * Checks if two entities are colliding based on their positions and collider 
 * dimensions.
 * 
 * @param other represents another Entity with which you want to check for 
 * collision. It is a pointer to the Entity class.
 * 
 * @return returns `true` if the two entities are colliding based on their
 * positions and collider dimensions, and `false` otherwise.
 */
bool Entity::isColliding(Entity *other) const 
{
    float xDistance = fabs(mPosition.x - other->getPosition().x) - 
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) - 
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void Entity::bump() { 
    Vector2 originalScale = mScale;
    while (mScale.x < originalScale.x*1.3) {
        mScale.x *= 1.05;
        mScale.y *= 1.05;
    }
    while (mScale.x > originalScale.x) {
        mScale.x *= 0.95;
        mScale.y *= 0.95;
    }
    mScale = originalScale;
}


/**
 * Updates the current frame index of an entity's animation based on the 
 * elapsed time and frame speed.
 * 
 * @param deltaTime represents the time elapsed since the last frame update.
 */
void Entity::animate(float deltaTime)
{
    mAnimationTime += deltaTime;
    float framesPerSecond = 1.0f / mFrameSpeed;

    if (mAnimationTime >= framesPerSecond)
    {
        mAnimationTime = 0.0f;
    }
}

void Entity::update(float deltaTime)
{
    mPosition = {
        mPosition.x + mSpeed * mMovement.x * deltaTime,
        mPosition.y + mSpeed * mMovement.y * deltaTime
    };
}

void Entity::render()
{
    Rectangle textureArea = {
            0.0f,
            0.0f,
            (float)mTexture.width,
            (float)mTexture.height
        };

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        mTexture, 
        textureArea, 
        destinationArea, 
        originOffset,
        mAngle, WHITE
    );
}