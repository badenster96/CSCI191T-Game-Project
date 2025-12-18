#ifndef _HELP_H
#define _HELP_H

#pragma once
#include <string>
#include <Scenes/_scene.h>
#include <windows.h>

class _Help : public _Scene{
public:
    _Help();
    ~_Help();

    void initGL();
    void drawScene();
    void reSizeScene(int w, int h);
    int  winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    void drawText(float x, float y, const std::string& text);

    int width;
    int height;
    bool isInit;
};

#endif // _HELP_H
