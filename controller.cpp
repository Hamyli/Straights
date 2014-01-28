#include "controller.h"
#include "model.h"


Controller::Controller(Model *m) : model_(m) {}

//handler for when new game button is clicked
void Controller::newgameButtonClicked(std::string s, std::deque<std::string> playerTypes) {

     model_->seed(s, playerTypes);
}

//handler for when quit button is clicked
void Controller::quitButtonClicked()
{
    model_->quit();
}

//handler for when a play occurs
void Controller::play(int i)
{
    model_->play(i);
}
