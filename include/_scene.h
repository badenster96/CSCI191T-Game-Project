#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>
#include<_light.h>
#include<_model.h>
#include<_inputs.h>
#include<_textureloader.h>
#include<_parallax.h>
#include<_skybox.h>
#include<_sprite.h>
#include<_timer.h>
#include<_3dmodelloader.h>
#include<_camera.h>
#include<_bullets.h>
#include<_collisioncheck.h>
#include<_sounds.h>


class _Scene
{
    public:

        virtual void reSizeScene(int width, int height) = 0;  // resize window
        virtual void initGL() = 0;                            // initialize GL graphics
        virtual void drawScene() = 0;
        virtual Scene getScene(){return scene;}
                                 // render scene
        virtual int winMsg(HWND,UINT,WPARAM,LPARAM) = 0;      // to get keyboard interrupts and pass it to inputs
        bool active;
        bool isInit = false;

        Scene scene;
    protected:

    private:
};

#endif // _SCENE_H
