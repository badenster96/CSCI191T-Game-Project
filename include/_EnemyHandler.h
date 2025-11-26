#ifndef _ENEMYHANDLER_H
#define _ENEMYHANDLER_H


#include <_enemy.h>

class _EnemyHandler
{
    public:
        _EnemyHandler();
        virtual ~_EnemyHandler();
        void setup(int numEnemies);
        void initModels(const char* model);
        void draw(vec3& player);
        _enemy* nearest(vec3& point);
        void calc(int rangeEnemiesPerWave, int minEnemiesPerWave, vec3& point);

        bool canSpawn();
        float lastWaveTime = 0;
        std::vector<_enemy*> enemies;

    protected:


    private:
};

#endif // _ENEMYHANDLER_H
