#include "_scene.h"


void _Scene::reSizeScene(int width, int height)
{
    float aspectRatio = (float)width/(float)height;// keep track of the ratio
    glViewport(0,0,width,height); // adjust my viewport

    glMatrixMode(GL_PROJECTION);  // To setup ptrojection
    glLoadIdentity();             // calling identity matrix
    gluPerspective(45, aspectRatio,0.1,1000.0); // setting perspective projection

    this->width = width;
    this->height= height;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             // calling identity matrix
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
