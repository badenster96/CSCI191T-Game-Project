#include "_level1.h"

_level1::_level1()
{
    //ctor
}

_level1::~_level1()
{
    //dtor
}
void _level1::reSizeScene(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->width = width;
    this->height = height;
}
void _level1::initGL(){

}

void _level1::drawScene() {

}

int _level1::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
