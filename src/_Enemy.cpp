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

// Spawns the enemy at a random location around the player
void _enemy::spawn(vec3 center){
    health = 10.0f + rand()%5;
    damage = 1.0f + rand()%3;
    float noSpawnCone = rand()%360 / 180.0f * PI;
    speed = 0.001f + rand()%50 * 0.001f;
    std::cout << speed << std::endl;
    float radius = rand()%500/10.0f + 50;
    do{
        angle = rand()%360 / 180.0f * PI;
    } while(abs(angle - noSpawnCone)*180.0f / PI < 0.0f);
    std::cout << "Spawned!" <<std::endl;

    // Calculate the random point
    pos.x = center.x + radius * cos(angle);
    pos.y = 0.0f;
    pos.z = center.z + radius * sin(angle);
    if(pos.x > 100)pos.x = 100;
    if(pos.z > 100)pos.z = 100;
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
    // create a vector pointing at the player
    vec3 path;
    path.x = point.x - pos.x;
    path.y = 0; // ignore vertical rotation for 2D sprites
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
