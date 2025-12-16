#ifndef _MENU_H
#define _MENU_H

#include "_scene.h"
#include <string>
#include <Windows.h>
#include <_sounds.h>

class _scenemanager;
extern _scenemanager sceneManager;


class _Menu : public _Scene {
public:

    _Menu();
    ~_Menu();
    _sounds* snds = new _sounds();

    void initGL() override;
    void drawScene() override;
    int winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    void reSizeScene(int w, int h) override;

private:
    void drawText(float x, float y, const std::string& text);
    void handleMouse(int x, int y, bool click);

    int width, height;
};

#endif // _MENU_H
