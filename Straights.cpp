#include "Player.h"
#include "Computer.h"
#include "Human.h"
#include "Card.h"
#include "Command.h"
#include <cassert>
#include <deque>
#include <cstdlib>

using namespace std;

const int CARD_COUNT = 52;
Card *cards_[CARD_COUNT];
deque<Rank> tableC; //cards on table for Clubs
deque<Rank> tableD; //cards on table for diamonds
deque<Rank> tableH; //cards on table for hearts
deque<Rank> tableS; //cards on table for spades

void createDeck(){
    for(int i = 0; i<CARD_COUNT; i++){
       cards_[i] = new Card((Suit)(i/13), (Rank)(i%13));
    }
}

void deleteDeck(){
    for(int i = 0; i<CARD_COUNT; i++){
        delete cards_[i];
    }
}
void shuffle(){
	int n = CARD_COUNT;

	while ( n > 1 ) {
		int k = (int) (lrand48() % n);
		--n;
		Card *c = cards_[n];
		cards_[n] = cards_[k];
		cards_[k] = c;
	}
}

int main(int argc, char* argv[]){
    //invite players
    Player *players [4];

    for(int i= 0; i<4; i++){
        cout << "Is player " << i+1 << " a human(h) or a computer(c)?" << endl << ">";
        string input;
        cin >> input;
        assert(input=="c" || input=="C" || input=="h" || input=="H");

        if(input=="c"||input=="C")
            players[i] = new Computer(i+1);
        else if(input=="h" || input=="H")
            players[i] = new Human(i+1);
    }

    createDeck();

    //seeding the random number 
    if(argc==2)
	srand48(atol(argv[1]));
    else
	srand48(0);

    //game loops while no one has more than 80 points
    while (players[0]->getTotalScore() < 80 && players[1]->getTotalScore() < 80 && players[2]->getTotalScore() < 80 && players[3]->getTotalScore() < 80)
    {
        shuffle();

        //reset tables
        tableC.clear();
        tableD.clear();
        tableH.clear();
        tableS.clear();
        int currentPlayer=0;
        //split the deck amongst the first four players
        for(int i = 0; i<CARD_COUNT; i++){
            players[i/13]->setHand(cards_[i]);
            if(cards_[i]->getSuit()==3 && cards_[i]->getRank()==6){
                currentPlayer=i/13;
            }
        }

        //gameplay starts
        cout << "A new round begins. It's player " << currentPlayer+1 << "'s turn to play." << endl;

        //loop for one round of the game
        for(int i = 0; i<52; i++)
        {
            players[currentPlayer]->turn(tableC, tableD, tableH, tableS);
            //human player turn
	    if(players[currentPlayer]->getptype() == "h")
            {
                Human * h = dynamic_cast<Human*>(players[currentPlayer]);
                Command c;
                bool acceptablePlayMade = false;
                while (!acceptablePlayMade)
                {
                    cout << ">";
                    cin >> c;
                    if(c.type == PLAY)
                    {
			//call play with respective table 
                        if(c.card.getSuit()==0)
                            acceptablePlayMade = h->play(c.card, tableC);
                        else if(c.card.getSuit()==1)
                            acceptablePlayMade = h->play(c.card, tableD);
                        else if(c.card.getSuit()==2)
                            acceptablePlayMade = h->play(c.card, tableH);
                        else if(c.card.getSuit()==3)
                            acceptablePlayMade = h->play(c.card, tableS);
                    }
                    else if(c.type == RAGEQUIT){
                        cout << "Player " << currentPlayer + 1 << " ragequits. A computer will now take over." << endl;
                        players[currentPlayer] = new Computer(h);
                        delete h;
                        acceptablePlayMade = true;
                        //decrement currentPlayer and i to restart this turn as computer
                        currentPlayer--;
                        i--;
                    }
                    else if(c.type == DECK){
                        for (int i = 0; i<CARD_COUNT; i++)
                        {
                            cout << *(cards_[i]);
                            if (i%13 == 12)
                                cout << endl;
                            else
                                cout << " ";
                        }
                    }
                    else if(c.type == DISCARD){
                        acceptablePlayMade = h->discard(c.card, tableC, tableD, tableH, tableS);
                    }
                    else if(c.type == QUIT){
                        return 0;
                    }
                }
            }
            currentPlayer = (++currentPlayer)%4;
        }
        //total scores
        for (int i = 0; i < 4; i++)
            players[i]->countRoundScore();
    }
    //tally score at the end of game
    int minscore = players[0]->getTotalScore();
    deque<int> winners;
    for (int i = 0; i < 4; i++)
    {
        if (players[i]->getTotalScore() < minscore)
            minscore = players[i]->getTotalScore();
    }
    for (int i = 0; i < 4; i++)
    {
        if (players[i]->getTotalScore() == minscore)
            winners.push_back(i+1);
        delete players[i];
    }
    for (int i = 0; i<winners.size(); i++)
        cout << "Player " << winners[i] << " wins!" << endl;
    deleteDeck();
    return 0;
}


