#pragma once

#include "model.hxx"
#include "view.hxx"

// The main game class for Blackjack.
class Controller : public ge211::Abstract_game
{
public:

    Controller();

protected:

    void on_mouse_up(ge211::Mouse_button, ge211::Posn<int>) override;

    void on_key(ge211::Key) override;

    void draw(ge211::Sprite_set&) override;
    View::Dimensions initial_window_dimensions() const override;
    std::string initial_window_title() const override;

private:
    Model            model_;
    View             view_;
    std::string      bet_input;
    ge211::Posn<int> cursor_;
    int              index_for_dealer;

};
