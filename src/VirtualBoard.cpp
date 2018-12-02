#include "VirtualBoard.h"
#include "Board.h"
#include "Card.h"

VirtualBoard::VirtualBoard() = default;

VirtualBoard::VirtualBoard(Board *board)
{
    emptyNum = 0;
    maxScore = 0;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
    {
        if(board->cards[i][j] != nullptr)
        {
            map[i][j] = board->cards[i][j]->score;
            maxScore = map[i][j] > maxScore ? map[i][j] : maxScore;
        }
        else
        {
            map[i][j] = 0;
            emptyNum++;
        }
    }
}

static VirtualBoard* virBoardPool = nullptr;

const int virBoarPoolSize = 50000;
int VirtualBoard::virBoardPoolPointer = 0;

int VirtualBoard::GetPoolPoint()
{
    return virBoardPoolPointer;
}

void VirtualBoard::InitPool()
{
    if(virBoardPool == nullptr)
    {
        virBoardPool = new VirtualBoard[virBoarPoolSize];
    }
}

void VirtualBoard::DeletePool()
{
    if(virBoardPool != nullptr)
    {
        delete[] virBoardPool;
        virBoardPool = nullptr;
        virBoardPoolPointer = 0;
    }
}

VirtualBoard* VirtualBoard::GetVirtualBoard()
{
    if(virBoardPoolPointer == virBoarPoolSize)
    {
        virBoardPoolPointer = 0;
    }

    return &virBoardPool[virBoardPoolPointer++];
}

void VirtualBoard::ReturnLastToPool()
{
    virBoardPoolPointer--;
    if(virBoardPoolPointer < 0)
    {
        virBoardPoolPointer = virBoarPoolSize - 1;
    }
}

VirtualBoard* VirtualBoard::Clone()
{
    //VirtualBoard *p = new VirtualBoard();
    VirtualBoard *p = GetVirtualBoard();
    *p = *this;
    return p;
}

VirtualBoard* VirtualBoard::Refresh(int num, int score)
{
    VirtualBoard *p = Clone();

    int i = num / 4;
    int j = num % 4;

    if(p->map[i][j] > 0)
    {
        ReturnLastToPool();
        return nullptr;
    }

    p->map[i][j] = score;
    p->emptyNum--;

    return p;
}

VirtualBoard* VirtualBoard::Up()
{
    VirtualBoard *p = Clone();

    int beforeCards, lastCard;
    bool beforeCanMerge;
    bool isMove = false;

    for(int i = 0; i < 4; i++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int j = 0; j < 4; j++)
        {
            if(p->map[i][j] != 0)
            {
                if(lastCard >= 0 && beforeCanMerge == true && p->map[i][lastCard] == p->map[i][j])
                                                // move and merge
                {
                    p->map[i][j] = 0;
                    p->map[i][beforeCards - 1] <<= 1;

                    p->emptyNum++;
                    maxScore = maxScore > p->map[i][beforeCards - 1] ? maxScore : p->map[i][beforeCards - 1];

                    isMove = true;
                    beforeCanMerge = false;
                }
                else
                {
                    if(j - beforeCards > 0)     // no merge but move
                    {
                        p->map[i][beforeCards] = p->map[i][j];
                        p->map[i][j] = 0;

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
        return p;
    }
    else
    {
        ReturnLastToPool();
        return nullptr;
    }
}

VirtualBoard* VirtualBoard::Down()
{
    VirtualBoard *p = Clone();

    int beforeCards, lastCard;
    bool beforeCanMerge;
    bool isMove = false;

    for(int i = 0; i < 4; i++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int j = 3; j >= 0; j--)
        {
            if(p->map[i][j] != 0)
            {
                if(lastCard >= 0 && beforeCanMerge == true && p->map[i][lastCard] == p->map[i][j])
                                                // move and merge
                {
                    p->map[i][j] = 0;
                    p->map[i][4 - beforeCards] <<= 1;

                    p->emptyNum++;
                    maxScore = maxScore > p->map[i][4 - beforeCards] ? maxScore : p->map[i][4 - beforeCards];

                    isMove = true;

                    beforeCanMerge = false;
                }
                else
                {
                    if(j + 1 + beforeCards < 4)     // no merge but move
                    {
                        p->map[i][3 - beforeCards] = p->map[i][j];
                        p->map[i][j] = 0;

                        lastCard = 3 - beforeCards;

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
        return p;
    }
    else
    {
        ReturnLastToPool();
        return nullptr;
    }
}

VirtualBoard* VirtualBoard::Left()
{
    VirtualBoard *p = Clone();

    int beforeCards, lastCard;
    bool beforeCanMerge;
    bool isMove = false;

    for(int j = 0; j < 4; j++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int i = 0; i < 4; i++)
        {
            if(p->map[i][j] != 0)
            {
                if(lastCard >= 0 && beforeCanMerge == true && p->map[lastCard][j] == p->map[i][j])
                                                // move and merge
                {
                    p->map[i][j] = 0;
                    p->map[beforeCards - 1][j] <<= 1;

                    p->emptyNum++;
                    maxScore = maxScore > p->map[beforeCards - 1][j] ? maxScore : p->map[beforeCards - 1][j];

                    isMove = true;

                    beforeCanMerge = false;
                }
                else
                {
                    if(i - beforeCards > 0)     // no merge but move
                    {
                        p->map[beforeCards][j] = p->map[i][j];
                        p->map[i][j] = 0;

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
        return p;
    }
    else
    {
        ReturnLastToPool();
        return nullptr;
    }
}

VirtualBoard* VirtualBoard::Right()
{
    VirtualBoard *p = Clone();

    int beforeCards, lastCard;
    bool beforeCanMerge;
    bool isMove = false;

    for(int j = 0; j < 4; j++)
    {
        lastCard = -1;
        beforeCards = 0;
        for(int i = 3; i >= 0; i--)
        {
            if(p->map[i][j] != 0)
            {
                if(lastCard >= 0 && beforeCanMerge == true && p->map[lastCard][j] == p->map[i][j])
                                                // move and merge
                {
                    p->map[i][j] = 0;
                    p->map[4 - beforeCards][j] <<= 1;

                    p->emptyNum++;
                    maxScore = maxScore > p->map[4 - beforeCards][j] ? maxScore : p->map[4 - beforeCards][j];

                    isMove = true;

                    beforeCanMerge = false;
                }
                else
                {
                    if(i + beforeCards + 1 < 4)     // no merge but move
                    {
                        p->map[3 - beforeCards][j] = p->map[i][j];
                        p->map[i][j] = 0;

                        lastCard = 3 - beforeCards;

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
        return p;
    }
    else
    {
        ReturnLastToPool();
        return nullptr;
    }
}
