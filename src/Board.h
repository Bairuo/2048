// Board.h
// Author: Bairuo

#ifndef BOARD_H_
#define BOARD_H_

#include "Rectangle.h"
#include<list>

class Card;

class Board : public Rectangle
{
public:
    int score = 0;
    int maxScore = 0;   // the max score in the cards
    unsigned int maxPointer = 0;

    bool lastMerged;
    int lastGetScore;
    int lastRefreshScore;

    Card *cards[4][4];

    bool Up();
    bool Down();
    bool Left();
    bool Right();
    void RecordHistory(int freshPoint, int operate);
    void RecordHistory(int p1, int operate, int getScore, int p2 = -1);

    bool TryUp();
    bool TryDown();
    bool TryLeft();
    bool TryRight();

    void Start();
    void Undo();
    void Repeat();
    void PlayBack();
    void AutoPlay();

    void Update();

    void MakeEmpty();
    int Fill();
    int Refresh();
    int Refresh(float startTime, float refreshTime, float initialSize);

    void ReproduceOperate(int pointer);
    void ReproduceRefresh(int pointer);
    void ReproduceHistory(int pointer);

    //Board() = default;

    ~Board();

    void GetCardNumberCoord(int num, int &get_i, int &get_j);
    void GetCardCoord(int i, int j, float &x, float &y);

    Board(const glm::vec3 &pos, const Color &color, const Color &roundColor, float width, float height,
          const GLchar *vertexPath = standardVsPath, const GLchar *fragmentPath = standardFragPath);

private:
    int idle = 16;
    float realWidth;
    float realHeight;

    void MakeCard(int i, int j, int score);

    Rectangle *cardBackground[4][4];
    std::list<Card *> destroyList;

    unsigned int pointer = 0;

    static const GLchar standardVsPath[];
    static const GLchar standardFragPath[];
};

#endif // BOARD_H_
