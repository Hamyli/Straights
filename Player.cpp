#include <iostream>
#include <deque>
#include "Card.h"
#include "Player.h"
#include <algorithm>
#include <sstream>
using namespace std;

bool Player::checkIfValid(deque<Rank> &Table, int ranknum, bool isSpades, deque<Rank> & tableS)
{
	if ((isSpades || tableS.size() != 0) && Table.size() == 0 && ranknum == 6)
	{
		Table.push_back((Rank)ranknum);
		return true;
	}
	else if (Table.size() != 0)
    {

        if (Table[0] == ranknum+1)
        {
            Table.push_front((Rank)ranknum);
            return true;
        }
        else if (Table[Table.size()-1] == ranknum-1)
        {
            Table.push_back((Rank)ranknum);
            return true;
        }
    }
	return false;
}

deque<Card*> Player::findLegalPlays(const std::deque<Rank>& tableC, const std::deque<Rank>& tableD, const std::deque<Rank>& tableH, const std::deque<Rank>& tableS)
{
    //check for legal plays all four suits of the cards on the table
    deque<Card> temp;
    if (tableC.size() == 0)
        temp.push_back(Card((Suit)0, (Rank)6));
    else
    {
        if (tableC[0] != 0)
        {
            temp.push_back(Card((Suit)0, (Rank)(tableC[0]-1)));
        }
        if (tableC[tableC.size()-1] != 12)
        {
            temp.push_back(Card((Suit)0, (Rank)(tableC[tableC.size()-1]+1)));
        }
    }
    if (tableD.size() == 0)
        temp.push_back(Card((Suit)1, (Rank)6));
    else
    {
        if (tableD[0] != 0)
        {
            temp.push_back(Card((Suit)1, (Rank)(tableD[0]-1)));
        }
        if (tableD[tableD.size()-1] != 12)
        {
            temp.push_back(Card((Suit)1, (Rank)(tableD[tableD.size()-1]+1)));
        }
    }
    if (tableH.size() == 0)
        temp.push_back(Card((Suit)2, (Rank)6));
    else
    {
        if (tableH[0] != 0)
        {
            temp.push_back(Card((Suit)2, (Rank)(tableH[0]-1)));
        }
        if (tableH[tableH.size()-1] != 12)
        {
            temp.push_back(Card((Suit)2, (Rank)(tableH[tableH.size()-1]+1)));
        }
    }
    if (tableS.size() == 0)
        temp.push_back(Card((Suit)3, (Rank)6));
    else
    {
        if (tableS[0] != 0)
        {
            temp.push_back(Card((Suit)3, (Rank)(tableS[0]-1)));
        }
        if (tableS[tableS.size()-1] != 12)
        {
            temp.push_back(Card((Suit)3, (Rank)(tableS[tableS.size()-1]+1)));
        }
    }

    if(tableC.size()==0 && tableD.size()==0 && tableH.size()==0 && tableS.size()==0)
    {
	temp.clear();
        temp.push_back(Card((Suit)3, (Rank)6));
    }

    deque<Card*> possibleMovesList;
    for(int i = 0; i<hand.size(); i++)
    {
        if (find(temp.begin(), temp.end(),*(hand[i])) != temp.end())
            possibleMovesList.push_back(hand[i]);
    }
    return possibleMovesList;
}

void Player::turn(std::deque<Rank>& a, std::deque<Rank>& b, std::deque<Rank>& c, std::deque<Rank>& d){}

//creates a player with an associated player number
Player::Player(int num){
    this->totalScore = 0;
    this->playerNum = num;
}

//plays a card from player's hand, returns true if successful, false if play is not valid
bool Player::play (const Card c, deque<Rank> &table, bool isSpades, deque<Rank>& TableS)
{
	if (checkIfValid(table, c.getRank(), isSpades, TableS))
	{
	    //deletes card
        deque<Card*>::iterator it = hand.begin();
        while (!(*(*it) == c))
            it++;
		hand.erase(it);
		cout << "Player " << playerNum << " plays " << c << "." << endl;
		return true;
	}
	else
	{
		cout << "This is not a legal play." << endl;
		return false;
	}
}

//discards a card from player's hand. returns false is a valid move exists (cannot discard)
bool Player::discard(Card c, deque<Rank>& tableC, deque<Rank>& tableD, deque<Rank>& tableH, deque<Rank>& tableS){
    if (findLegalPlays(tableC, tableD, tableH, tableS).size() == 0)
        cout << "Player " << playerNum << " discards " << c << "." << endl;
    else
    {
        cout << "You have a legal play. You may not discard." << endl;
        return false;
    }
	discardPile.push_back(c);
    deque<Card*>::iterator it = hand.begin();
    while (!(*(*it) == c))
        it++;
    hand.erase(it);
    return true;
}

//tally the round score and prints the discard pile of each player
string Player::countRoundScore(){
    stringstream ss;
    int roundScore = 0;
    cout << "Player " << playerNum << "'s discards: ";
    ss << "Player " << playerNum << "'s discards: ";
    for (int i = 0; i<discardPile.size(); i++)
    {
        cout << discardPile[i] << " ";
        ss << discardPile[i] << " ";
        roundScore += (discardPile[i].getRank()+1);
    }
    cout << endl << "Player " << playerNum << "'s score: " << totalScore << " + " << roundScore << " = ";
    ss << "\nPlayer " << playerNum << "'s score: " << totalScore << " + " << roundScore << " = ";
    totalScore += roundScore;
    cout << totalScore << endl;
    ss << totalScore << "\n";
    discardPile.clear();
    return ss.str();
}
//accessor for total score
int Player::getTotalScore(){
    return this->totalScore;
}
//adds cards to player's hand
void Player::setHand(Card * c)
{
    hand.push_back(c);
}
//access for player type, either human or computer
string Player::getptype(){
    return this->ptype;
}

int Player::getDiscardSize(){
    return discardPile.size();
}

int Player::getScore(){
    return totalScore;
}