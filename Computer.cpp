#include <iostream>
#include "Computer.h"

using namespace std;

Computer::Computer(int num):Player(num){
    this->ptype="c";
}

Computer::Computer(Player * player):Player(player->playerNum){
    this -> ptype="c";
    this -> hand = player->hand;
}

void Computer::turn(deque<Rank> &TableC, deque<Rank> &TableD, deque<Rank> &TableH, deque<Rank> &TableS)
{
    deque<Card*> possibleMoves = findLegalPlays(TableC,TableD,TableH,TableS);
    if (possibleMoves.size() == 0)
        discard(*(hand[0]),TableC,TableD,TableH,TableS);
    else
    {
        switch ((possibleMoves[0])->getSuit())
        {
            case 0:
                play(*(possibleMoves[0]), TableC, false, TableS);
                break;
            case 1:
                play(*(possibleMoves[0]), TableD, false, TableS);
                break;
            case 2:
                play(*(possibleMoves[0]), TableH, false, TableS);
                break;
            case 3:
                play(*(possibleMoves[0]), TableS, true, TableS);
                break;
            default:
                break;
        }
    }
}

