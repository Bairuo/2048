// AnswerNode.h
// Author: Bairuo

#ifndef ANSWERNODE_H_
#define ANSWERNODE_H_

#include "VirtualBoard.h"

#define inf 0x7f7f0000

enum NodeType{MaxNode, MinNode};

class AnswerNode
{
public:
    float alpha = -inf;
    float beta = inf;
    int maxOperate = -2;

    NodeType nodeType;
    VirtualBoard *board;

    AnswerNode();
    AnswerNode(VirtualBoard *board);
    ~AnswerNode();
};

typedef AnswerNode AnswerTree;

#endif // ANSWERNODE_H_
