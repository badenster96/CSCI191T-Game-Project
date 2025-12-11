#ifndef _PLAYER_H
#define _PLAYER_H

#include <_3dmodelloader.h>
#include <_camera.h>
#include <_common.h>


class _player
{
    public:
        _player();
        virtual ~_player();
        _3DModelLoader* playerModel = new _3DModelLoader;
        _textureLoader* pTex = new _textureLoader;
        // Getters
        vec3& getPos() {return pos;}
        // Main Functions
        void draw();
        void init(std::string model);
        void update(float deltaTime);
        void handleInput(UINT uMsg, WPARAM wParam, LPARAM lParam, HWND hWnd);
        // Helper Functions
        void applyPlayerStats();
        void applyItemStats();
        vec3 calcMoveVector();
        void rotatePlayer(const vec3& normVec, float deltaTime);
        void resetPlayer();
        void hit(float hitDamage);
        void setTarget(vec3 point);

        vec3 currTarget;
        // Movement
        bool isMovingForward, isMovingBack, isMovingLeft, isMovingRight;
        bool isJumping;
        bool isMoving;
        // Camera Controls
        float currentAngle;
        float camHeightOffset;
        // Player Stats
        std::unordered_map<std::string,float> stats;
        std::unordered_map<std::string,float> itemStats;
        float currHealth, maxHealth, attackSpeed, damage, critChance, critDamage, armor, piercing, movementSpeed, range = 0;
        // Animation
        float iFrames = 0.3f;

        // Objects
        _camera cam;
        vec3 pos;
        enum {STAND, WALKLEFT,WALKRIGHT,RUN,JUMP,ATTACK,PAIN};

    protected:

    private:
};

#endif // _PLAYER_H
