#include "_collisioncheck.h"

_collisionCheck::_collisionCheck()
{
    //ctor
}

_collisionCheck::~_collisionCheck()
{
    //dtor
}

bool _collisionCheck::isLinearCol(vec3, vec3)
{

}

bool _collisionCheck::isRadialCol(vec2 p1, vec2 p2, float r1, float r2, float thrhld)
{
   return(sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2))-(r1+r2)< thrhld);

}

bool _collisionCheck::isSphereCol(vec3 p1, vec3 p2, float r1, float r2, float thrhld)
{
  return(sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2))-(r1+r2)< thrhld);
}

bool _collisionCheck::isPlanoCol(vec2, vec2)
{

}

bool _collisionCheck::isCubicCol(vec3, vec3)
{

}

void _collisionCheck::clampBounds(vec3& pos, float boundarySize){
    vec3 minBound = vec3(-boundarySize, -10.0f, -boundarySize);
    vec3 maxBound = vec3(boundarySize, 50.0f, boundarySize);
    if(pos.x < minBound.x) pos.x = minBound.x;
    if(pos.y < minBound.y) pos.y = minBound.y;
    if(pos.z < minBound.z) pos.z = minBound.z;

    if(pos.x > maxBound.x) pos.x = maxBound.x;
    if(pos.y > maxBound.y) pos.y = maxBound.y;
    if(pos.z > maxBound.z) pos.z = maxBound.z;
}
