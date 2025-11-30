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
        void update(vec3& player);
        void draw(vec3& player);
        _enemy* nearest(vec3& point);
        void resolveCollisions();
        void calc(int rangeEnemiesPerWave, int minEnemiesPerWave, vec3& point);

        bool canSpawn();
        float lastWaveTime = 0;
        std::vector<_enemy*> enemies;

    protected:


    private:
};

#endif // _ENEMYHANDLER_H
