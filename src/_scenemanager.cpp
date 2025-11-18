#include "_scenemanager.h"

_scenemanager::_scenemanager()
{
    //ctor
    currentSceneEnum = MAIN;
}

_scenemanager::~_scenemanager()
{
    //dtor
}
//main init function, called each time the window is created
// initalize the scene only once, loading all of its assets whenever the window is created
void _scenemanager::initScene() {
    switchTo(currentSceneEnum);
    currentScene->initGL();
}

// Resize scene
void _scenemanager::reSizeScene(int width, int height) {
    currentScene->reSizeScene(width,height);
}
// passes the draw function to the current scene
// main draw function, is called in the main loop
void _scenemanager::drawScene() {
    currentSceneEnum = currentScene->getScene();
    switchTo(currentSceneEnum);
    currentScene->drawScene();
}
// moves the currentScene pointer
void _scenemanager::switchTo(Scene scene) {
    // currentScene->getScene();
    currentSceneEnum = scene;
    if(scene == LEVEL1) {
        currentScene = level1;
    }
    if(scene == LEVEL2) {
        currentScene = level2;
    }
    if(scene == LEVEL3) {
        currentScene = level3;
    }
    if(scene == MAIN) {
        currentScene = menu;
    }
    if(!currentScene->isInit) {
        currentScene->initGL();
        currentScene->isInit = true;
    }
}
Scene _scenemanager::getCurrentScene() {
    return currentSceneEnum;
}
void _scenemanager::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    currentScene->winMsg(hWnd,uMsg,wParam, lParam);
}
