//
// Created by lrsoft on 2023/4/10.
//

#include "Timer.h"
#include <iostream>
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

bool Timer::IsPause = false;
bool Timer::afterPause = false;

Timer::Timer(bool willEffectByPause) {
    EffectByPause = willEffectByPause;
}

void Timer::Tick() {
    if (!IsPause || !EffectByPause) {
        if (afterPause && EffectByPause) {
            nowFrame = getSystemTime();
            lastTime = nowFrame;
            deltaFrame = 0.0f;
            afterPause = false;
            return;
        }
        if (!FirstRun) {//Not first run
            nowFrame = getSystemTime();
            deltaFrame = nowFrame - lastTime;
            lastTime += deltaFrame;
        } else {
            nowFrame = getSystemTime();
            deltaFrame = 0;
            lastTime = nowFrame;

            FirstRun = false;
        }

        accumulateTime += deltaFrame;
    }
}

void Timer::Tick(float update_nowFrame) {
    if (!IsPause || !EffectByPause) {
        if (afterPause && EffectByPause) {
            nowFrame = update_nowFrame;
            lastTime = update_nowFrame;
            deltaFrame = 0.0f;
            return;
        }
        if (!FirstRun) {//Not first run
            nowFrame = update_nowFrame;
            deltaFrame = nowFrame - lastTime;
            lastTime += deltaFrame;
        } else {
            nowFrame = update_nowFrame;
            deltaFrame = 0;
            lastTime = nowFrame;
            FirstRun = false;
        }

        accumulateTime += deltaFrame;
    }
}


void Timer::Clear() {
    nowFrame = 0.0f;
    deltaFrame = 0.0f;
    lastTime = 0.0f;
    accumulateTime = 0.0;
    FirstRun = true;
}

void Timer::Pause() {
    IsPause = true;
}

void Timer::Resume() {
    IsPause = false;
    afterPause = true;
}

void Timer::AfterResume() {
    afterPause = false;
}

long long Timer::getNowFrame() {
    return nowFrame;
}

long long Timer::getDeltaFrame() {
    return deltaFrame;
}

long long Timer::getLastFrame() {
    return lastTime;
}

long long Timer::getAccumlateTime() {
    return accumulateTime;
}

bool Timer::getIsPause() {
    return IsPause;
}

long long Timer::getSystemTime() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
