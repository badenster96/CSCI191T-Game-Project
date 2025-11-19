#include "_enemy.h"

_enemy::_enemy()
{
    //ctor
}

_enemy::~_enemy()
{
    //dtor
}
_enemy::init(){

}
_
// Spawns the enemy at a random location around the player
void _enemy::spawn(vec3 center){
    float radius = rand()%500/100.0f + 3;
    float angle  = rand()%360 / 180.0f * PI;
    // Calculate the random point
    vec3 spawnPoint;
    spawnPoint.x = radius * cos(angle);
    spawnPoint.y = 0.0f;
    spawnPoint.z = radius * sin(angle);
    isSpawned = isAlive = true;

    // glTranslatef(spawnPoint.x, spawnPoint.y, spawnPoint.z);
}

void _enemy::moveTowardPoint(vec3 point) {
    // create a vector pointing at the player
    vec3 path;
    float speed = 0.1f;
    path.x = point.x - pos.x;
    path.y = 0; // ignore vertical rotation for 2D sprites
    path.z = point.z - pos.z;
    float angle = atan2f(path.x, path.z) * 180.0f / PI;
    // Normalize the vector, to control speed better
    float length = sqrtf(path.x*path.x + path.z*path.z);
    if(length > 0.01f){
        path.x /= length * speed;
        path.z /= length * speed;
    }
    // Now that the vector has been normalized, move the enemy
    glTranslatef(pos.x + path.x, pos.y, pos.z + path.z);
    glRotatef(angle,0,1,0);
}
