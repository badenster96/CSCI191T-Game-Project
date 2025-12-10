#ifndef _ENEMY_H
#define _ENEMY_H

#include <_common.h>
#include <_3dmodelloader.h>


class _enemy
{
    public:
        _enemy();
        virtual ~_enemy();
        _3DModelLoader* mdl = new _3DModelLoader;
        _textureLoader* tex = new _textureLoader;
        void init(std::string filename);
        void update();
        void draw();

        void spawn(vec3 center);
        void pain();

        void moveTowardPoint(vec3 point);

        bool isAlive = false;
        bool isSpawned = false;
        float angle;

        //Animation
        float pframe = 0.0f;
        enum {STAND, WALKLEFT,WALKRIGHT,RUN,JUMP,ATTACK,PAIN};
        bool isInPain = false;
        bool isFlashing = false;
        float painStartTime = 0.0f;
        float flashStartTime = 0.0f;
        float flashDuration = 0.015f;

        float health, damage, speed = 0;
        float iFrames, lastTimeHit;
        vec3 pos;


    protected:

    private:
};

#endif // _ENEMY_H
