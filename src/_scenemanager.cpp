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
    // switchTo(currentSceneEnum);
    std::cout << "Initializing scene" << std::endl;
    if(currentSceneEnum == LEVEL1) currentScene = level1;
    else if(currentSceneEnum == LEVEL2) currentScene = level2;
    else if(currentSceneEnum == LEVEL3) currentScene = level3;
    else if(currentSceneEnum == MAIN)   currentScene = menu;

    // Initializes a scene if it isnt initialized
    if(!currentScene->isInit) currentScene->initGL();
    // currentScene->initGL();
}
void _scenemanager::drawScene() {
    Scene requested = currentScene->getScene();
    if(requested != currentSceneEnum) {
            currentSceneEnum = requested;
            initScene();
    }
    currentScene->drawScene();
}

// Resize scene
void _scenemanager::reSizeScene(int width, int height) {
    currentScene->reSizeScene(width,height);
}
// passes the draw function to the current scene
// main draw function, is called in the main loop

void _scenemanager::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    currentScene->winMsg(hWnd,uMsg,wParam, lParam);
}
