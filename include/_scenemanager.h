#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include <_scene.h>
#include <_level1.h>
#include <_level2.h>
#include <_level3.h>
#include <_menu.h>


class _scenemanager
{
    public:
        _scenemanager();
        virtual ~_scenemanager();

        void initScene();
        void reSizeScene(int width, int height);
        void drawScene();
        void switchTo(Scene scene);
        Scene getCurrentScene();
        void winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        Scene currentSceneEnum;
    protected:

    private:
    _level1* level1 = new _level1();
    _level2* level2 = new _level2();
    _level3* level3 = new _level3();
    _Menu*   menu   = new _Menu();
    _Scene* currentScene = nullptr;
};

#endif // _SCENEMANAGER_H
