#include "_scenemanager.h"

_scenemanager::_scenemanager()
{
    //ctor
}

_scenemanager::~_scenemanager()
{
    //dtor
}

void _scenemanager::initScene(int scene) {
    switchTo(scene);
    currentScene->initGL();
    // if(scene == 4)
}

void _scenemanager::reSizeScene(int width, int height) {
    currentScene->reSizeScene(width,height);
}
void _scenemanager::drawScene() {
    currentScene->drawScene();
}
void _scenemanager::switchTo(int scene) {
    if(scene == 1) currentScene = level1;
    if(scene == 2) currentScene = level2;
    if(scene == 3) currentScene = level3;
    if(scene == 4) currentScene = menu;
}
void _scenemanager::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    currentScene->winMsg(hWnd,uMsg,wParam, lParam);
}
