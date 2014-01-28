#ifndef MVC_CONTROLLER_H
#define MVC_CONTROLLER_H


#include <gtkmm.h>
#include <deque>

class Model;

class Controller {
public:
    Controller( Model* );
    void newgameButtonClicked(std::string, std::deque<std::string>);
    void quitButtonClicked();
    void play(int);
private:
    Model *model_;
}; // Controller


#endif

