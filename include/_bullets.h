#ifndef _BULLETS_H
#define _BULLETS_H

#include<_common.h>
#include<_textureloader.h>
#include <_3DModelLoader.h>
#include<_timer.h>

class _bullets
{
    public:
        _bullets();
        virtual ~_bullets();

        _3DModelLoader         *spearModel = new _3DModelLoader();
        _textureLoader *tex = new _textureLoader();
        _timer         *mytime   = new _timer();

        vec3 pos;
        vec3 des;
        vec3 src;

        vec3 scale;
        float t=0; // for parametric equation
        bool live = true;

        void iniBullet(char *);
        void drawBullet();
        void bulletActions();
        void shootBullet(const vec3& start, const vec3& end);

        int actionTrigger;

        enum{READY,SHOOT,HIT};

    protected:

    private:
};

#endif // _BULLETS_H
