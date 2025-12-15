#ifndef _CAPSULEHANDLER_H
#define _CAPSULEHANDLER_H

#include <vector>

#include <_capsule.h>
#include <_common.h>
#include <_collisioncheck.h>


class _capsuleHandler
{
    public:
        _capsuleHandler();
        virtual ~_capsuleHandler();

        _collisionCheck* myCol = new _collisionCheck;

        void setup(int quantity);
        void draw();
        void update();
        void reset();
        void capsuleSpawner(int numCaps, vec3 point);
        bool checkPickup(vec3 p, _collisionCheck *myCol);
        float spawnTime = 0.0f;
        float spawnInterval = 2.0f;
        int numCapsulesSpawned = 0;
        std::vector<_capsule*> capsules;
    protected:

    private:
};

#endif // _CAPSULEHANDLER_H
