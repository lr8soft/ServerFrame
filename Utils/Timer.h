//
// Created by lrsoft on 2023/4/10.
//

#ifndef SERVERFRAME_TIMER_H
#define SERVERFRAME_TIMER_H


class Timer {
private:
    long long accumulateTime = 0;
    long long nowFrame = 0, deltaFrame = 0, lastTime = 0;

    bool FirstRun = true, EffectByPause = true;

    static bool IsPause, afterPause;

    long long getSystemTime();
public:
    Timer(bool willEffectByPause = true);

    void Tick();
    void Tick(float nowFrame);
    void Clear();

    static void Pause();
    static void Resume();
    static void AfterResume();
    static bool getIsPause();

    long long getNowFrame();
    long long getDeltaFrame();
    long long getLastFrame();

    long long getAccumlateTime();


};


#endif //SERVERFRAME_TIMER_H
