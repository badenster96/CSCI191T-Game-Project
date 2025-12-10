#ifndef _CAPSULEHANDLER_H
#define _CAPSULEHANDLER_H

#include <vector>

#include <_capsule.h>
#include <_common.h>


class _capsuleHandler
{
    public:
        _capsuleHandler();
        virtual ~_capsuleHandler();

        void setup(int quantity);
        void draw();
        void update();
        void capsuleSpawner(int numCaps, vec3 point);
        std::vector<_capsule*> capsules;
    protected:

    private:
};

#endif // _CAPSULEHANDLER_H
