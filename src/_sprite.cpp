#include "_sprite.h"

_sprite::_sprite()
{
    //ctor

    xMin = yMin =0.0;
    xMax = yMax =1.0;

    rot.x = rot.y = rot.z = 0;

    pos.x =0;
    pos.y =-0.5;
    pos.z = -3;

    vertX[0].x = -0.5; vertX[0].y = -0.5; vertX[0].z=1;
    vertX[1].x =  0.5; vertX[1].y = -0.5; vertX[1].z=1;
    vertX[2].x =  0.5; vertX[2].y =  0.5; vertX[2].z=1;
    vertX[3].x = -0.5; vertX[3].y =  0.5; vertX[3].z=1;
}

_sprite::~_sprite()
{
    //dtor
}

void _sprite::spriteInit(const char* fileName, int xFrames, int yFrames)
{
   myTex->loadTexture(fileName);
   frames.x = xFrames;
   frames.y = yFrames;

   xMin =0;
   yMin =0;
   xMax = 1.0/xFrames;
   yMax = 1.0/yFrames;
}

void _sprite::drawSprite()
{
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1, 1, 1, 1);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glPushMatrix();

        glTranslatef(pos.x,pos.y,pos.z);
        glRotatef(rot.y,0,1,0);
        myTex->bindTexture();
        glBegin(GL_QUADS);
            glTexCoord2f(xMin,yMax);
            glVertex2f(vertX[0].x,vertX[0].y);

            glTexCoord2f(xMax,yMax);
            glVertex2f(vertX[1].x,vertX[1].y);

            glTexCoord2f(xMax,yMin);
            glVertex2f(vertX[2].x,vertX[2].y);

            glTexCoord2f(xMin,yMin);
            glVertex2f(vertX[3].x,vertX[3].y);

        glEnd();

    glPopMatrix();
    glPopMatrix();
}

void _sprite::spriteActions()
{
    switch(actionTrigger)
    {
        case STAND:
            xMin =0;
            yMin =0;

            xMax=1.0/frames.x;
            yMax=1.0/frames.y;

        break;

        case WALKLEFT:
             xMax =0;
             xMin = 1.0/frames.x;

        break;

        case WALKRIGHT:

          //  if(xMin<1)

             xMin +=1.0/frames.x;
             xMax +=1.0/frames.x;

            if(xMax>=1)
            {
              yMin +=1.0/frames.y;
              yMax +=1.0/frames.y;

              xMin =0;
              xMax = 1.0/frames.x;
            }

        break;

        case WALKFRONT:
        break;

        case WALKBACK:
        break;
    }
}

void _sprite::face(vec3 toCamera) {
    vec3 toPlayer;
    toPlayer.x = toCamera.x - pos.x;
    toPlayer.y = 0; // ignore vertical rotation for 2D sprites
    toPlayer.z = toCamera.z - pos.z;
    rot.y = atan2f(toPlayer.x, toPlayer.z) * 180.0f / PI;
}
