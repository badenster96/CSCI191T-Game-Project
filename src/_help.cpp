#include "_help.h"
#include <GL/gl.h>
#include <iostream>

_Help::_Help()
    : width(800), height(600) {scene = HELPSCENE;}

_Help::~_Help() {}

void _Help::initGL() {
    std::cout << "Initializing Help Screen\n";
    active = true;

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

void _Help::drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    glListBase(fontBase);
    glCallLists((GLsizei)text.length(), GL_UNSIGNED_BYTE, text.c_str());
}

void _Help::drawScene() {
    glViewport(0, 0, width, height);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                 GL_TEXTURE_BIT | GL_LIGHTING_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Background
    glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.3f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    // Title
    drawText(-0.25f, 0.75f, "CONTROLS");

    // Controls list
    drawText(-0.6f, 0.45f, "W / A / S / D  - Move");
    drawText(-0.6f, 0.30f, "Mouse          - Look");
    drawText(-0.6f, 0.15f, "This is an auto shooter,");
    drawText(-0.6f, 0.00f, "You automatically shoot at the nearest enemy.");
    drawText(-0.6f, -0.15f, "Your pullets can penetrate enemies");
    drawText(-0.6f, -0.30f, "And critically strike them!");
    drawText(-0.6f, -0.45f, "ESC            - Back to Menu");

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}

int _Help::winMsg(HWND, UINT uMsg, WPARAM wParam, LPARAM) {
    switch (uMsg) {
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                scene = MAIN;
                active = false;
                return 0;
            }
            break;
    }
    return 0;
}

void _Help::reSizeScene(int w, int h) {
    width = w;
    height = h;
}
