#include "_menu.h"
#include <GL/gl.h>
#include <iostream>


_Menu::_Menu()
    : width(800), height(600) {}

_Menu::~_Menu() {}

void _Menu::initGL() {
    std::cout << "Initializing Menu" << std::endl;
    scene = MAIN;
    isInit = true;
    active = true;

    HDC hDC = wglGetCurrentDC();
    HFONT font = CreateFontA(
        -24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
        "Arial"
    );

    SelectObject(hDC, font);
    wglUseFontBitmapsA(hDC, 0, 256, 1000);
}

void _Menu::drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    glListBase(1000);
    glCallLists((GLsizei)text.length(), GL_UNSIGNED_BYTE, text.c_str());
}

void _Menu::drawScene() {
    glViewport(0, 0, width, height);
    // --- Save all relevant GL state ---
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);

    // --- Save matrices ---
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // --- Clear depth so menu draws on top ---
    glDisable(GL_DEPTH_TEST);

    // --- Set up 2D orthographic projection ---
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);  // Coordinates from -1 to 1 in X and Y
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // --- Disable lighting, textures, etc. for clean text rendering ---
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

        // Draw background
    glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.3f); // background color
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();
    // --- Draw your menu ---
    glColor3f(1.0f, 1.0f, 1.0f);


    drawText(-0.2f, 0.4f, "NEW GAME");
    drawText(-0.2f, 0.0f, "HELP");
    drawText(-0.2f, -0.4f, "QUIT");

    // --- Restore matrices ---
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // --- Restore all GL state ---
    glPopAttrib();
}
void _Menu::handleMouse(int x, int y, bool click) {
    if (!active || !click) return;

    float ndcY = 1.0f - (2.0f * y / (float)height);
    float buttonHalfHeight = 0.1f;

    if (ndcY < 0.4f + buttonHalfHeight && ndcY > 0.4f - buttonHalfHeight) {
        scene = LEVEL1;
        active = false;
        std::cout << "Start Game\n";
    }
    else if (ndcY < 0.0f + buttonHalfHeight && ndcY > 0.0f - buttonHalfHeight) {
        scene = CREDITS;
        active = false;
        std::cout << "Show Help\n";
        // Optionally, switch to a credits scene if you have one
    }
    else if (ndcY < -0.4f + buttonHalfHeight && ndcY > -0.4f - buttonHalfHeight) {
        scene = QUIT;
        std::cout << "Quit Game\n";
        PostQuitMessage(0);
    }
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
                if (scene == CREDITS || scene == LEVEL1) {
                    scene = MAIN;
                }
            }
            break;
        }
    }
    return 0;
}

void _Menu::reSizeScene(int w, int h) {
    width = w;
    height = h;
}

