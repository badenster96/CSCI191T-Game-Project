#ifndef _ENEMY_H
#define _ENEMY_H

#include <_common.h>
#include <_3dmodelloader.h>


class _enemy : public _3DModelLoader
{
    public:
        _enemy();
        virtual ~_enemy();

        void spawn(vec3 center);

        void moveTowardPoint(vec3 point);

        int damage, health;
        bool isAlive, isSpawned;

    protected:

    private:
};

#endif // _ENEMY_H
