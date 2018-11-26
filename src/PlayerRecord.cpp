#include "PlayerRecord.h"

std::vector<PlayerRecord> PlayerRecord::playerRecords;


void PlayerRecord::init(const std::string &dataFile)
{
    std::string name;
    int bestScore;
    std::ifstream read;
    read.open(dataFile, std::ios::in);

    while(!read.eof())
    {
        read >> name >> bestScore;

        if(name == "")
            continue;
        playerRecords.push_back(PlayerRecord(name, bestScore));
    }

    read.close();
}

PlayerRecord::PlayerRecord(const std::string &name)
{
    bool isFind = false;
    this->name = name;

    for(unsigned int i = 0; i < playerRecords.size(); i++)
    {
        if(playerRecords[i].getName() == name)
        {
            bestScore = playerRecords[i].getBestScore();
            isFind = true;
            break;
        }
    }

    if(!isFind)
    {
        bestScore = 0;
        playerRecords.push_back(*this);
    }
}

PlayerRecord::PlayerRecord(const std::string &name, int bestScore)
{
    this->name = name;
    this->bestScore = bestScore;
}

std::string PlayerRecord::getName() const
{
    return name;
}

int PlayerRecord::getBestScore() const
{
    return bestScore;
}

void PlayerRecord::setBestScore(int bestScore)
{
    this->bestScore = bestScore;
}

void PlayerRecord::save(const std::string &dataFile)
{
    std::ofstream write;
    write.open(dataFile, std::ios::out);

    for(unsigned int i = 0; i < playerRecords.size(); i++)
    {
        write << playerRecords[i].getName() << " " << playerRecords[i].getBestScore();

        if(i < playerRecords.size() - 1)
            write << std::endl;
    }
    write.close();
}

bool PlayerRecord::operator>(const PlayerRecord &b) const
{
    return bestScore > b.bestScore;
}

bool PlayerRecord::operator<(const PlayerRecord &b) const
{
    return bestScore < b.bestScore;
}
