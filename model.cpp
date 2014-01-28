#include "model.h"

using namespace std;

const int CARD_COUNT = 52;
Card *cards_[CARD_COUNT];
deque<Rank> tableC; //cards on table for Clubs
deque<Rank> tableD; //cards on table for diamonds
deque<Rank> tableH; //cards on table for hearts
deque<Rank> tableS; //cards on table for spades
Player *players [4];
int currentPlayer=-1;
std::deque<int> winners;
std::string roundScores = "";


Model::Model()
{

}

//begins the game with random seed and shuffle deck. Also determines human players and computer players
void Model::seed(string seed, deque<string> & playerTypes)
{
    for(int i = 0; i<CARD_COUNT; i++){
        cards_[i] = new Card((Suit)(i/13), (Rank)(i%13));
    }

    srand48(atol(seed.c_str()));
    cout << atol(seed.c_str()) <<endl;
    for(int i= 0; i<4; i++){
        if(playerTypes[i]=="Computer")
            players[i] = new Computer(i+1);
        else if(playerTypes[i]=="Human")
            players[i] = new Human(i+1);
    }
    roundScores = "";
    this->newRoundStart();

}

//starts a new round with a reshuffle and split the deck into 4 people's hands
void Model::newRoundStart()
{
    //shuffle
    int n = CARD_COUNT;

	while ( n > 1 ) {
		int k = (int) (lrand48() % n);
		--n;
		Card *c = cards_[n];
		cards_[n] = cards_[k];
		cards_[k] = c;
	}

    //reset tables
    tableC.clear();
    tableD.clear();
    tableH.clear();
    tableS.clear();

    currentPlayer=0;

    //split the deck amongst the first four players
    for(int i = 0; i<CARD_COUNT; i++){
        players[i/13]->setHand(cards_[i]);
        if(cards_[i]->getSuit()==3 && cards_[i]->getRank()==6){
            currentPlayer=i/13;
        }
    }
    cout << "A new round begins. It's player " << currentPlayer+1 << "'s turn to play." << endl;
    notify();
    players[currentPlayer]->turn(tableC, tableD, tableH, tableS);
    while (players[currentPlayer]->getptype() != "h")
    {
        currentPlayer = (++currentPlayer)%4;
        if (players[currentPlayer]->hand.size() == 0)
        {
            this->endRound();
            return;
        }
        players[currentPlayer]->turn(tableC, tableD, tableH, tableS);
    }
    notify();
}

//end round displaying round scores and updates the view with associated round end information
void Model::endRound()
{
    for (int i = 0; i < 4; i++)
        roundScores = roundScores + players[i]->countRoundScore();
    notify();
    roundScores = "";
    //game has not ended
    if (players[0]->getTotalScore() < 80 && players[1]->getTotalScore() < 80 && players[2]->getTotalScore() < 80 && players[3]->getTotalScore() < 80)
        this->newRoundStart();
    else
    {
        //tally score at the end of game
        int minscore = players[0]->getTotalScore();

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
        notify();
    }
}

//accessor for the controller to get the cards on the table
deque<deque<int> > Model::getCardsOnTable()
{
    deque<deque<int> > cardsOnTable;

    deque<int> tempC;
    for (int i = 0; i<tableC.size(); i++)
    {
        tempC.push_back((int)(tableC[i]));
    }
    cardsOnTable.push_back(tempC);

    deque<int> tempD;
    for (int i = 0; i<tableD.size(); i++)
    {
        tempD.push_back((int)(tableD[i]));
    }
    cardsOnTable.push_back(tempD);

    deque<int> tempH;
    for (int i = 0; i<tableH.size(); i++)
    {
        tempH.push_back((int)(tableH[i]));
    }
    cardsOnTable.push_back(tempH);

    deque<int> tempS;
    for (int i = 0; i<tableS.size(); i++)
    {
        tempS.push_back((int)(tableS[i]));
    }
    cardsOnTable.push_back(tempS);

    return cardsOnTable;
}

//accessor for the controller to get the current player hand
deque<pair<int, int> > Model::getCurrentPlayerHand()
{
    deque<pair<int, int> > currentPlayerHand;
    if (currentPlayer == -1)
        return currentPlayerHand;
    for (int i = 0; i<players[currentPlayer]->hand.size(); i++)
    {
        Card *c = players[currentPlayer]->hand[i];
        pair<int, int> p;
        p.first = c->getRank();
        p.second = c->getSuit();
        currentPlayerHand.push_back(p);
    }
    return currentPlayerHand;
}

//quits the game and clears all associated memory and clears round score. notifies observers
void Model::quit()
{
    tableC.clear();
    tableD.clear();
    tableH.clear();
    tableS.clear();
    winners.clear();
    roundScores = "";
    currentPlayer=-1;
    for(int i = 0; i<CARD_COUNT; i++){
        delete cards_[i];
    }
    notify();
}

//plays the game, either play a legal play or discards
void Model::play(int pos)
{
    Human * h = dynamic_cast<Human*>(players[currentPlayer]);
    bool acceptablePlayMade = false;

    //check for ragequit
    if (pos == -1)
    {
        cout << "Player " << currentPlayer + 1 << " ragequits. A computer will now take over." << endl;
        players[currentPlayer] = new Computer(h);
        delete h;
        acceptablePlayMade = true;
        players[currentPlayer]->turn(tableC, tableD, tableH, tableS);
    }
    else
    {
        //check to see if a blank card is clicked
        if (pos >= h->hand.size())
            return;
        //play the card
        deque<Card*> playable = players[currentPlayer]->findLegalPlays(tableC, tableD, tableH, tableS);

        if(playable.size() != 0)
        {
            //call play with respective table
            if(h->hand[pos]->getSuit()==0)
                acceptablePlayMade = h->play(*(h->hand[pos]), tableC, false, tableS);
            else if(h->hand[pos]->getSuit()==1)
                acceptablePlayMade = h->play(*(h->hand[pos]), tableD, false, tableS);
            else if(h->hand[pos]->getSuit()==2)
                acceptablePlayMade = h->play(*(h->hand[pos]), tableH, false, tableS);
            else if(h->hand[pos]->getSuit()==3)
                acceptablePlayMade = h->play(*(h->hand[pos]), tableS, true, tableS);
        }
        else{
            acceptablePlayMade = h->discard(*(h->hand[pos]), tableC, tableD, tableH, tableS);
        }
    }

    //if an acceptable play is made, run computer turns until it's another human player's turn
    if (acceptablePlayMade)
    {
        currentPlayer = (++currentPlayer)%4;
        if (players[currentPlayer]->hand.size() == 0)
        {
            this->endRound();
            return;
        }
        players[currentPlayer]->turn(tableC, tableD, tableH, tableS);
        //loop until next human player
        while (players[currentPlayer]->getptype() != "h")
        {
            currentPlayer = (++currentPlayer)%4;
            if (players[currentPlayer]->hand.size() == 0)
            {
                this->endRound();
                return;
            }
            players[currentPlayer]->turn(tableC, tableD, tableH, tableS);
        }
    }
    notify();

}

//accessor for controller to get round scores
string Model::getRoundScores()
{
    return roundScores;
}

//accessor for controller to get winners
deque<int> Model::getWinners()
{
    return winners;
}

//accessor for controller to get discards pile size
int Model::getDiscardSize(int i)
{
    return players[i]->getDiscardSize();
}

//accessor for controlle rto get total scores of each player
int Model::getTotalScore(int i)
{
    return players[i]->getScore();
}

//accessor for controller to get the current player's number
int Model::getCurPlayer()
{
    return currentPlayer+1;
}
