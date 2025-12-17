#ifndef _SCENE_H
#define _SCENE_H

#include <windows.h>
#include <vector>
#include "_common.h"


class _Scene {
    public:
        _Scene() {}
        virtual ~_Scene() {}
        virtual void reSizeScene(int width, int height); // resize window
        virtual void initGL() = 0; // initialize GL graphics
        virtual void drawScene() = 0;
        virtual Scene getScene(){return scene;} // render scene
        virtual int winMsg(HWND,UINT,WPARAM,LPARAM) = 0; // to get keyboard interrupts and pass it to inputs
        void showCursor();
        void lockCursor();
        virtual void carryOver(_Scene* prev){};
        virtual void* getPlayer(){return nullptr;}
        virtual void* getInventory(){return nullptr;}
        void setWindow(HWND hWnd) { this->hWnd = hWnd; }
        bool active;
        bool isInit = false;
        bool debug = false;

        int width, height;
        GLuint fontBase;

        Scene scene;
    protected:
        HWND hWnd = nullptr;
    private: };


#endif // _SCENE_H
