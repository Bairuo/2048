// Animation.h
// Author: Bairuo

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include<vector>
#include<time.h>
#include "Frame.h"
#include "Color.h"

class Animation
{
public:
    Color getColor() const;
    Posture getPosture() const;
    Color getEndColor() const;
    Posture getEndPosture() const;

    void Start();
    void Update();

    void addFrame(float time, const Posture &posture, const Color &color);
    void moveAllFramesTo(const Posture &posture);

    bool getActive() const;
    void setActive(bool active);
    bool getPause() const;
    void setPause(bool pause);
    bool getLoop() const;
    void setLoop(bool loop);

    void setStartTime(float t);
    float getTimer() const;
    void setTimer(float t);

private:
    bool active = false;
    bool pause = false;
    bool loop = false;

    float timer;
    float startTime = 0;

    unsigned int pointer;
    std::vector<Frame> frames;
};


#endif // ANIMATION_H_
