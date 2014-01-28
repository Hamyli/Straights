#ifndef MODEL_H
#define MODEL_H

#include "subject.h"
#include "Player.h"
#include "Computer.h"
#include "Human.h"
#include "Card.h"
#include <deque>
#include <cstdlib>
#include <iostream>

class Model : public Subject {
public:
    Model();
    void seed(std::string, std::deque<std::string>&);
    void newRoundStart();
    std::deque<std::pair<int, int> > getCurrentPlayerHand();
    void quit();
    std::deque<std::deque<int> > getCardsOnTable();
    void play(int);
    void endRound();
    std::string getRoundScores();
    std::deque<int> getWinners();
    int getDiscardSize(int);
    int getTotalScore(int);
    int getCurPlayer();
}; // Model


#endif
