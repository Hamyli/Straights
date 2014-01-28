#ifndef COMPUTER_H_INCLUDED
#define COMPUTER_H_INCLUDED

#include <iostream>
#include "Card.h"
#include <deque>
#include "Player.h"

class Computer : public Player{
public:
	Computer(int);
	Computer(Player*);
	virtual void turn(std::deque<Rank>&, std::deque<Rank>&, std::deque<Rank>&, std::deque<Rank>&);

};

#endif

