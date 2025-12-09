#ifndef _LEVEL1_H
#define _LEVEL1_H

#include <_scene.h>


class _level1 : public _Scene
{
    public:
        _level1();
        virtual ~_level1();

        void reSizeScene(int width, int height) override;  // resize window
        void initGL() override;                            // initialize GL graphics
        void drawScene() override;                         // render scene
        int winMsg(HWND,UINT,WPARAM,LPARAM) override;      // to get keyboard interrupts and pass it to inputs

    protected:

    private:
};

#endif // _LEVEL1_H
