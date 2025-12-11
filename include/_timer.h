#ifndef _TIMER_H
#define _TIMER_H

#include<_common.h>

class _timer
{
    public:
        _timer();
        virtual ~_timer();

        clock_t startTime;     // to get current time

        clock_t getTicks();    // timer count
        float getTickSeconds();
        void reset();          // reset start time

        void pauseTime();      // For you to implement
        void resumeTime();

        float currentTime = 0.0f;
        clock_t lastTime = 0.0f;
        float deltaTime = 0.0f;
        bool paused = false;

    protected:

    private:
};

#endif // _TIMER_H
