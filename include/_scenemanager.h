#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include <_scene.h>
//#include <_level1.h>
#include <_level2.h>
#include <_level3.h>
#include <_level4.h>
#include <_menu.h>


class _scenemanager
{
    public:
        _scenemanager();
        virtual ~_scenemanager();
        void switchScene(Scene nextScene);
        void reSizeScene(int width, int height);
        void initScene();
        void drawScene();
        void winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        Scene currentSceneEnum, nextSceneEnum;
    protected:
        int width, height;

    private:
    //_level1* level1;
    _level2* level2;
    _level3* level3;
    _level4* level4;
    _Menu*   menu;
    _Scene* currentScene = nullptr;
};

#endif // _SCENEMANAGER_H
