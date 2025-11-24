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
void _EnemyHandler::draw() {
    glPushMatrix();
        for(auto& e : enemies) if(e->isAlive)e->draw();
    glPopMatrix();
}

_enemy* _EnemyHandler::nearest(vec3& point) {
    float minDistance = 100000.0f;
    _enemy* nearestEnemy;
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

void _EnemyHandler::calc(int rangeEnemiesPerWave, int minEnemiesPerWave, int waveInterval, vec3& point) {
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    int enemiesPerWave = rand()%rangeEnemiesPerWave + minEnemiesPerWave;
    if(currentTime - lastWaveTime >= waveInterval){
        int spawned = 0;
        for(auto& e : enemies) {
            if(!e->isSpawned && spawned < enemiesPerWave){
                e->spawn(point);
                spawned++;
            }
        }
        lastWaveTime = currentTime;
    }
}
