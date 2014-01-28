#ifndef HUMAN_H_INCLUDED
#define HUMAN_H_INCLUDED

#include <iostream>
#include <vector>
#include "Card.h"
#include "Player.h"


class Human : public Player{
public:
    Human(int);
    virtual void turn(std::deque<Rank>&, std::deque<Rank>&, std::deque<Rank>&, std::deque<Rank>&);
};

std::ostream &operator<<(std::ostream &, const std::deque<Card*> &);
std::ostream &operator<<(std::ostream &, const std::deque<Rank> &);
#endif // HUMAN_H_INCLUDED

