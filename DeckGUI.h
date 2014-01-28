#ifndef __DECK_GUI_H
#define __DECK_GUI_H
#include <gdkmm/pixbuf.h>
#include <vector>

using std::vector;


class DeckGUI {
public:
	DeckGUI();
	virtual ~DeckGUI();
	Glib::RefPtr<Gdk::Pixbuf> getCardImage( int r, int s );   // Returns the image for the specified card.
	Glib::RefPtr<Gdk::Pixbuf> getNullCardImage();                 // Returns the image to use for the placeholder.

private:
	vector< Glib::RefPtr< Gdk::Pixbuf > > deck;                   // Contains the pixel buffer images.
}; // DeckGUI
#endif
