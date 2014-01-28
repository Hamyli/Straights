#include "view.h"
#include <sstream>
#include <iostream>
using namespace std;

// Creates buttons with labels. Sets butBox elements to have the same size, 
// with 10 pixels between widgets
View::View(Controller *c, Model *m): model_(m),controller_(c), new_button( "Start new game with seed:" ), quit_button( "End current game" ), seedPanel(true, 10), table_box(true, 10){
    
    const Glib::RefPtr<Gdk::Pixbuf> nullCardPixbuf = deck.getNullCardImage();
    
	// Sets some properties of the window.
    set_size_request(1000, 650);
    set_border_width(10);
    
    table_frame.set_label("Cards on table");
    table_frame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_TOP);
    table_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    
    // Add gameplay panels
    add(mainBox);
    mainBox.add(seedPanel);
    mainBox.add(table_frame);
    mainBox.add(players_box);
    mainBox.add(hand);
    
    // Add buttons to the seedPanel. Buttons initially invisible gg
    seedPanel.add( new_button );
    seedPanel.add( seed_bar );
    seedPanel.add( quit_button );
    seed_bar.set_text("0");
    quit_button.set_sensitive(false);
    
    // Associate button "clicked" events with local onButtonClicked() method defined below.
    new_button.signal_clicked().connect( sigc::mem_fun( *this, &View::newgameButtonClicked ) );
    quit_button.signal_clicked().connect( sigc::mem_fun( *this, &View::quitButtonClicked ) );
    
    //add table
    table_frame.add(table_box);
    table_box.add(club_box);
    table_box.add(diamond_box);
    table_box.add(heart_box);
    table_box.add(spade_box);
    
    for (int i = 0; i < 13; i++ ) {
		card[i] = new Gtk::Image( nullCardPixbuf );
		club_box.add( *card[i] );
	} // for
    for (int i = 13; i < 26; i++ ) {
		card[i] = new Gtk::Image( nullCardPixbuf );
		diamond_box.add( *card[i] );
	} // for
    for (int i = 26; i < 39; i++ ) {
		card[i] = new Gtk::Image( nullCardPixbuf );
		heart_box.add( *card[i] );
	} // for
    for (int i = 39; i < 52; i++ ) {
		card[i] = new Gtk::Image( nullCardPixbuf );
		spade_box.add( *card[i] );
	} // for
    
    for (int i = 0; i < 4; i++)
    {
        stringstream ss;
        ss << i + 1;
        player_frame[i] = new Gtk::Frame();
        player_frame[i]->set_label("Player " + string(ss.str()));
        players_box.add(*player_frame[i]);
        players[i] = new Gtk::VBox();
        player_frame[i]->add(*players[i]);
        rage_button[i] = new Gtk::Button("Human");
        rage_button[i]->signal_clicked().connect(sigc::bind<int> ( sigc::mem_fun( *this, &View::rageButtonClicked ), i ) );
        players[i]->add(*rage_button[i]);
        points[i] = new Gtk::Label("0 points");
        players[i]->add(*points[i]);
        discards[i] = new Gtk::Label("0 discards");
        players[i]->add(*discards[i]);
    }
    
    hand.add(handCards);
    for (int i = 0; i < 13; i++)
    {
        handCard[i] = new Gtk::Button();
        cardInHand[i] = new Gtk::Image(nullCardPixbuf);
        handCard[i]->set_image(*cardInHand[i]);
        handCards.add(*handCard[i]);
        handCard[i]->set_sensitive(false);
        handCard[i]->signal_clicked().connect(sigc::bind<int> ( sigc::mem_fun( *this, &View::handCardButtonClicked ), i ) );
    }
       
    show_all();
    
    model_->subscribe(this);
    
} // View::View

View::~View() {}

void View::rageButtonClicked(int i)
{
    if (rage_button[i]->get_label() == "Human")
        rage_button[i]->set_label("Computer");
    else if (rage_button[i]->get_label() == "Computer")
        rage_button[i]->set_label("Human");
    else
        controller_->play(-1);
}

void View::handCardButtonClicked(int i)
{
    controller_->play(i);
}

void View::update() {
    //update cards in hand
    deque<pair<int, int> > hand = model_->getCurrentPlayerHand();
    for (int i = 0; i<hand.size(); i++)
    {
        cardInHand[i]->set(deck.getCardImage(hand[i].first, hand[i].second));
    }

    for (int i = hand.size(); i<13; i++)
    {
        cardInHand[i]->set(deck.getNullCardImage());
    }
    
    //update cards on table
    deque<deque<int> > cardsOnTable = model_->getCardsOnTable();
    
    for (int i = 0; i < 52; i++)
    {
        card[i]->set( deck.getNullCardImage() );//129
    }
    for (int i = 0; i < cardsOnTable[0].size(); i++ ) {
		card[cardsOnTable[0][i]]->set( deck.getCardImage(cardsOnTable[0][i], 0 ));
	}
    for (int i = 0; i < cardsOnTable[1].size(); i++ ) {
		card[cardsOnTable[1][i]+13]->set( deck.getCardImage(cardsOnTable[1][i], 1 ));
	}
    for (int i = 0; i < cardsOnTable[2].size(); i++ ) {
		card[cardsOnTable[2][i]+26]->set( deck.getCardImage(cardsOnTable[2][i], 2 ));
	}
    for (int i = 0; i < cardsOnTable[3].size(); i++ ) {
		card[cardsOnTable[3][i]+39]->set( deck.getCardImage(cardsOnTable[3][i], 3 ));
	}
    
    for (int i = 0; i<4; i++)
    {
        if (i == model_->getCurPlayer() - 1)
            rage_button[i]->set_sensitive(true);
        else
            rage_button[i]->set_sensitive(false);
        
    }
    
    for(int i = 0; i<4; i++)
    {
        stringstream ss;
        ss << model_->getTotalScore(i) << " points";
        stringstream ss2;
        ss2 << model_->getDiscardSize(i) << " discards";
        points[i]->set_label(ss.str());
        discards[i]->set_label(ss2.str());
    }
    
    //create dialogue for round scores
    if (model_->getRoundScores() != "")
    {
        Gtk::MessageDialog scoreDialog(*this, model_->getRoundScores(), false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK);
        scoreDialog.run();
    }
    
    if (model_->getWinners().size() != 0)
    {
        for (int i = 0; i < model_->getWinners().size(); i++)
        {
            stringstream ss;
            ss << "Player " << model_->getWinners()[i] << " wins!";
            Gtk::MessageDialog winnerDialog(*this, ss.str(), false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK);
            winnerDialog.run();
        }
        
        controller_->quitButtonClicked();
        for (int i = 0; i < 4; i++)
        {
            rage_button[i]->set_label("Human");
            rage_button[i]->set_sensitive(true);
            
        }
        quit_button.set_sensitive(false);
        new_button.set_sensitive(true);
        for (int i = 0; i<13; i++)
            handCard[i]->set_sensitive(false);
        for(int i = 0; i<4; i++)
        {
            points[i]->set_label("0 points");
            discards[i]->set_label("0 discards");
        }
    }
}

void View::newgameButtonClicked() {
    deque<string> playerTypes;
    for (int i = 0; i < 4 ; i++)
    {
        if (rage_button[i]->get_label() == "Rage!")
        {
            if(rage_button[i]->get_sensitive())
                playerTypes.push_back("Human");
            else
                playerTypes.push_back("Computer");
        }
        else
            playerTypes.push_back(rage_button[i]->get_label());
        
        if(rage_button[i]->get_label() == "Computer")
            rage_button[i]->set_sensitive(false);
        rage_button[i]->set_label("Rage!");
    }
    for (int i = 0; i<13; i++)
        handCard[i]->set_sensitive(true);
    
    controller_->newgameButtonClicked(seed_bar.get_text(), playerTypes);
    quit_button.set_sensitive(true);
    new_button.set_sensitive(false);
}

void View::quitButtonClicked() {
    controller_->quitButtonClicked();
    for (int i = 0; i < 4; i++)
    {
        rage_button[i]->set_label("Human");
        rage_button[i]->set_sensitive(true);
        
    }
    quit_button.set_sensitive(false);
    new_button.set_sensitive(true);
    for (int i = 0; i<13; i++)
        handCard[i]->set_sensitive(false);
    for(int i = 0; i<4; i++)
    {
        points[i]->set_label("0 points");
        discards[i]->set_label("0 discards");
    }
}