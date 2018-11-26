// PlayerRecord.h
// Author: Bairuo

#ifndef PLAYERRECORD_H_
#define PLAYERRECORD_H_

#include<string>
#include<iostream>
#include<fstream>
#include<vector>

class PlayerRecord
{
public:
    PlayerRecord(const std::string &name);
    PlayerRecord(const std::string &name, int bestScore);

    void save(const std::string &dataFile);

    static void init(const std::string &dataFile);
    static std::vector<PlayerRecord> playerRecords;

    bool operator<(const PlayerRecord &b) const;
    bool operator>(const PlayerRecord &b) const;

    std::string getName() const;
    int getBestScore() const;
    void setBestScore(int bestScore);

private:
    int score = 0;
    int bestScore = 0;
    std::string name;
};

#endif // PLAYERRECORD_H_


