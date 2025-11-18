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
    level1 = new _level1();
    level2 = new _level2();
    level3 = new _level3();
    menu   = new _Menu();
    currentSceneEnum = MAIN;
    currentScene = menu;
}

_scenemanager::~_scenemanager()
{
    delete level1;
    delete level2;
    delete level3;
    delete menu;
}

void _scenemanager::switchScene() {
    if(currentScene) currentScene->isInit = false;
    if(currentSceneEnum == LEVEL1) currentScene = level1;
    else if(currentSceneEnum == LEVEL2) currentScene = level2;
    else if(currentSceneEnum == LEVEL3) currentScene = level3;
    else if(currentSceneEnum == MAIN)   currentScene = menu;
}
// The main functions main interacts with
// Kept as simple as possible, to avoid confusion
// All these do is pass the initialize and draw functions to the current scene
void _scenemanager::initScene() {
    // makes sure the currently selected scene is whats being initialized
    std::cout << "Initialized " << currentSceneEnum << std::endl;
    currentScene->initGL();
}
void _scenemanager::drawScene() {
    std::cout << "Drawing Scene " << currentSceneEnum << std::endl;
    // Read in the enum from the current state
    if(currentSceneEnum != currentScene->getScene()){
            currentSceneEnum = currentScene->getScene(); // Checks for Enum state changes within the scene thats being drawn
            switchScene(); // Switches the enum with the enum from the scene, to check if the scenes going to be switched
    }
    if(currentScene){

        // make sure the scene is initialized
        if(!currentScene->isInit) initScene(); // Initializes scene if the new scene mismatches the old scene
        currentScene->drawScene();
    }
}

// Resize scene
void _scenemanager::reSizeScene(int width, int height) {
    currentScene->reSizeScene(width,height);
}

void _scenemanager::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    currentScene->winMsg(hWnd,uMsg,wParam, lParam);
}
