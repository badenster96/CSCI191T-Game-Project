#include "_timer.h"

_timer::_timer()
{
    //ctor

    startTime = clock();
    lastTime = startTime;
}

_timer::~_timer()
{
    //dtor
}
clock_t _timer::getTicks()
{
    auto currentTime = clock() - startTime;
    if(currentTime < 0.1f) currentTime = 0.1f;
    return currentTime;
}
float _timer::getTickSeconds(){
    if(paused) return 0.0f;
    clock_t now = clock();
    deltaTime = static_cast<float>(now - lastTime) / CLOCKS_PER_SEC;
    lastTime = now;
    if(deltaTime > 0.1f) deltaTime = 0.1f;
    return deltaTime;
}
float _timer::getTotalSeconds() {
    return static_cast<float>(clock() - startTime) / CLOCKS_PER_SEC;
}
void _timer::reset()
{
    startTime = clock();
}

void _timer::pauseTime()
{

}

void _timer::resumeTime()
{

}
