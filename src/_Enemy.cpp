#include "_enemy.h"

_enemy::_enemy()
{
    //ctor
    iFrames = 0.3f;
    lastTimeHit = 0.0f;


}

_enemy::~_enemy()
{
    //dtor
}
void _enemy::init(const char* filename){
    initModel(filename);
}

// Spawns the enemy at a random location around a point
void _enemy::spawn(vec3 center){
    // Enemy stats
    health = 10.0f + rand()%5;
    damage = 1.0f + rand()%3;
    speed = 0.2f + rand()%100 * 0.001f;
    // Spawn logic
    float radius = rand()%50/10.0f + 10;
    angle = rand()%360 / 180.0f * PI;
    // Calculate the random point
    pos.x = center.x + radius * cos(angle);
    pos.y = 0.0f;
    pos.z = center.z + radius * sin(angle);
    // std::cout << radius << " " << angle << " " << pos.x << pos.z << std::endl;
    isSpawned = isAlive = true;
}
void _enemy::draw() {
    glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(angle+90, 0, 1, 0);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glScalef(0.1,0.1,0.1);
        Actions();
        Draw();
    glPopMatrix();
}

void _enemy::moveTowardPoint(vec3 point) {
    vec3 path;
    path.x = point.x - pos.x;
    path.y = 0;
    path.z = point.z - pos.z;
    this-> angle = atan2f(path.x, path.z) * 180.0f / PI;
    // Normalize the vector, to control speed better
    float length = sqrtf(path.x*path.x + path.z*path.z);
    if(length > 0.01f){
        path.x /= length;
        path.z /= length;
        actionTrigger = RUN;
        pos.x += path.x * speed;
        pos.z += path.z * speed;

    }
}
