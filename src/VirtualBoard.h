// VirtualBoard.h
// Author: Bairuo

#ifndef VIRTUALBOARD_H_
#define VIRTUALBOARD_H_

class Board;

class VirtualBoard
{
public:
    int map[4][4];
    int maxScore;
    int emptyNum;

    VirtualBoard();
    VirtualBoard(Board *board);

    VirtualBoard* Refresh(int num, int score);

    VirtualBoard* Up();     // Will return null if do not move
    VirtualBoard* Down();
    VirtualBoard* Left();
    VirtualBoard* Right();

    VirtualBoard* Clone();

    static void InitPool();
    static void DeletePool();
    static int GetPoolPoint();
    static VirtualBoard* GetVirtualBoard();
    static void ReturnLastToPool();

private:
    static int virBoardPoolPointer;
};

#endif // VIRTUALBOARD_H_
