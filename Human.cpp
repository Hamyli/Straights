#include "Human.h"
#include "Card.h"
#include <iostream>

using namespace std;

Human::Human(int num):Player(num)
{
    ptype="h";
}

void Human::turn(deque<Rank> &tableC, deque<Rank> &tableD, deque<Rank> &tableH, deque<Rank> &tableS){
    cout << "Cards on the table: " << endl;
    cout << "Clubs: " << tableC << endl;
    cout << "Diamonds: " << tableD << endl;
    cout << "Hearts: " << tableH << endl;
    cout << "Spades: " << tableS << endl;

    cout << "Your hand: " << this->hand << endl;
    cout << "Legal plays: " << findLegalPlays(tableC,tableD, tableH, tableS) << endl;
}

//output stream for deque<Card*>  
ostream &operator<<(std::ostream &out, const deque<Card*> &a){
    for(int i = 0; i < a.size(); i++){
        out << *(a[i]) << " ";
    }
    return out;
}

//output stream for deque<Rank>
ostream &operator<<(ostream &out, const deque<Rank> &a){
    string ranks[RANK_COUNT] = {"A", "2", "3", "4", "5", "6",
		"7", "8", "9", "10", "J", "Q", "K"};

    for(int i = 0; i < a.size(); i++){
        out << ranks[a[i]] << " ";
    }
    return out;
}

