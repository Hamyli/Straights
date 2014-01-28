#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <iostream>
#include "Card.h"
#include <deque>
#include <string>

class Player{
public:
    Player(int);
    virtual void turn(std::deque<Rank>&, std::deque<Rank>&, std::deque<Rank>&, std::deque<Rank>&);
    bool play(Card, std::deque<Rank>&, bool, std::deque<Rank>&);
    bool discard(Card, std::deque<Rank>&, std::deque<Rank>&, std::deque<Rank>&, std::deque<Rank>&);
    void setHand(Card*);
    std::string countRoundScore();
    int getTotalScore();
    std::string getptype();
    std::deque<Card*> hand;
    std::deque<Card*> findLegalPlays(const std::deque<Rank>&, const std::deque<Rank>&, const std::deque<Rank>&, const std::deque<Rank>&);
    int getDiscardSize();
    int getScore();
    friend class Computer;
protected:
    int playerNum;
    int totalScore;
    std::deque<Card> discardPile;
    std::string ptype;
    bool checkIfValid(std::deque<Rank>&, int, bool, std::deque<Rank>&);

};

#endif // PLAYER_H_INCLUDED

