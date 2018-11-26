#include "AI.h"
#include "VirtualBoard.h"
#include "AnswerNode.h"
#include "GameTime.h"
#include <cmath>
#include "time.h"

static const float smoothWeight = 0.1f;
static const float monoWeight = 1.0f;
static const float emptyWeight = 2.7f;
static const float maxWeight = 1.0f;

static const float depthSearchTime = 0.03f;
static int maxDepth = 14;

static const int nodePoolSize = 30000;
static AnswerNode* pool = nullptr;
static int poolPointer = 0;

static AnswerNode* GetAnswerNode(VirtualBoard *board);
static bool AddMinNode(AnswerNode *node, AnswerNode *son, int operate, int depth);
static float Search(AnswerNode *node, int depth);      // return alpha or beta

static float Eval(VirtualBoard *board);
static float EvalEmpty(int emptyNum);
static float EvalSmoothness(VirtualBoard *board);
static float EvalMonotonicity(VirtualBoard *board);

AI::AI(Board *board)
{
    if(pool == nullptr)
    {
        pool = new AnswerNode[nodePoolSize];
    }

    VirtualBoard::InitPool();
}

AI::~AI()
{
    if(pool != nullptr)
    {
        delete[] pool;
        pool = nullptr;
        poolPointer = 0;
    }

    VirtualBoard::DeletePool();
}

int AI::GetOperate() const
{
    if(root->board->maxScore >= 2048)
    {
        return 4;
    }

    if(root->maxOperate == -2)
    {
        return -1;
    }

    return root->maxOperate;

}

void AI::Update()
{
    timer += GameTime::deltaTime;
}

void AI::StartSearch(Board *board)
{
    root.reset(new AnswerNode(new VirtualBoard(board)));
    root->nodeType = MaxNode;

    timer = 0;

    searchThread.reset(new std::thread(Search, root.get(), 0), [](std::thread *pt) -> void {pt->join();});
}

void AI::StopSearch()
{
    searchThread.reset();
}

AnswerNode* GetAnswerNode(VirtualBoard *board)
{
    if(poolPointer == nodePoolSize)
    {
        poolPointer = 0;
    }

    pool[poolPointer].board = board;
    pool[poolPointer].alpha = -inf;
    pool[poolPointer].beta = inf;
    pool[poolPointer].maxOperate = -2;

    return &pool[poolPointer++];
}

bool AddMinNode(AnswerNode *node, AnswerNode *son, int operate, int depth)
{
    son->alpha = node->alpha;
    son->nodeType = MinNode;
    float sonBeta = Search(son, depth + 1);

    if(sonBeta >= node->alpha)
    {
        node->alpha = sonBeta;
        node->maxOperate = operate;
    }

    if(node->beta < node->alpha)
    {
        return true;    // cut
    }
    else
    {
        return false;
    }
}


float Search(AnswerNode *node, int depth)
{
    if(depth >= maxDepth)
    {
        float eval = Eval(node->board);
        if(node->nodeType == MaxNode)
        {
            node->alpha = eval;
        }
        else
        {
            node->beta = eval;
        }

        return eval;
    }
    else if(node->nodeType == MaxNode)
    {
        VirtualBoard *board1 = node->board->Up();
        if(board1 != nullptr)
        {
            AnswerNode *p = GetAnswerNode(board1);
            if(AddMinNode(node, p, 0, depth))
            {
                return node->alpha;
            }
        }

        VirtualBoard *board2 = node->board->Down();
        if(board2 != nullptr)
        {
            AnswerNode *p = GetAnswerNode(board2);
            if(AddMinNode(node, p, 1, depth))
            {
                return node->alpha;
            }
        }

        VirtualBoard *board3 = node->board->Left();
        if(board3 != nullptr)
        {
            AnswerNode *p = GetAnswerNode(board3);
            if(AddMinNode(node, p, 2, depth))
            {
                return node->alpha;
            }
        }

        VirtualBoard *board4 = node->board->Right();
        if(board4 != nullptr)
        {
            AnswerNode *p = GetAnswerNode(board4);
            if(AddMinNode(node, p, 3, depth))
            {
                return node->alpha;
            }
        }

        return node->alpha;
    }
    else    // MinNode
    {
        float minEval = inf;
        VirtualBoard *minBoard = nullptr;
        VirtualBoard *board = nullptr;

        for(int j = 1; j <= 2; j++)
        {
            for(int num = 0; num < 16; num++)
            {
                board = node->board->Refresh(num, j << 1);

                if(board == nullptr)    // can not refresh
                {
                    continue;
                }

                float eval = Eval(board);
                if(eval < minEval)
                {
                    minBoard = board;
                    minEval = eval;
                }
                else
                {
                    VirtualBoard::ReturnLastToPool();
                }
            }
        }

        AnswerNode *son = GetAnswerNode(minBoard);
        son->beta = node->beta;
        son->nodeType = MaxNode;

        float sonAlpha = Search(son, depth + 1);
        if(sonAlpha < node->beta)
        {
            node->beta = sonAlpha;
        }

        return node->beta;
    }

}

float EvalMonotonicity(VirtualBoard *board)
{
    float increases = 0;

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(board->map[i][j] > 0)
            {
                float value = log(board->map[i][j]) / log(2);
                float targetValue;

                if(j + 1 < 4 && board->map[i][j + 1] > 0)
                {
                    targetValue = log(board->map[i][j + 1]) / log(2);
                    increases += fabs(targetValue - value);
                }

                if(board->map[i + 1][j] > 0)
                {
                    targetValue = log(board->map[i + 1][j]) / log(2);
                    increases += fabs(targetValue - value);
                }
            }
        }
    }

    return -increases;
}

float EvalSmoothness(VirtualBoard *board)
{
    float smoothEval = 0;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(board->map[i][j] > 0)
            {
                float value = log(board->map[i][j]) / log(2);

                for(int right = 3; right > j; right--)
                {
                    if(board->map[i][right] > 0)
                    {
                        float targetValue = log(board->map[i][right]) / log(2);
                        smoothEval -= fabs(targetValue - value);
                        break;
                    }
                }

                for(int down = 3; down > i; down--)
                {
                    if(board->map[down][j] > 0)
                    {
                        float targetValue = log(board->map[down][j]) / log(2);
                        smoothEval -= fabs(targetValue - value);
                        break;
                    }
                }
            }
        }
    }

    return smoothEval;
}

float EvalEmpty(int emptyNum)
{
    if(emptyNum > 0)
    {
        return log(emptyNum);
    }
    else
    {
        return -10000;
    }
}

float Eval(VirtualBoard *board)
{
    return EvalSmoothness(board) * smoothWeight
           + EvalMonotonicity(board) * monoWeight
           + EvalEmpty(board->emptyNum) * emptyWeight
           + board->maxScore * maxWeight;
}
