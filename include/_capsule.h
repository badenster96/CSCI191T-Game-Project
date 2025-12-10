#ifndef _CAPSULE_H
#define _CAPSULE_H

#include "_model.h"
#include "_common.h"
#include "_textureloader.h"



class _capsule : public _model
{
    public:

        _capsule();
        virtual ~_capsule();
        void init();
        void update();
        void spawn(vec3 center);
        void draw();

        CAPSULE_STATE getState() { return state; }

        _textureLoader* myTex = nullptr;
        CAPSULE_STATE state;
        float spawnTime = 0.0f;
        bool hasSpawnedEnemies = false;
        vec3 pos;
    protected:
        float dy, height, radius;

    private:
};

#endif // _CAPSULE_H
