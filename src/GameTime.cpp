#include "GameTime.h"
#include <cstdio>
#include <iostream>

float GameTime::deltaTime = 0;
GameTime* GameTime::instance = nullptr;

GameTime* GameTime::GetInstance()
{
    if(instance == nullptr)
    {
        instance = new GameTime();
    }
    return instance;
}

void GameTime::Init()
{
    lastTime = clock();
}

void GameTime::Update()
{
    clock_t cur = clock();
    deltaTime = 1.0f * (cur - lastTime) / CLOCKS_PER_SEC;
    lastTime = cur;
}
