#include "GameManager.h"
#include "PlayerRecord.h"
#include "Board.h"
#include "Text.h"
#include "InputText.h"
#include "BoardRecord.h"
#include "bairuofunctions.h"
#include<algorithm>


static float winLoseDelayed = 1;
static const unsigned int maxNameLength = 8;
static const int winScore = 2048;

static const Color inputColor(0.004f, 0.341f, 0.655f, 0.5f);
static const Color noteColor(1, 1, 1, 1);

static const Color winColor(0.891f, 0.75f, 0.221f, 0.5f);
static const Color winpenColor(1, 1, 1, 1);
static const Color loseColor(0.98f, 0.973f, 0.937f, 0.5f);
static const Color losepenColor(0.464f, 0.43f, 0.395f, 1.0f);

void GameManager::StartPlayBackAnimation(Board *board)
{
    curtain.reset(new Rectangle(board->posture().transVec3, loseColor, Color(0, 0, 0, 0), board->radius, board->posture().getScaleX(), board->posture().getScaleY()));

    curtain->animation.reset(new Animation());
    curtain->animation->addFrame(0, curtain->posture, Color(loseColor.r, loseColor.g, loseColor.b, 0.6f));
    curtain->animation->addFrame(1.0f, curtain->posture, Color(loseColor.r, loseColor.g, loseColor.b, 0.8f));
    curtain->animation->addFrame(3.0f, curtain->posture, Color(loseColor.r, loseColor.g, loseColor.b, 0));
    curtain->animation->Start();

    text.reset(new Text("History play back", board->posture().transVec3, losepenColor, 51));

    text->animation.reset(new Animation());
    text->animation->addFrame(0, text->posture, Color(losepenColor.r, losepenColor.g, losepenColor.b, 0));
    text->animation->addFrame(1.0f, text->posture, Color(losepenColor.r, losepenColor.g, losepenColor.b, 1));
    text->animation->addFrame(2.8f, text->posture, Color(losepenColor.r, losepenColor.g, losepenColor.b, 0.2f));
    text->animation->addFrame(2.85f, text->posture, Color(losepenColor.r, losepenColor.g, losepenColor.b, 0));
    text->animation->Start();

    winLoseDelayed = 0;
}

void GameManager::ShowRanking()
{
    isRanking = true;
    curtain.reset(new Rectangle(glm::vec3(0, 0, 0), inputColor, Color(), 0, 2, 2));

    curtain->animation.reset(new Animation());
    curtain->animation->addFrame(0, curtain->posture, Color(inputColor.r, inputColor.g, inputColor.b, 0));
    curtain->animation->addFrame(1, curtain->posture, Color(inputColor.r, inputColor.g, inputColor.b, 0.5f));
    curtain->animation->Start();

    text.reset(new Text("Score Ranking", glm::vec3(0, 0.8f, 0), noteColor, 51));

    text->animation.reset(new Animation());
    text->animation->addFrame(0, text->posture, Color(noteColor.r, noteColor.g, noteColor.b, 0));
    text->animation->addFrame(1, text->posture, Color(noteColor.r, noteColor.g, noteColor.b, 1));
    text->animation->Start();

    auto RankMethod = [](const PlayerRecord &a, const PlayerRecord &b) -> bool {return a.getBestScore() > b.getBestScore();};
    std::sort(PlayerRecord::playerRecords.begin(), PlayerRecord::playerRecords.end(), RankMethod);

    float y, t = 0;
    for(unsigned int i = 0; i < PlayerRecord::playerRecords.size(); i++, t += 0.2f)
    {
        y = 0.55f - i * 0.14f;
        ranks[i].reset(new Text(PlayerRecord::playerRecords[i].getName() + "    " + bairuo::ToString(PlayerRecord::playerRecords[i].getBestScore()),
                            glm::vec3(0, y, 0), noteColor, 32));

        ranks[i]->animation.reset(new Animation());
        ranks[i]->animation->addFrame(t, Posture(1.2f, y, ranks[i]->posture.getPosZ(), 1), noteColor);
        ranks[i]->animation->addFrame(t + 1, Posture(0, y, ranks[i]->posture.getPosZ(), 1), noteColor);
        ranks[i]->animation->Start();
    }
}

void GameManager::EndRanking()
{
    if(!isRanking)return;

    CleanScreen();
}

void GameManager::StartInput()
{
    isInput = true;
    curtain.reset(new Rectangle(glm::vec3(0, 0, 0), inputColor, Color(), 0, 2, 2));

    curtain->animation.reset(new Animation());
    curtain->animation->addFrame(0, curtain->posture, Color(inputColor.r, inputColor.g, inputColor.b, 0));
    curtain->animation->addFrame(1, curtain->posture, Color(inputColor.r, inputColor.g, inputColor.b, 0.5f));
    curtain->animation->Start();

    text.reset(new Text("Please enter your name", glm::vec3(0, 0.2f, 0), noteColor, 51));

    text->animation.reset(new Animation());
    text->animation->addFrame(0, text->posture, Color(noteColor.r, noteColor.g, noteColor.b, 0));
    text->animation->addFrame(1, text->posture, Color(noteColor.r, noteColor.g, noteColor.b, 1));
    text->animation->Start();

    inputField.reset(new InputText("", glm::vec3(0, 0, 0), noteColor, 38));

    inputField->generateCurosAnimation(noteColor, 1);
}

std::string GameManager::GetInput()
{
    return inputField->text;
}

void GameManager::DelteInput()
{
    std::string curInput = inputField->text;
    if(curInput.size() > 0)
    {
        curInput = curInput.substr(0, curInput.size() - 1);
        inputField->text = curInput;
    }
}

void GameManager::HandleInput(char ch)
{
    std::string curInput = inputField->text;
    if(curInput.size() < maxNameLength)
    {
        inputField->text = curInput + ch;
    }
}

void GameManager::EndInput()
{
    isInput = false;

    CleanScreen();
}

void GameManager::StartGame(Board *board)
{
    isGameWin = false;
    isGameLose = false;
    isInput = false;
    isRanking = false;

    CleanScreen();

    winLoseDelayed = 1;
    board->Start();
}

void GameManager::WinGame(Board *board)
{
    isGameWin = true;
    curtain.reset(new Rectangle(board->posture().transVec3, winColor, Color(0, 0, 0, 0), board->radius, board->posture().getScaleX(), board->posture().getScaleY()));

    curtain->animation.reset(new Animation());
    curtain->animation->setStartTime(winLoseDelayed);
    curtain->animation->addFrame(0, curtain->posture, Color(winColor.r, winColor.g, winColor.b, 0));
    curtain->animation->addFrame(0.5f, curtain->posture, Color(winColor.r, winColor.g, winColor.b, 0.5f));
    curtain->animation->Start();

    text.reset(new Text("You win!", board->posture().transVec3, winpenColor, 68));

    text->animation.reset(new Animation());
    text->animation->setStartTime(winLoseDelayed);
    text->animation->addFrame(0, text->posture, Color(winpenColor.r, winpenColor.g, winpenColor.b, 0));
    text->animation->addFrame(0.5f, text->posture, Color(winpenColor.r, winpenColor.g, winpenColor.b, 1));
    text->animation->Start();

    winLoseDelayed = 0;
}

void GameManager::LoseGame(Board *board)
{
    isGameLose = true;
    curtain.reset(new Rectangle(board->posture().transVec3, loseColor, Color(0, 0, 0, 0), board->radius, board->posture().getScaleX(), board->posture().getScaleY()));

    curtain->animation.reset(new Animation());
    curtain->animation->setStartTime(winLoseDelayed);
    curtain->animation->addFrame(0, curtain->posture, Color(loseColor.r, loseColor.g, loseColor.b, 0));
    curtain->animation->addFrame(0.5f, curtain->posture, Color(loseColor.r, loseColor.g, loseColor.b, 0.5f));
    curtain->animation->Start();

    text.reset(new Text("Game Over", board->posture().transVec3, losepenColor, 68));

    text->animation.reset(new Animation());
    text->animation->setStartTime(winLoseDelayed);
    text->animation->addFrame(0, text->posture, Color(losepenColor.r, losepenColor.g, losepenColor.b, 0));
    text->animation->addFrame(0.5f, text->posture, Color(losepenColor.r, losepenColor.g, losepenColor.b, 1));
    text->animation->Start();

    winLoseDelayed = 0;
}

bool GameManager::JudgeWin(Board *board)
{
    if(board->maxScore >= winScore)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool GameManager::JudgeLose(Board *board)
{
    if(board->maxScore > 0 && !board->TryUp() && !board->TryDown() && !board->TryLeft() && !board->TryRight())
    {
        return true;
    }
    else
    {
        return false;
    }
}

GameManager::GameManager()
{

}

GameManager::~GameManager()
{
    Release();
}

void GameManager::Release()
{
    curtain.reset();
    text.reset();
    inputField.reset();

    for(int i = 0; i < 10; i++)
    {
        ranks[i].reset();
    }
}

void GameManager::CleanScreen()
{
    isRanking = false;
    isGameWin = false;
    isGameLose = false;

    Release();
}

void GameManager::PauseScreenAnimation()
{
    if(curtain.get() != nullptr && curtain->animation.get() != nullptr)
    {
        curtain->animation->setPause(true);
    }

    if(text.get() != nullptr && text->animation.get() != nullptr)
    {
        text->animation->setPause(true);
    }

    if(inputField.get() != nullptr && inputField->cursor->animation.get() != nullptr)
    {
        inputField->cursor->animation->setPause(true);
    }

    for(int i = 0; i < 10; i++)
    {
        if(ranks[i].get() != nullptr && ranks[i]->animation.get() != nullptr)
        {
            ranks[i]->animation->setPause(true);
        }
    }
}

void GameManager::ContinueScreenAnimation()
{
    if(curtain.get() != nullptr && curtain->animation.get() != nullptr)
    {
        curtain->animation->setPause(false);
    }

    if(text.get() != nullptr && text->animation.get() != nullptr)
    {
        text->animation->setPause(false);
    }

    if(inputField.get() != nullptr && inputField->cursor->animation.get() != nullptr)
    {
        inputField->cursor->animation->setPause(false);
    }

    for(int i = 0; i < 10; i++)
    {
        if(ranks[i].get() != nullptr && ranks[i]->animation.get() != nullptr)
        {
            ranks[i]->animation->setPause(false);
        }
    }
}

void GameManager::Update()
{
    if(curtain.get() != nullptr)
    {
        curtain->Update();
    }

    if(text.get() != nullptr)
    {
        text->Update();
    }

    if(inputField.get() != nullptr)
    {
        inputField->Update();
    }

    for(int i = 0; i < 10; i++)
    {
        if(ranks[i].get() != nullptr)
        {
            ranks[i]->Update();
        }
    }
}
