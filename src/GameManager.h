// GameManager.h
// Author: Bairuo

#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

#include <string>
#include <boost/shared_ptr.hpp>

class Board;
class Text;
class InputText;
class Rectangle;

class GameManager
{
public:
    bool isInput = false;
    bool isGameWin = false;
    bool isGameLose = false;
    bool isRanking = false;

    void Update();

    GameManager();
    ~GameManager();

    void CleanScreen();

    void PauseScreenAnimation();
    void ContinueScreenAnimation();

    void StartInput();
    void EndInput();
    std::string GetInput();
    void HandleInput(char ch);
    void DelteInput();

    void ShowRanking();
    void EndRanking();

    void StartPlayBackAnimation(Board *board);

    void StartGame(Board *board);
    void WinGame(Board *board);
    void LoseGame(Board *board);

    bool JudgeWin(Board *board);
    bool JudgeLose(Board *board);

private:
    boost::shared_ptr<Rectangle> curtain;
    boost::shared_ptr<InputText> inputField;
    boost::shared_ptr<Text> text;
    boost::shared_ptr<Text> ranks[10];

    // If want to be more efficient, need replacing 'release' with 'active false'
    void Release();
};

#endif // GAMEMANAGER_H_
