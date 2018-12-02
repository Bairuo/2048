#include "Board.h"
#include "Card.h"
#include "BoardRecord.h"
#include "ConfigUtilities.h"
#include<stdlib.h>
#include<ctime>

static const int cardNum = 4;
static const Color defaultColor = {0.804f, 0.757f, 0.706f, 1};
static const float boardRadius = 0.02f;
static const float moveTime = 0.1f;
static std::vector<BoardRecord> history = {};

#if defined(_WIN32)
const GLchar Board::standardVsPath[] = ".\\res\\standard.vs";
const GLchar Board::standardFragPath[] = ".\\res\\rounded.frag";
#else
const GLchar Board::standardVsPath[] = "./res/standard.vs";
const GLchar Board::standardFragPath[] = "./res/rounded.frag";
#endif

Board::Board(const glm::vec3 &pos, const Color &color, const Color &roundColor, float width, float height,
             const GLchar *vertexPath, const GLchar *fragmentPath
             ):Rectangle(pos, color, roundColor, boardRadius, width + width / cardNum * 0.12f, height + height / cardNum * 0.12f, vertexPath, fragmentPath)
{
    this->realWidth = width;
    this->realHeight = height;

    float cardWidth = width / cardNum * 0.88f;
    float cardHeight = height / cardNum * 0.88f;
    float x0 = pos.x - width / 2;
    float y0 = pos.y + height / 2;
    float halfWidth = width / cardNum / 2;
    float halfHeight = height / cardNum / 2;
    float x, y;

    for(int i = 0; i < cardNum; i++)
        for(int j = 0; j < cardNum; j++)
        {
            x = x0 + (2 * i + 1) * halfWidth;
            y = y0 - (2 * j + 1) * halfHeight;
            cardBackground[i][j] = new Rectangle(glm::vec3(x, y, 0), defaultColor, Color(0, 0, 0, 0), c_cardRadius, cardWidth, cardHeight);
            cards[i][j] = nullptr;
        }
    srand(time(0));
}

Board::~Board()
{
    for(int i = 0; i < cardNum; i++)
        for(int j = 0; j < cardNum; j++)
        {
            if(cards[i][j] != nullptr)
                delete cards[i][j];
            delete cardBackground[i][j];
        }

    for(std::list<Card *>::iterator i = destroyList.begin(); i != destroyList.end(); i++)
        delete *i;
}

void Board::GetCardNumberCoord(int num, int &get_i, int &get_j)
{
    get_i = num / 4;
    get_j = num % 4;
}

void Board::GetCardCoord(int i, int j, float &x, float &y)
{
    float x0 = posture().getPosX() - realWidth / 2;
    float y0 = posture().getPosY() + realHeight / 2;
    float halfWidth = realWidth / cardNum / 2;
    float halfHeight = realHeight / cardNum / 2;

    x = x0 + (2 * i + 1) * halfWidth;
    y = y0 - (2 * j + 1) * halfHeight;
}

void Board::MakeCard(int i, int j, int score)
{
    float cardWidth = realWidth / cardNum * 0.88f;
    float cardHeight = realHeight / cardNum * 0.88f;
    float x, y;

    GetCardCoord(i, j, x, y);

    cards[i][j] = new Card(score, glm::vec3(x, y, 0), Color(0, 0, 0, 0), cardWidth, cardHeight);

    if(score > maxScore)
    {
        maxScore = score;
    }

    idle--;
}

int Board::Refresh(float startTime, float refreshTime, float initialSize)
{
    if(idle <= 0)return -1;

    int id = 0;
    int num = rand() % idle;
    int score = rand() % 2;


    int i = 0, j = 0;
    bool isFind = false;

    int k = 0;
    for(i = 0; i < cardNum; i++)
    {
        for(j = 0; j < cardNum; j++)
        {
            if(cards[i][j] == nullptr)
            {
                if(k < num)
                    k++;
                else
                {
                    isFind = true;
                    break;
                }
            }
            id++;
        }

        if(isFind)
            break;
    }

    MakeCard(i, j, score * 2 + 2);
    lastRefreshScore = score << 1;

    cards[i][j]->Refresh(startTime, refreshTime, initialSize);

    return id;
}

int Board::Refresh()
{
    return Refresh(0, c_refreshTime, 0.5f);
}

int Board::Fill()
{
    if(lastMerged)
    {
        return Refresh(moveTime + c_mergeBornTime * 0.5f, c_refreshTime, 0.75f);
    }
    else
    {
        return Refresh(moveTime, c_refreshTime, 0.75f);
    }
}

void Board::RecordHistory(int freshPoint, int operate)
{
    RecordHistory(freshPoint, operate, lastGetScore);
}

void Board::RecordHistory(int p1, int operate, int getScore, int p2)
{
    BoardRecord record;

    pointer++;
    maxPointer = pointer;

    record.freshPos1 = p1;
    record.freshPos2 = p2;

    int n = 0;
    for(int i = 0; i < cardNum; i++)
        for(int j = 0; j < cardNum; j++)
    {
        if(cards[i][j] != nullptr)
        {
            record.boardMap[i][j] = cards[i][j]->score;     // fresh card also record
        }
        else
        {
            record.boardMap[i][j] = -1;
        }
        n++;
    }

    if(pointer > 0)
    {
        history[pointer - 1].operate = operate;
        history[pointer - 1].getScore = getScore;
    }

    if(pointer == history.size())
    {
        history.push_back(record);
    }
    else
    {
        history[pointer] = record;
    }
}

void Board::ReproduceOperate(int pointer)
{
    if(pointer < 0)
    {
        MakeEmpty();
        return;
    }

    int operate = history[pointer].operate;

    switch(operate)
    {
    case 0:
        Up();
        break;
    case 1:
        Down();
        break;
    case 2:
        Left();
        break;
    case 3:
        Right();
        break;
    default:
        break;
    }
}

void Board::ReproduceRefresh(int pointer)
{
    BoardRecord record = history[pointer];
    int i, j;

    if(pointer == 0)
    {
        int p1 = record.freshPos1;
        int p2 = record.freshPos2;

        GetCardNumberCoord(p1, i, j);
        MakeCard(i, j, record.boardMap[i][j]);
        cards[i][j]->Refresh(0, c_refreshTime, 0.5f);

        GetCardNumberCoord(p2, i, j);
        MakeCard(i, j, record.boardMap[i][j]);
        cards[i][j]->Refresh(0, c_refreshTime, 0.5f);
    }
    else if(pointer > 0)
    {
        int p = record.freshPos1;
        float startTime;

        //std::cout << pointer << ", " << maxPointer << std::endl;

        GetCardNumberCoord(p, i, j);
        MakeCard(i, j, record.boardMap[i][j]);
        if(history[pointer - 1].getScore > 0)
        {
            startTime = moveTime + c_mergeBornTime * 0.5f;
        }
        else
        {
            startTime = moveTime;
        }
        cards[i][j]->Refresh(startTime, c_refreshTime, 0.75f);
    }
}

void Board::ReproduceHistory(int pointer)
{
    int n = 0;
    BoardRecord record = history[pointer];

    maxScore = 0;

    MakeEmpty();

    for(int i = 0; i < cardNum; i++)
        for(int j = 0; j < cardNum; j++)
    {
        if(record.boardMap[i][j] > 0)
        {
            if(n != record.freshPos1 && n != record.freshPos2)
            {
                MakeCard(i, j, record.boardMap[i][j]);
            }
            else
            {
                if(record.freshPos2 == -1)    // Is not the start
                {
                    MakeCard(i, j, record.boardMap[i][j]);
                    cards[i][j]->Refresh(0, c_refreshTime, 0.75f);
                }
                else
                {
                    MakeCard(i, j, record.boardMap[i][j]);
                    cards[i][j]->Refresh(0, c_refreshTime, 0.5f);
                }
            }
        }
        n++;
    }
}

void Board::Undo()
{
    if(pointer > 0)
    {
        pointer--;
        score -= history[pointer].getScore;
        ReproduceHistory(pointer);
    }
}

void Board::Repeat()
{
    if(pointer < maxPointer)
    {
        score += history[pointer].getScore;
        pointer++;
        ReproduceHistory(pointer);
    }
}

void Board::MakeEmpty()
{
    idle = cardNum * cardNum;
    score = 0;
    maxScore = 0;

    for(int i = 0; i < cardNum; i++)
        for(int j = 0; j < cardNum; j++)
        {
            if(cards[i][j] != nullptr)
            {
                delete cards[i][j];
                cards[i][j] = nullptr;
            }
        }
}

void Board::Start()
{
    MakeEmpty();

    pointer = -1;
    maxPointer = 0;

    int p1 = Refresh();
    int p2 = Refresh();

    RecordHistory(p1, 0, 0, p2);
}

bool Board::Up()
{
    int beforeCards, lastCard, score = 0;
    int mergeScore;
    float x, y;
    bool beforeCanMerge;
    bool isMove = false;
    bool isMerge = false;

    for(int i = 0; i < cardNum; i++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int j = 0; j < cardNum; j++)
        {
            if(cards[i][j] != nullptr)
            {
                if(lastCard >= 0 && beforeCanMerge == true && cards[i][lastCard]->score == cards[i][j]->score)
                                                // move and merge
                {
                    mergeScore = cards[i][j]->score;
                    score += cards[i][j]->score * 2;

                    GetCardCoord(i, beforeCards - 1, x, y);
                    cards[i][j]->MoveTo(x, y, moveTime);

                    cards[i][j]->Death(moveTime);
                    cards[i][beforeCards - 1]->Death(moveTime);
                    destroyList.push_back(cards[i][j]);
                    destroyList.push_back(cards[i][beforeCards - 1]);
                    cards[i][j] = nullptr;
                    cards[i][beforeCards - 1] = nullptr;
                    idle += 2;

                    MakeCard(i, beforeCards - 1, mergeScore * 2);
                    cards[i][beforeCards - 1]->MergeBorn(moveTime);


                    isMove = true;
                    isMerge = true;

                    beforeCanMerge = false;
                }
                else
                {
                    if(j - beforeCards > 0)     // no merge but move
                    {
                        GetCardCoord(i, beforeCards, x, y);
                        cards[i][j]->MoveTo(x, y, moveTime);

                        cards[i][beforeCards] = cards[i][j];
                        cards[i][j] = nullptr;

                        lastCard = beforeCards;

                        isMove = true;
                    }
                    else                        // no merge, no move
                    {
                        lastCard = j;
                    }

                    beforeCards++;
                    beforeCanMerge = true;      // if this card haven't been merged this turn, it must can be merged next
                }
            }
        }
    }

    if(isMove)
    {
        lastMerged = isMerge;
        lastGetScore = score;
        this->score += score;
    }

    return isMove;
}

bool Board::Down()
{
    int beforeCards, lastCard, score = 0;
    int mergeScore;
    float x, y;
    bool beforeCanMerge;
    bool isMove = false;
    bool isMerge = false;

    for(int i = 0; i < cardNum; i++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int j = cardNum - 1; j >= 0; j--)
        {
            if(cards[i][j] != nullptr)
            {
                if(lastCard >= 0 && beforeCanMerge == true && cards[i][lastCard]->score == cards[i][j]->score)
                                                // move and merge
                {
                    mergeScore = cards[i][j]->score;
                    score += cards[i][j]->score * 2;

                    GetCardCoord(i, cardNum - beforeCards, x, y);
                    cards[i][j]->MoveTo(x, y, moveTime);

                    cards[i][j]->Death(moveTime);
                    cards[i][cardNum - beforeCards]->Death(moveTime);
                    destroyList.push_back(cards[i][j]);
                    destroyList.push_back(cards[i][cardNum - beforeCards]);
                    cards[i][j] = nullptr;
                    cards[i][cardNum - beforeCards] = nullptr;
                    idle += 2;

                    MakeCard(i, cardNum - beforeCards, mergeScore * 2);
                    cards[i][cardNum - beforeCards]->MergeBorn(moveTime);

                    isMove = true;
                    isMerge = true;

                    beforeCanMerge = false;
                }
                else
                {
                    if(j + 1 + beforeCards < cardNum)     // no merge but move
                    {
                        GetCardCoord(i, cardNum - 1 - beforeCards, x, y);
                        cards[i][j]->MoveTo(x, y, moveTime);

                        cards[i][cardNum - 1 - beforeCards] = cards[i][j];
                        cards[i][j] = nullptr;

                        lastCard = cardNum - 1 - beforeCards;

                        isMove = true;
                    }
                    else                        // no merge, no move
                    {
                        lastCard = j;
                    }

                    beforeCards++;
                    beforeCanMerge = true;      // if this card haven't been merged this turn, it must can be merged next
                }
            }
        }
    }

    if(isMove)
    {
        lastMerged = isMerge;
        lastGetScore = score;
        this->score += score;
    }


    return isMove;
}

bool Board::Left()
{
    int beforeCards, lastCard, score = 0;
    int mergeScore;
    float x, y;
    bool beforeCanMerge;
    bool isMove = false;
    bool isMerge = false;

    for(int j = 0; j < cardNum; j++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int i = 0; i < cardNum; i++)
        {
            if(cards[i][j] != nullptr)
            {
                if(lastCard >= 0 && beforeCanMerge == true && cards[lastCard][j]->score == cards[i][j]->score)
                                                // move and merge
                {
                    mergeScore = cards[i][j]->score;
                    score += cards[i][j]->score * 2;

                    GetCardCoord(beforeCards - 1, j, x, y);
                    cards[i][j]->MoveTo(x, y, moveTime);

                    cards[i][j]->Death(moveTime);
                    cards[beforeCards - 1][j]->Death(moveTime);
                    destroyList.push_back(cards[i][j]);
                    destroyList.push_back(cards[beforeCards - 1][j]);
                    cards[i][j] = nullptr;
                    cards[beforeCards - 1][j] = nullptr;
                    idle += 2;

                    MakeCard(beforeCards - 1, j, mergeScore * 2);
                    cards[beforeCards - 1][j]->MergeBorn(moveTime);

                    isMove = true;
                    isMerge = true;

                    beforeCanMerge = false;
                }
                else
                {
                    if(i - beforeCards > 0)     // no merge but move
                    {
                        GetCardCoord(beforeCards, j, x, y);
                        cards[i][j]->MoveTo(x, y, moveTime);

                        cards[beforeCards][j] = cards[i][j];
                        cards[i][j] = nullptr;

                        lastCard = beforeCards;

                        isMove = true;
                    }
                    else                        // no merge, no move
                    {
                        lastCard = i;
                    }

                    beforeCards++;
                    beforeCanMerge = true;      // if this card haven't been merged this turn, it must can be merged next
                }
            }
        }
    }

    if(isMove)
    {
        lastMerged = isMerge;
        lastGetScore = score;
        this->score += score;
    }

    return isMove;
}

bool Board::Right()
{
    int beforeCards, lastCard, score = 0;
    int mergeScore;
    float x, y;
    bool beforeCanMerge;
    bool isMove = false;
    bool isMerge = false;

    for(int j = 0; j < cardNum; j++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int i = cardNum - 1; i >= 0; i--)
        {
            if(cards[i][j] != nullptr)
            {
                if(lastCard >= 0 && beforeCanMerge == true && cards[lastCard][j]->score == cards[i][j]->score)
                                                // move and merge
                {
                    mergeScore = cards[i][j]->score;
                    score += cards[i][j]->score * 2;

                    GetCardCoord(cardNum - beforeCards, j, x, y);
                    cards[i][j]->MoveTo(x, y, moveTime);

                    cards[i][j]->Death(moveTime);
                    cards[cardNum - beforeCards][j]->Death(moveTime);
                    destroyList.push_back(cards[i][j]);
                    destroyList.push_back(cards[cardNum - beforeCards][j]);
                    cards[i][j] = nullptr;
                    cards[cardNum - beforeCards][j] = nullptr;
                    idle += 2;

                    MakeCard(cardNum - beforeCards, j, mergeScore * 2);
                    cards[cardNum - beforeCards][j]->MergeBorn(moveTime);

                    isMove = true;
                    isMerge = true;

                    beforeCanMerge = false;
                }
                else
                {
                    if(i + beforeCards + 1 < cardNum)     // no merge but move
                    {
                        GetCardCoord(cardNum - 1 - beforeCards, j, x, y);
                        cards[i][j]->MoveTo(x, y, moveTime);

                        cards[cardNum - 1 - beforeCards][j] = cards[i][j];
                        cards[i][j] = nullptr;

                        lastCard = cardNum - 1 - beforeCards;

                        isMove = true;
                    }
                    else                        // no merge, no move
                    {
                        lastCard = i;
                    }

                    beforeCards++;
                    beforeCanMerge = true;      // if this card haven't been merged this turn, it must can be merged next
                }
            }
        }
    }

    if(isMove)
    {
        lastMerged = isMerge;
        lastGetScore = score;
        this->score += score;
    }

    return isMove;
}

bool Board::TryUp()
{
    int beforeCards, lastCard;

    for(int i = 0; i < cardNum; i++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int j = 0; j < cardNum; j++)
        {
            if(cards[i][j] != nullptr)
            {
                if(lastCard >= 0 && cards[i][lastCard]->score == cards[i][j]->score)
                                                // move and merge
                {
                    return true;
                }
                else
                {
                    if(j - beforeCards > 0)     // no merge but move
                    {
                        return true;
                    }
                    else                        // no merge, no move
                    {
                        lastCard = j;
                    }

                    beforeCards++;
                }
            }
        }
    }

    return false;
}

bool Board::TryDown()
{
    int beforeCards, lastCard;

    for(int i = 0; i < cardNum; i++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int j = cardNum - 1; j >= 0; j--)
        {
            if(cards[i][j] != nullptr)
            {
                if(lastCard >= 0 && cards[i][lastCard]->score == cards[i][j]->score)
                                                // move and merge
                {
                    return true;
                }
                else
                {
                    if(j + 1 + beforeCards < cardNum)     // no merge but move
                    {
                        return true;
                    }
                    else                        // no merge, no move
                    {
                        lastCard = j;
                    }

                    beforeCards++;
                }
            }
        }
    }
    return false;
}

bool Board::TryLeft()
{
    int beforeCards, lastCard;

    for(int j = 0; j < cardNum; j++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int i = 0; i < cardNum; i++)
        {
            if(cards[i][j] != nullptr)
            {
                if(lastCard >= 0 && cards[lastCard][j]->score == cards[i][j]->score)
                                                // move and merge
                {
                    return true;
                }
                else
                {
                    if(i - beforeCards > 0)     // no merge but move
                    {
                        return true;
                    }
                    else                        // no merge, no move
                    {
                        lastCard = i;
                    }

                    beforeCards++;
                }
            }
        }
    }

    return false;
}

bool Board::TryRight()
{
    int beforeCards, lastCard;

    for(int j = 0; j < cardNum; j++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int i = cardNum - 1; i >= 0; i--)
        {
            if(cards[i][j] != nullptr)
            {
                if(lastCard >= 0 && cards[lastCard][j]->score == cards[i][j]->score)
                                                // move and merge
                {
                    return true;
                }
                else
                {
                    if(i + beforeCards + 1 < cardNum)     // no merge but move
                    {
                        return true;
                    }
                    else                        // no merge, no move
                    {
                        lastCard = i;
                    }

                    beforeCards++;
                }
            }
        }
    }

    return false;
}

void Board::Update()
{
    Rectangle::Update();

    // Do not do these rendering together,
    // otherwise cards will be covered when moving
    for(int i = 0; i < cardNum; i++)
        for(int j = 0; j < cardNum; j++)
        {
            cardBackground[i][j]->Update();
        }

    for(int i = 0; i < cardNum; i++)
        for(int j = 0; j < cardNum; j++)
        {
            if(cards[i][j] != nullptr)
            {
                cards[i][j]->Update();
            }
        }

    for(std::list<Card *>::iterator i = destroyList.begin(); i != destroyList.end();)
    {
        (*i)->Update();
        if(!(*i)->active)
        {
            delete *i;
            i = destroyList.erase(i);
        }
        else
        {
            i++;
        }
    }
}
