#include "_waveHandler.h"

_waveHandler::_waveHandler()
{
    //ctor
}

_waveHandler::~_waveHandler()
{
    //dtor
}
void _waveHandler::update() {
     enemiesPerWave = 8 * wave;
     enemiesPerCapsule = 4 *  wave;
     capsulesPerWave =  enemiesPerWave /  enemiesPerCapsule;
}

void _waveHandler::waveSpawn(int wave){
    int enemiesPerWave = 8 * wave;
    int enemiesPerCapsule = 4 * wave;
    int capsulesPerWave = enemiesPerWave / enemiesPerCapsule;
}
