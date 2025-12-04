// The Scene Manager'
// The scene manager is a simple shell that encapsulates all of the scenes, which include the menu, levels, and credits.
// Its only job is to switch between the scenes, and then pass initializing and drawing commands to the currently selected scene
// It does this with the currentScene pointer, which points to the current scene
// this is controlled by reading the currentSceneEnum every draw frame from the scene it is drawing, since its the scenes that direct them to other scenes

// for example, the menu scene holds a similar scene enum, and when the scene enum within the menu changes, scenemanager reads that, initializes the new scene,
// and draws the new scene instead

#include "_scenemanager.h"

_scenemanager::_scenemanager()
{
    //ctor
    // Initialize defined levels here
    //level1 = new _level1();
    level2 = new _level2();
    level3 = new _level3();
    level4 = new _level4();
    menu   = new _Menu();
    currentSceneEnum = MAIN;
    currentScene = menu;
}

_scenemanager::~_scenemanager()
{
    // delete level1;
    delete level2;
    delete level3;
    delete level4;
    delete menu;
}

// Helper function that reads the sceneManager enum and changes the level to that value
void _scenemanager::switchScene(Scene nextScene) {
    // if(currentSceneEnum == LEVEL1) currentScene = level1;
    //else
    if(currentSceneEnum == LEVEL2) currentScene = level2;
    else if(currentSceneEnum == LEVEL3) currentScene = level3;
    else if(currentSceneEnum == LEVEL4) currentScene = level4;
    else if(currentSceneEnum == MAIN)   currentScene = menu;
    currentScene->isInit = false;
}
// The main functions main interacts with
// Kept as simple as possible, to avoid confusion
// All these do is pass the initialize and draw functions to the current scene
void _scenemanager::initScene() {
    currentScene->initGL();
}
void _scenemanager::drawScene() {
    Scene requested = currentScene->getScene();
    if(requested != currentSceneEnum) {
            currentSceneEnum = requested;
            switchScene(currentSceneEnum);
    }
    if(currentScene && !currentScene->isInit) {
        currentScene->initGL();
        currentScene->reSizeScene(width, height);
    }
    if(currentScene) currentScene->drawScene();
}

// Resize scene
void _scenemanager::reSizeScene(int nWidth, int nHeight) {
    width = nWidth;
    height = nHeight;
    currentScene->reSizeScene(width,height);
}

void _scenemanager::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(currentScene)currentScene->setWindow(hWnd);
    currentScene->winMsg(hWnd,uMsg,wParam, lParam);
}
