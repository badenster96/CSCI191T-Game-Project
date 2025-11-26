#include "_EnemyHandler.h"
#include <iostream>

_EnemyHandler::_EnemyHandler()
{
    //ctor
}

_EnemyHandler::~_EnemyHandler()
{
    //dtor
}
void _EnemyHandler::setup(int numEnemies) {
    std::cout << "EnemyHandler Setup" << std::endl;
    enemies.resize(numEnemies);
    for(int i = 0; i < enemies.size(); i++) {
        std::cout << "Enemy " << i << std::endl;
        enemies[i] = new _enemy();
    }
}
void _EnemyHandler::initModels(const char* model){
    std::cout << "EnemyHandler InitModels" << std::endl;
    for(int i = 0; i < enemies.size(); i++) enemies.at(i)->init(model);
}
void _EnemyHandler::draw(vec3& player) {
    glPushMatrix();
        for(auto& e : enemies) {
            float dx = e->pos.x - player.x;
            float dz = e->pos.z - player.z;
            float distSq = dx*dx + dz*dz;

            if(distSq > 300.0f * 300.0f) continue;
            if(e->isAlive)e->draw();
        }
    glPopMatrix();
}

_enemy* _EnemyHandler::nearest(vec3& point) {
    float minDistance = 100000.0f;
    _enemy* nearestEnemy = nullptr;
    for(auto& e : enemies) {
        if(e->isSpawned && e->isAlive){
            e->moveTowardPoint(point);
            vec3 distBetween;
            distBetween.x = point.x - e->pos.x;
            distBetween.y = point.y - e->pos.y;
            distBetween.z = point.z - e->pos.z;
            // float dist = xDist*xDist + yDist*yDist + zDist*zDist;
            float dist = distBetween.normalize();
            if(dist < minDistance) {
                    minDistance = dist;
                    nearestEnemy = e;
            }
        }
    }
    return nearestEnemy;
}

void _EnemyHandler::calc(int rangeEnemiesPerWave, int minEnemiesPerWave, vec3& point) {
    int enemiesPerWave = rand()%rangeEnemiesPerWave + minEnemiesPerWave;
    int spawned = 0;
    for(auto& e : enemies) {
        if(!e->isSpawned && spawned < enemiesPerWave){
            e->spawn(point);
            spawned++;
        }
    }
}

bool _EnemyHandler::canSpawn() {
    for(const auto& e : enemies){
        if(!e->isSpawned) return true;
    }
    return false;
}
