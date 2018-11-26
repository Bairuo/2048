#include "Player.h"
#include "AI.h"
#include "Board.h"
#include "GameTime.h"
#include "../include/GLFW/glfw3.h"
#include "GameUtilities.h"

static const float playingPeriod = 0.3f;
static const float autoPeriod = 0.5f;

Player::Player()
{

}

Player::~Player()
{

}

void Player::StartPlaying()
{
    timer = -2.7f;
    pointer = 0;
    state = PlayingBack;
}

void Player::PausePlaying()
{
    state = PlayingPause;
}

void Player::ContinueToPlay()
{
    state = PlayingBack;
}

void Player::QuitPlaying(Board *board)
{
    state = Idle;

    board->ReproduceHistory(board->maxPointer);
}

void Player::StartAI(Board *board)
{
    timer = 0;
    state = Auto;

    ai.reset(new AI(board));
    ai->StartSearch(board);
}

void Player::UpdateAI(Board *board)
{
    ai->StartSearch(board);
}

void Player::ShutDownAI()
{
    state = Idle;

    ai.reset();
}

bool Player::AutoPlay(Board *board, bool &isForward)
{
    timer += GameTime::deltaTime;
    ai->Update();

    if(timer > autoPeriod)
    {
        timer = 0;
        ai->StopSearch();
        int operate = ai->GetOperate();

        switch(operate)
        {
        case 0:
            board->Up();
            break;
        case 1:
            board->Down();
            break;
        case 2:
            board->Left();
            break;
        case 3:
            board->Right();
            break;
        case -1:    // lose
            isForward = false;
            return true;
        case 4:     // win
            isForward = false;
            return true;
        default:
            break;
        }

        moveOperate = operate;

        isForward = true;
    }
    return false;
}

void Player::Playing(Board *board, int &get_pointer, bool &isForward)
{
    timer += GameTime::deltaTime;

    if(timer > playingPeriod)
    {
        timer = 0;
        board->ReproduceOperate(pointer - 1);

        isForward = true;
        get_pointer = pointer;

        if(pointer == board->maxPointer)
        {
            state = Idle;
        }
        else
        {
            pointer++;
        }
    }
}

void Player::HandleKeyOperate(Board *board, int key)
{
    /* Basic game operation */
    switch(key)
    {
    case GLFW_KEY_UP:
        if(gameManager.isRanking)gameManager.EndRanking();
        if(!gameManager.isGameWin && !gameManager.isGameLose)
        {
            if(board->Up())
            {
                state = Moved;
                moveOperate = 0;
            }

        }
        break;
    case GLFW_KEY_DOWN:
        if(gameManager.isRanking)gameManager.EndRanking();
        if(!gameManager.isGameWin && !gameManager.isGameLose)
        {
            if(board->Down())
            {
                state = Moved;
                moveOperate = 1;
            }
        }
        break;
    case GLFW_KEY_LEFT:
        if(gameManager.isRanking)gameManager.EndRanking();
        if(!gameManager.isGameWin && !gameManager.isGameLose)
        {
            if(board->Left())
            {
                state = Moved;
                moveOperate = 2;
            }
        }
        break;
    case GLFW_KEY_RIGHT:
        if(gameManager.isRanking)gameManager.EndRanking();
        if(!gameManager.isGameWin && !gameManager.isGameLose)
        {
            if(board->Right())
            {
                state = Moved;
                moveOperate = 3;
            }
        }
        break;
    case GLFW_KEY_Z:
        gameManager.CleanScreen();
        board->Undo();
        break;
    case GLFW_KEY_Y:
        if(gameManager.isRanking)gameManager.EndRanking();
        board->Repeat();
        break;
    default:
        break;
    }


    /* Extend game operation */
    switch(key)
    {
    case GLFW_KEY_S:
        gameManager.CleanScreen();
        gameManager.ShowRanking();
        break;
    case GLFW_KEY_H:
        gameManager.CleanScreen();
        board->MakeEmpty();
        board->score = 0;
        board->maxScore = 0;
        gameManager.StartPlayBackAnimation(board);
        StartPlaying();
        break;
    case GLFW_KEY_I:
        if(!gameManager.isGameWin && !gameManager.isGameLose)
        {
            gameManager.CleanScreen();
            StartAI(board);
        }

        break;
    default:
        break;
    }
}
