// Player.h
// Author: Bairuo

#ifndef PLAYER_H_
#define PLAYER_H_

#include <boost/shared_ptr.hpp>

class AI;
class Board;

enum PlayerState{Idle, Moved, PlayingBack, PlayingPause, Auto};

class Player
{
public:
    PlayerState state;
    int moveOperate;

    Player();
    ~Player();

    void StartPlaying();
    void Playing(Board *board, int &pointer, bool &isForward);   // Play back
    bool AutoPlay(Board *board, bool &isForward);   // return isOver
    void StartAI(Board *board);
    void UpdateAI(Board *board);
    void ShutDownAI();
    void PausePlaying();
    void ContinueToPlay();
    void QuitPlaying(Board *board);
    void HandleKeyOperate(Board *board, int key);

private:
    boost::shared_ptr<AI> ai;
    unsigned int pointer;
    float timer;
};


#endif // PLAYER_H_
