// BoardRecord.h
// Author: Bairuo

#ifndef BOARDRECORD_H_
#define BOARDRECORD_H_

class BoardRecord
{
public:
    int boardMap[4][4];

    int freshPos1 = -1;
    int freshPos2 = -1;

    int operate;
    int getScore;

    BoardRecord();
};

#endif // BOARDRECORD_H_
