#ifndef _WAVEHANDLER_H
#define _WAVEHANDLER_H


class _waveHandler
{
    public:
        _waveHandler();
        virtual ~_waveHandler();
        void update();
        void waveSpawn(int wave);


        int enemiesPerWave;
        int enemiesPerCapsule;
        int capsulesPerWave;
        bool waveReady = false;
        bool waveSpawning = false;
        bool waveEnd = false;
        int wave = 1;
        float timeBetweenWaves = 5.0f;
        float timeSinceLastWave = 0.0f;

    protected:

    private:
};

#endif // _WAVEHANDLER_H
