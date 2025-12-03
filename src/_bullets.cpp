#include "_bullets.h"


_bullets::_bullets()
{
    //ctor
    pos.x = pos.y = pos.z =0;
    t=0;
    src.x= src.y= src.z =0;
    des.x=0;
    des.y=20;
    des.z=-50;

    live= false;
}

_bullets::~_bullets()
{
    //dtor
}
void _bullets::iniBullet(char* filename)
{
   // if we use texture on bullets
   spearModel->initModel(filename);
}

void _bullets::drawBullet()
{
    //glDisable(GL_TEXTURE_2D);// only if you are using glut sphere
    glPushMatrix();

     if(live)
     {
         glTranslatef(pos.x,pos.y,pos.z);
         glScalef(0.1f,0.1f,0.1f);
         // glutSolidSphere(0.5,20,20);
     }
     spearModel->Draw();

     glPopMatrix();
     //glEnable(GL_TEXTURE_2D);// only if you are using glut sphere
}

// In _bullets.cpp, define it:
void _bullets::shootBullet(const vec3& start, const vec3& end)
{
    src = start;        // starting position
    des = end;          // destination
    pos = start;        // bullet starts at src
    t = 0.0f;           // reset interpolation
    live = true;        // activate the bullet
    actionTrigger = SHOOT; // set state to shooting
}
void _bullets::bulletActions(){
    switch(actionTrigger)
    {
    case READY:
        pos.x=0;
        pos.y=0;
        pos.z=0;

        des.x =0;
        des.y= 0;
        des.z = 0;

        live =false;
        t=0;
        break;

    case SHOOT:   // applying parametric equation
    // p = p1+t(p2-p1)

        if(live)
        {
            pos.x = src.x + t*(des.x -src.x);
            pos.y = src.y + t*(des.y -src.y);
            pos.z = src.z + t*(des.z -src.z);

            if(t<1) t+=0.05;
            else actionTrigger = READY;
        }
        break;

    case HIT:
        break;
    }
}
