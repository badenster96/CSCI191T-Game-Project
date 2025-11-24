#include "_camera.h"

_camera::_camera()
{
    //ctor
}

_camera::~_camera()
{
    //dtor
}
void _camera::camInit()
{
    eye.x =0; eye.y =0; eye.z =10;
    des.x =0; des.y =0; des.z =-10;
    up.x  =0; up.y  =1; up.z  =0;

    step =0.05;

    distance = sqrt(pow(eye.x-des.x,2)+pow(eye.y-des.y,2)+pow(eye.z-des.z,2));

    rotAngle.x =0;
    rotAngle.y =0;
}

void _camera::camReset()
{
    eye.x =0; eye.y =0; eye.z =10;
    des.x =0; des.y =0; des.z =0;
    up.x  =0; up.y  =1; up.z  =0;

    step =0.5;

    distance = sqrt(pow(eye.x-des.x,2)+pow(eye.y-des.y,2)+pow(eye.z-des.z,2));

    rotAngle.x =0;
    rotAngle.y =0;
}

void _camera::rotateXY()
{
    float radX = rotAngle.x * PI / 180.0f;
    float radY = rotAngle.y * PI / 180.0f;
    eye.x = des.x + distance*cos(radY)*sin(radX);
    eye.y = des.y + distance*sin(radY);
    eye.z = des.z + distance*cos(radY)*cos(radX);
}

void _camera::rotateUP()
{

}

void _camera::camMoveFdBd(int dir)
{
       eye.z += step*dir; // if forward dir =1 else dir =-1
       des.z += step*dir;
}

void _camera::camMoveLtRt(int dir)
{
    eye.x += step*dir;
    des.x += step*dir;
}

void _camera::setUpCamera()
{
    gluLookAt(eye.x,eye.y,eye.z,
              des.x,des.y,des.z,
              up.x, up.y, up.z);
}
