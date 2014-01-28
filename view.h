#ifndef VIEW_H
#define VIEW_H

#include <gtkmm.h>
#include <deque>
#include "DeckGUI.h"
#include "observer.h"
#include "controller.h"
#include "model.h"

class View : public Gtk::Window, public Observer{
public:
    View(Controller*, Model*);
	virtual ~View();
    virtual void update();

private:

    //reference to model for observer pattern
    Model *model_;
    Controller *controller_;

    // Card Images
	DeckGUI deck;

	// Member widgets:
    Gtk::VBox mainBox;

    //start game and end game panel with associated buttons
    Gtk::Button new_button;
    Gtk::Button quit_button;
    Gtk::Entry seed_bar;
    Gtk::HBox seedPanel;

    //displays the cards on table
    Gtk::Image *card[52];
    Gtk::VBox table_box;
    Gtk::HBox club_box;
    Gtk::HBox diamond_box;
    Gtk::HBox heart_box;
    Gtk::HBox spade_box;
	Gtk::Frame table_frame;      // Main window divided into two horizontal panels

    //player display panel
    Gtk::Button *rage_button[4];
    Gtk::Label *points[4];
    Gtk::Label *discards[4];
    Gtk::VBox *players[4];
    Gtk::Frame *player_frame[4];
    Gtk::HBox players_box;

    //hand card display panel
    Gtk::Frame hand;
    Gtk::HBox handCards;
    Gtk::Button *handCard[13];
    Gtk::Image *cardInHand[13];

	// Signal handlers:
	void newgameButtonClicked();
	void quitButtonClicked();
    void rageButtonClicked(int);
    void handCardButtonClicked(int);

}; // View

#endif
