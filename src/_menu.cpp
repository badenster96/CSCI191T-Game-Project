#include "_menu.h"
#include <GL/gl.h>
#include <iostream>


_Menu::_Menu()
    : width(800), height(600) {}

_Menu::~_Menu() {}

void _Menu::initGL() {
    std::cout << "Initializing Menu" << std::endl;
    scene = MAIN;
    active = true;
    snds->playMusic("sounds/music/Fire&Ice.wav");

    showCursor();

    fontBase = glGenLists(256);
    HDC hDC = wglGetCurrentDC();
    HFONT font = CreateFontA(
        -24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
        "Consolas"
    );

    SelectObject(hDC, font);
    wglUseFontBitmapsA(hDC, 0, 256, fontBase);
    isInit = true;
}

void _Menu::drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    glListBase(fontBase);
    glCallLists((GLsizei)text.length(), GL_UNSIGNED_BYTE, text.c_str());
}

void _Menu::drawScene() {
    glViewport(0, 0, width, height);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
    showCursor();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.3f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    if(hoveredButton == NEWGAME){
        glColor3f(1.0f,1.0f,0.0f);
    } else glColor3f(1.0f,1.0f,1.0f);
    drawText(-0.2f, 0.4f, "NEW GAME");

    if(hoveredButton == HELP) {
        glColor3f(1.0f,1.0f,0.0f);
    } else glColor3f(1.0f,1.0f,1.0f);
    drawText(-0.2f, 0.0f, "HELP");

    if(hoveredButton == QUIT) {
        glColor3f(1.0f,1.0f,0.0f);
    } else glColor3f(1.0f,1.0f,1.0f);
    drawText(-0.2f, -0.4f, "QUIT");

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}
void _Menu::handleMouse(int x, int y, bool click) {
    if (!active || !click) return;

    float ndcY = 1.0f - (2.0f * y / (float)height);
    float buttonHalfHeight = 0.1f;

    if (ndcY < 0.4f + buttonHalfHeight && ndcY > 0.4f - buttonHalfHeight) {
        scene = LEVEL3;
        snds->stopMusic();
        active = false;
        std::cout << "Start Game\n";
    }
    else if (ndcY < 0.0f + buttonHalfHeight && ndcY > 0.0f - buttonHalfHeight) {
        scene = HELPSCENE;
        active = false;
        isInit = false;
        std::cout << "Show Help\n";
    }
    else if (ndcY < -0.4f + buttonHalfHeight && ndcY > -0.4f - buttonHalfHeight) {
        std::cout << "Quit Game\n";
        close();
    }



}
void _Menu::hoverMouse(int mouseX, int mouseY){
    float ndcY = 1.0f - (2.0f * mouseY / (float)height);
    float buttonHalfHeight = 0.1f;

    if (ndcY < 0.4f + buttonHalfHeight && ndcY > 0.4f - buttonHalfHeight) {
        hoveredButton = NEWGAME;
    } else if (ndcY < 0.0f + buttonHalfHeight && ndcY > 0.0f - buttonHalfHeight) {
        hoveredButton = HELP;
    } else if (ndcY < -0.4f + buttonHalfHeight && ndcY > -0.4f - buttonHalfHeight) {
        hoveredButton = QUIT;
    } else hoveredButton = NONE;

}
void _Menu::close(){
    scene = QUITSCENE;
    snds->stopMusic();
    glDeleteLists(fontBase, 256);
}

int _Menu::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            handleMouse(x, y, true);
            break;
        }

        case WM_KEYDOWN: {
            if (wParam == VK_ESCAPE) {
                if (scene == MAIN){
                    close();
                }
                else scene = MAIN;
            }
            break;
        }
        case WM_MOUSEMOVE: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            hoverMouse(x,y);
        }
    }
    return 0;
}

void _Menu::reSizeScene(int w, int h) {
    width = w;
    height = h;
}

