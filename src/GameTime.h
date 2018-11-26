// GameTime.h
// Author: Bairuo

#ifndef GAMETIME_H_
#define GAMETIME_H_

#include <time.h>

class GameTime
{
public:
    static float deltaTime;

    void Init();
    void Update();

    static GameTime* GetInstance();

private:
    static GameTime* instance;
    clock_t lastTime;

};

#endif // GAMETIME_H_
