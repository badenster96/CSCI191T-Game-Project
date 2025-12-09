#ifndef _ENEMY_H
#define _ENEMY_H

#include <_common.h>
#include <_3dmodelloader.h>


class _enemy : public _3DModelLoader
{
    public:
        _enemy();
        virtual ~_enemy();
        void init(const char* filename);
        void draw();
        int ReadMD2Model(const char* filename, struct md2_model_t* mdl)override;

        void spawn(vec3 center);

        void moveTowardPoint(vec3 point);

        bool isAlive = false;
        bool isSpawned = false;
        float angle;

        //Animation
        float pframe = 0.0f;

        float health, damage, speed = 0;
        float iFrames, lastTimeHit;


    protected:

    private:
};

#endif // _ENEMY_H
