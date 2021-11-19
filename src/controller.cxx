#include "controller.hxx"


Controller::Controller()
        : model_(),
          view_(model_),
          bet_input(""),
          cursor_{0, 0},
          index_for_dealer(1)
{}

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int> pos)
{
    if(model_.get_game_state() == Game_state::playing
       && view_.click_hit_button(pos)) {
        model_.player_move("hit", NAN);
    }
    else if(model_.get_game_state() == Game_state::playing
            && view_.click_stay_button (pos)) {
        model_.player_move("stay", NAN);
        view_.dealer_is_dealing = true;
        index_for_dealer = 1;
    }
    else if(model_.get_game_state() == Game_state::over) {
        quit();
    }
}

void Controller::on_key(ge211::Key key)
{
    if(model_.get_game_state() == Game_state::over) {
        quit();
    }
    else if(model_.get_game_state() != Game_state::playing &&
        !view_.dealer_is_dealing) {
        if (key == ge211::Key::code('\r')) {
            try {
                model_.player_move("bet", std::stoi(bet_input));
                bet_input.clear();
            }
            catch (std::invalid_argument e) {
                bet_input.clear();
            }
            if (model_.get_game_state() == Game_state::over) {
                quit();
            }
        }
        else if(key == ge211::Key::code('c')) {
            bet_input.clear();
        }
        else {
            bet_input.append(key.as_text());
        }
        view_.update_bet(bet_input);
    }
}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, cursor_, index_for_dealer);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

