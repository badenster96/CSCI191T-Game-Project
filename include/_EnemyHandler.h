#ifndef _ENEMYHANDLER_H
#define _ENEMYHANDLER_H


#include <_enemy.h>
#include <_collisioncheck.h>

class _EnemyHandler
{
    public:
        _EnemyHandler();
        virtual ~_EnemyHandler();
        _collisionCheck* myCol = new _collisionCheck;
        void setup(int numEnemies);
        void initModels(const char* model);
        void update(vec3& player, float deltaTime);
        void draw();
        _enemy* nearest(vec3& point);
        void resolveCollisions();
        void spawn(int enemiesPerWave, vec3& point);
        int numEnemies();

        bool canSpawn();
        float lastWaveTime = 0;
        int totalEnemiesSpawned = 0;
        int enemiesKilled = 0;
        std::vector<_enemy*> enemies;

    protected:


    private:
};

#endif // _ENEMYHANDLER_H
