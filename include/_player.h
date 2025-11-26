#ifndef _PLAYER_H
#define _PLAYER_H

#include <_3dmodelloader.h>
#include <_camera.h>
#include <_common.h>


class _player : public _3DModelLoader
{
    public:
        _player();
        virtual ~_player();
        // Getters
        vec3& getPos() {return pos;}
        // Main Functions
        void draw();
        void init(const char* model);
        void update();
        void handleInput(UINT uMsg, WPARAM wParam, LPARAM lParam, HWND hWnd);
        // Helper Functions
        void applyPlayerStats();
        vec3 calcMoveVector();
        void rotatePlayer(const vec3& normVec);
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
        float currHealth, maxHealth, attackSpeed, damage, critChance, armor, armorPiercing, movementSpeed = 0;
        // Animation
        float iFrames = 0.3f;

        // Objects
        _camera cam;

    protected:

    private:
};

#endif // _PLAYER_H
