#include "BoardRecord.h"

BoardRecord::BoardRecord()
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
    {
            boardMap[i][j] = -1;
    }
}

