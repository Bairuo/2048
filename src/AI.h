// AI.h
// Author: Bairuo

#ifndef AI_H_
#define AI_H_

#include <thread>
#include <boost/shared_ptr.hpp>

class Board;
class VirtualBoard;
class AnswerNode;

class AI
{
public:
    AI(Board *board);
    ~AI();

    void Update();
    int GetOperate() const;

    void StartSearch(Board *board);
    void StopSearch();

private:
    boost::shared_ptr<AnswerNode> root;
    boost::shared_ptr<std::thread> searchThread;
    float timer;
};


#endif // AI_H_
