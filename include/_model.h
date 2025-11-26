#ifndef _MODEL_H
#define _MODEL_H

#include<_common.h>
#include <_textureloader.h>

class _model
{
    public:
        _model();
        virtual ~_model();

        double rotateX;
        double rotateY;
        double rotateZ;

        double posX;
        double posY;
        double posZ;

        double scale;

        vec3 p;
        _textureLoader myTex;

        void drawModel();

    protected:

    private:
};

#endif // _MODEL_H
