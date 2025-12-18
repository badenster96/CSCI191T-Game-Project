#include "Graphics/_parallax.h"



_parallax::_parallax()
{
    //ctor
    xMin = yMin = 0.0;
    xMax = yMax = 1.0;

    mytime->reset();
}

_parallax::~_parallax()
{
    //dtor
}
void _parallax::drawParallax(float w, float h)
{
    glPushAttrib(GL_ENABLE_BIT);       // Save current states
    glDisable(GL_DEPTH_TEST);          // Draw on top
    glDisable(GL_LIGHTING);            // Ignore lighting

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);       // 2D screen coordinates

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            glColor3f(1.0f, 1.0f, 1.0f);
            btex->bindTexture();

            glBegin(GL_QUADS);
                glTexCoord2f(xMin, yMax); glVertex2f(0, 0);
                glTexCoord2f(xMax, yMax); glVertex2f(w, 0);
                glTexCoord2f(xMax, yMin); glVertex2f(w, h);
                glTexCoord2f(xMin, yMin); glVertex2f(0, h);
            glEnd();
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib(); // Restore states
    glMatrixMode(GL_MODELVIEW);
}


void _parallax::parallaxInit(char* fileName)
{
    btex->loadTexture(fileName);
}

void _parallax::prlxScrollAuto(string dir , float speed)
{
    if(mytime->getTicks() > 40)
    {
        if(dir == "left")  { xMin += speed; xMax += speed; }
        else if(dir == "right"){ xMin -= speed; xMax -= speed; }
        else if(dir == "up")   { yMin -= speed; yMax -= speed; }
        else if(dir == "down") { yMin += speed; yMax += speed; }

        // Wrap coordinates to [0,1] to repeat texture
        if(xMin > 1.0f) { xMin -= 1.0f; xMax -= 1.0f; }
        if(xMax < 0.0f) { xMin += 1.0f; xMax += 1.0f; }
        if(yMin > 1.0f) { yMin -= 1.0f; yMax -= 1.0f; }
        if(yMax < 0.0f) { yMin += 1.0f; yMax += 1.0f; }

        mytime->reset();
    }
}
