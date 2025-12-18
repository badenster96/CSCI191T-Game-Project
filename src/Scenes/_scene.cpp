#include "Scenes/_scene.h"


void _Scene::reSizeScene(int width, int height)
{
    float aspectRatio = (float)width/(float)height;
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, aspectRatio,0.1,1000.0);

    this->width = width;
    this->height= height;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void _Scene::showCursor() {
    ClipCursor(NULL);
    while(ShowCursor(TRUE) < 0);
}

void _Scene::lockCursor(){
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    POINT ul = { rcClient.left, rcClient.top };
    POINT lr = { rcClient.right, rcClient.bottom };
    ClientToScreen(hWnd, &ul);
    ClientToScreen(hWnd, &lr);
    RECT clipRect = { ul.x, ul.y, lr.x, lr.y };
    ClipCursor(&clipRect);
    while(ShowCursor(FALSE) > 0);
}
