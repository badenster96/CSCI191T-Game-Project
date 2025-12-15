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
void _EnemyHandler::update(vec3& player, float deltaTime){
    for(auto& e : enemies) {
        e->moveTowardPoint(player, deltaTime);
    }
    resolveCollisions();
}
void _EnemyHandler::draw() {
    for(auto& e : enemies) {
        if(e->isAlive)e->draw();
    }
}

_enemy* _EnemyHandler::nearest(vec3& point) {
    float minDistance = 100000.0f;
    _enemy* nearestEnemy = nullptr;
    for(auto& e : enemies) {
        if(e->isSpawned && e->isAlive){
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
 void _EnemyHandler::resolveCollisions() {
     float minimumDistance = 4.0f;
    for(int i = 0; i < enemies.size(); i++){
        _enemy* e1 = enemies[i];
        if(!e1 || !e1->isSpawned || !e1->isAlive) continue;
        for(int j = i+1; j < enemies.size(); j++){
            _enemy* e2 = enemies[j];
            if(!e2 || !e2->isSpawned || !e2->isAlive) continue;

            float dx = e1->pos.x - e2->pos.x;
            float dy = e1->pos.y - e2->pos.y;
            float dz = e1->pos.z - e2->pos.z;
            float distSq = dx*dx + dy*dy + dz*dz;

            if(distSq < minimumDistance * minimumDistance){
                float distance = sqrt(distSq);
                if(distance < 0.0001f) distance = 0.0001f;

                float push = (minimumDistance - distance) * 0.5f;

                float nx = dx / distance;
                float ny = dy / distance;
                float nz = dz / distance;

                e1->pos.x += nx * push;
                e1->pos.y += ny * push;
                e1->pos.z += nz * push;

                e2->pos.x -= nx * push;
                e2->pos.y -= ny * push;
                e2->pos.z -= nz * push;
            }
        }
    }
}

void _EnemyHandler::spawn(int enemiesPerWave, vec3& point) {
    int spawned = 0;
    for(auto& e : enemies) {
        if(!e->isSpawned && spawned < enemiesPerWave){
            e->spawn(point);
            spawned++;
        }
    }
    totalEnemiesSpawned += spawned;
}

bool _EnemyHandler::canSpawn() {
    for(const auto& e : enemies){
        if(!e->isSpawned) return true;
    }
    return false;
}
int _EnemyHandler::numEnemies() {
    int totalEnemies = 0;
    for(int i = 0; i < enemies.size(); i++){
        if(enemies.at(i)->isSpawned) totalEnemies++;
    }
    return totalEnemies;
}
