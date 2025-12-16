#ifndef _SKYBOX_H
#define _SKYBOX_H

#include<_common.h>
#include<_textureloader.h>

class _skyBox
{
    public:
        _skyBox();
        virtual ~_skyBox();

        _textureLoader *textures = new _textureLoader();

        GLuint tex[7]; // all the images
        vec3 pos;      // position of the skybox
        vec3 rotation; // handle skybox rotations
        vec3 boxSize;  // scale the skybox

        float xMin,yMin,xMax,yMax;
        void skyBoxInit(int skyBoxSize, std::string skyboxName, std::string ext); // load images
        void drawSkyBox();
        void texLoad(std::string skyboxName, std::string ext);
        int size;

    protected:

    private:
};

#endif // _SKYBOX_H
