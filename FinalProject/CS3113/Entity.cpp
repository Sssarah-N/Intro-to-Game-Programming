#include "Entity.h"

Entity::Entity() : mPosition {0.0f, 0.0f}, mMovement {0.0f, 0.0f}, 
                   mVelocity {0.0f, 0.0f}, mAcceleration {0.0f, 0.0f},
                   mScale {DEFAULT_SIZE, DEFAULT_SIZE},
                   mColliderDimensions {DEFAULT_SIZE, DEFAULT_SIZE}, 
                   mTexture {0}, mTextureType {SINGLE}, mAngle {0.0f},
                   mSpriteSheetDimensions {}, mDirection {RIGHT}, 
                   mWalkAnimations {{}}, mAnimationIndices {}, mFrameSpeed {0},
                   mEntityType {NONE},mIsCollidingBottom(false) { }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath, 
    EntityType entityType) : mPosition {position}, mVelocity {0.0f, 0.0f}, 
    mAcceleration {0.0f, 0.0f}, mScale {scale}, mMovement {0.0f, 0.0f}, 
    mColliderDimensions {scale}, mTexture {LoadTexture(textureFilepath)}, 
    mTextureType {SINGLE}, mDirection {RIGHT}, mWalkAnimations {{}}, 
    mAnimationIndices {}, mFrameSpeed {0}, mSpeed {DEFAULT_SPEED}, 
    mAngle {0.0f}, mEntityType {entityType}, mOriginalPos(position),
    mIsCollidingBottom(false) { }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath, 
        TextureType textureType, Vector2 spriteSheetDimensions, std::map<Direction, 
        std::vector<int>> animationAtlas, EntityType entityType) : 
        mPosition {position}, mVelocity {0.0f, 0.0f}, 
        mAcceleration {0.0f, 0.0f}, mMovement { 0.0f, 0.0f }, mScale {scale},
        mColliderDimensions {scale}, mTexture {LoadTexture(textureFilepath)}, 
        mTextureType {ATLAS}, mSpriteSheetDimensions {spriteSheetDimensions},
        mWalkAnimations {animationAtlas}, mDirection {RIGHT},
        mAnimationIndices {animationAtlas.at(RIGHT)}, 
        mFrameSpeed {DEFAULT_FRAME_SPEED}, mAngle { 0.0f }, 
        mSpeed { DEFAULT_SPEED }, mEntityType {entityType}, mOriginalPos(position),
        mIsCollidingBottom(false) { }

Entity::~Entity() { UnloadTexture(mTexture); };

void Entity::checkCollisionY (std::map<std::string, Entity*> collidableEntities)
{
    for (const auto& pair : collidableEntities)
    {
        // STEP 1: For every entity that our player can collide with...
        Entity *collidableEntity = pair.second;
        if (collidableEntity->getName() == mName) continue;
        
        if (isColliding(collidableEntity) && collidableEntity->mCheckCollision)
        {
            // Track collision for gameplay purposes
            mLastCollidedEntity = collidableEntity;
            // printf("is colliding with: %s\n", collidableEntity->getName().c_str());
            
            // STEP 2: Calculate the distance between its centre and our centre
            //         and use that to calculate the amount of overlap between
            //         both bodies.
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) - 
                              (collidableEntity->mColliderDimensions.y / 2.0f));
            
            // STEP 3: "Unclip" ourselves from the other entity, and zero our
            //         vertical velocity.
            if (mVelocity.y > 0) 
            {
                mPosition.y -= yOverlap;
                mVelocity.y  = 0;
                mIsCollidingBottom = true;
            } else if (mVelocity.y < 0) 
            {
                mPosition.y += yOverlap;
                mVelocity.y  = 0;
                mIsCollidingTop = true;
            }
        }
    }
}

void Entity::checkCollisionX(std::map<std::string, Entity*> collidableEntities)
{
   for (const auto& pair : collidableEntities)
    {
        Entity *collidableEntity = pair.second;
        if (collidableEntity->getName() == mName) continue;
        
        if (isColliding(collidableEntity) && collidableEntity->mCheckCollision)
        {
            // Track collision for gameplay purposes
            mLastCollidedEntity = collidableEntity;
            
            // When standing on a platform, we're always slightly overlapping
            // it vertically due to gravity, which causes false horizontal
            // collision detections. So the solution I found is only resolve X
            // collisions if there's significant Y overlap, preventing the 
            // platform we're standing on from acting like a wall.
            // printf("is colliding with: %s\n", collidableEntity->getName().c_str());
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) - (collidableEntity->mColliderDimensions.y / 2.0f));
            
            // Skip if barely touching vertically (standing on platform)
            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(xDistance - (mColliderDimensions.x / 2.0f) - (collidableEntity->mColliderDimensions.x / 2.0f));

            if (mVelocity.x > 0) {
                mPosition.x     -= xOverlap;
                mVelocity.x      = 0;

                // Collision!
                mIsCollidingRight = true;
            } else if (mVelocity.x < 0) {
                mPosition.x    += xOverlap;
                mVelocity.x     = 0;
 
                // Collision!
                mIsCollidingLeft = true;
            }
        }
    }
}

void Entity::checkCollisionY(Map *map)
{
    if (map == nullptr) return;

    Vector2 topCentreProbe    = { mPosition.x, mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 topLeftProbe      = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 topRightProbe     = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y - (mColliderDimensions.y / 2.0f) };

    Vector2 bottomCentreProbe = { mPosition.x, mPosition.y + (mColliderDimensions.y / 2.0f) };
    Vector2 bottomLeftProbe   = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y + (mColliderDimensions.y / 2.0f) };
    Vector2 bottomRightProbe  = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y + (mColliderDimensions.y / 2.0f) };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    // COLLISION ABOVE (jumping upward)
    if ((map->isSolidTileAt(topCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topLeftProbe, &xOverlap, &yOverlap)   ||
         map->isSolidTileAt(topRightProbe, &xOverlap, &yOverlap)) && mVelocity.y < 0.0f)
    {
        mPosition.y += yOverlap;   // push down
        mVelocity.y  = 0.0f;
        mIsCollidingTop = true;
    }

    // COLLISION BELOW (falling downward)
    if ((map->isSolidTileAt(bottomCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomLeftProbe, &xOverlap, &yOverlap)   ||
         map->isSolidTileAt(bottomRightProbe, &xOverlap, &yOverlap)) && mVelocity.y > 0.0f)
    {
        mPosition.y -= yOverlap;   // push up
        mVelocity.y  = 0.0f;
        mIsCollidingBottom = true;
    } 
}

void Entity::checkCollisionX(Map *map)
{
    if (map == nullptr) return;

    Vector2 leftCentreProbe   = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y };

    Vector2 rightCentreProbe  = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    // COLLISION ON RIGHT (moving right)
    if (map->isSolidTileAt(rightCentreProbe, &xOverlap, &yOverlap) 
         && mVelocity.x > 0.0f && yOverlap >= 0.5f)
    {
        mPosition.x -= xOverlap * 1.01f;   // push left
        mVelocity.x  = 0.0f;
        mIsCollidingRight = true;
    }

    // COLLISION ON LEFT (moving left)
    if (map->isSolidTileAt(leftCentreProbe, &xOverlap, &yOverlap) 
         && mVelocity.x < 0.0f && yOverlap >= 0.5f)
    {
        mPosition.x += xOverlap * 1.01;   // push right
        mVelocity.x  = 0.0f;
        mIsCollidingLeft = true;
    }
}

bool Entity::isColliding(Entity *other) const 
{
    if (!other->isActive() || other == this) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x) - 
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) - 
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void Entity::animate(float deltaTime)
{
    // Select animation based on current state
    switch (mEntityState)
    {
        case WALK:
            mAnimationIndices = mWalkAnimations.at(mDirection);
            break;
        case ATTACK:
            if (!mAttackAnimations.empty())
                mAnimationIndices = mAttackAnimations.at(mEntityState);
            break;
        default:
            mAnimationIndices = mWalkAnimations.at(mDirection);
            break;
    }

    mAnimationTime += deltaTime;
    float framesPerSecond = 1.0f / mFrameSpeed;

    if (mAnimationTime >= framesPerSecond)
    {
        mAnimationTime = 0.0f;

        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
        
        // If in ATTACK state and animation finished, return to WALK
        if (mEntityState == ATTACK && mCurrentFrameIndex == 0)
        {
            mEntityState = WALK;
        }
    }
}

void Entity::AIWander() { 

    // random wandering
    mWanderTimer += GetFrameTime();

    switch (mAIState)
    {
    case IDLE:
        mMovement.x = 0.0f;
        mMovement.y = 0.0f;
        break;

    case WALKING:
    {
        if (mWanderTimer >= mWanderInterval) {
            int shouldMove = GetRandomValue(0, 1);
            
            if (shouldMove == 1) {
                int randomDir = GetRandomValue(0, 3);
                switch (randomDir) {
                    case 0: 
                        mDirection = UP;
                        mMovement.y = -1.0f; 
                        mMovement.x = 0.0f; 
                        break;
                    case 1: 
                        mDirection = DOWN;
                        mMovement.y = 1.0f;  
                        mMovement.x = 0.0f; 
                        break;
                    case 2: 
                        mDirection = LEFT;
                        mMovement.x = -1.0f; 
                        mMovement.y = 0.0f; 
                        break;
                    case 3: 
                        mDirection = RIGHT;
                        mMovement.x = 1.0f;  
                        mMovement.y = 0.0f; 
                        break;
                }
            } else {
                // 50% chance of standing still 
                mMovement.x = 0.0f;
                mMovement.y = 0.0f;
                
                switch (mDirection) {
                    case DOWN:  mCurrentFrameIndex = 1; break;
                    case LEFT:  mCurrentFrameIndex = 1; break;
                    case RIGHT: mCurrentFrameIndex = 1; break;
                    case UP:    mCurrentFrameIndex = 1; break;
                }
            }
            
            mWanderInterval = (float)GetRandomValue(10, 30) / 10.0f;
            mWanderTimer = 0.0f;
        }
        break;
    }
    
    default:
        break;
    }
}


 
void Entity::AIFlyer() { 
    if (mPosition.y >= mOriginalPos.y + 165.0f) mDirection = UP;
    else if (mPosition.y <= mOriginalPos.y) mDirection = DOWN;
 }

void Entity::AIFollow(Entity *target)
{
    if (!target) return;
    
    switch (mAIState)
    {
    case WALKING:
    {
        Vector2 targetPos = target->getPosition();
        float dx = targetPos.x - mPosition.x;
        float dy = targetPos.y - mPosition.y;
        
        float minDist = 40.0f;
        if (abs(dx) < minDist && abs(dy) < minDist) {
            mAIState = IDLE;
            break;
        }
        if (abs(dx) > minDist) {
            if (dx > 0) moveRight();
            else        moveLeft();
        }
        if (abs(dy) > minDist) {
            if (dy > 0) moveDown();
            else        moveUp();
        }
        break;
    }
    
    case IDLE:
    {
        float dist = Vector2Distance(mPosition, target->getPosition());
        if (dist > 20.0f) mAIState = WALKING;
    }
    default:
        break;
    }
}

void Entity::AIActivate(Entity *target)
{
    switch (mAIType)
    {
    case WANDERER:
        AIWander();
        break;

    case FOLLOWER:
        AIFollow(target);
        break;
    
    case FLYER:
        AIFlyer();
        break;
    
    default:
        break;
    }
}

void Entity::update(float deltaTime, Entity *player, std::vector<Map*> maps, 
    std::map<std::string, Entity*> collidableEntities)
{
    if (mEntityStatus == INACTIVE) return;
    
    if (mEntityType == NPC) {
        if (!mTarget) mTarget = player;
        AIActivate(mTarget);
    } 

    resetColliderFlags();

    if (GetLength(mMovement) > 1.0f) 
        normaliseMovement();

    mVelocity.x = mMovement.x * mSpeed;
    
    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y = mMovement.y * mSpeed;
    
    mVelocity.y += mAcceleration.y * deltaTime;
    // if (mAIType != FLYER) mVelocity.y += mAcceleration.y * deltaTime;
    // else {
    //     if (mDirection == UP)
    //         mVelocity = {0, (float)(-mSpeed)};
    //     if (mDirection == DOWN)
    //         mVelocity = {0, (float)(mSpeed)};
    // }

    if (mTextureType == ATLAS && mVelocity.x != 0) {
        if (mVelocity.x < 0) {
            setDirection(LEFT);
        } else {
            setDirection(RIGHT);
        }
    }

    // ––––– JUMPING ––––– //
    if (mIsJumping)
    {
        // STEP 1: Immediately return the flag to its original false state
        mIsJumping = false;
        
        // STEP 2: The player now acquires an upward velocity
        mVelocity.y -= mJumpingPower;
    }

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities);
    for (size_t i = 0; i < maps.size(); ++i) checkCollisionY(maps[i]);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities);
    for (size_t i = 0; i < maps.size(); ++i) checkCollisionX(maps[i]);

    // Animate if: walking on ground OR attacking (regardless of ground/movement)
    if (mTextureType == ATLAS) {
        if ((GetLength(mMovement) != 0) || mEntityState == ATTACK || mAIType == FLYER)
            animate(deltaTime);
    }
}

void Entity::render()
{
    if(mEntityStatus == INACTIVE) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
        case SINGLE:
            // Whole texture (UV coordinates)
            textureArea = {
                // top-left corner
                0.0f, 0.0f,

                // bottom-right corner (of texture)
                static_cast<float>(mTexture.width),
                static_cast<float>(mTexture.height)
            };
            break;
        case ATLAS:
            textureArea = getUVRectangle(
                &mTexture, 
                mAnimationIndices[mCurrentFrameIndex], 
                mSpriteSheetDimensions.x, 
                mSpriteSheetDimensions.y
            );
        
        default: break;
    }

    // // Flip texture horizontally when facing left
    // if (mDirection == LEFT && textureArea.width > 0) {
    //     textureArea.width = -textureArea.width;
    // }

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
        textureArea, destinationArea, originOffset,
        mAngle, WHITE
    );

    displayCollider();
}

void Entity::displayCollider() 
{
    // draw the collision box
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,  
        mPosition.y - mColliderDimensions.y / 2.0f,  
        mColliderDimensions.x,                        
        mColliderDimensions.y                        
    };

    DrawRectangleLines(
        colliderBox.x,      // Top-left X
        colliderBox.y,      // Top-left Y
        colliderBox.width,  // Width
        colliderBox.height, // Height
        GREEN               // Color
    );
}