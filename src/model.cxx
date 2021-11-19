#include "model.hxx"

Model::Model()
        :deck_(),
         player_(100),
         dealer_(NAN),
         bet_(10),
         game(Game_state::start)
{ }

void Model::deal_cards()
{
    player_.add_to_hand(deck_.deal_card());
    player_.add_to_hand(deck_.deal_card());

    dealer_.add_to_hand(deck_.deal_card());
    dealer_.add_to_hand(deck_.deal_card());
}

void Model::player_move(std::string move, int bet)
{
    if(game == Game_state::over) {
        return;
    }

    if (move == "bet") {

        player_.clear_hand();
        dealer_.clear_hand();
        // we'll use betting $0 as the user quitting the game
        if (bet == 0) {
            game = Game_state::over;
            return;
        }

        else if (bet <= player_.get_money() && bet > 0) {
            player_.add_money(-1 * bet);
            bet_ = bet;

            deck_.set_all_active();
            deal_cards();
            game = Game_state::playing;

            if(player_.sum_hand() == 21) {
                game = Game_state::win;
                pay_bet(2.5);
            }
        }
        else {
            throw std::invalid_argument("Model::Player move, Invalid "
                                            "input");
        }
    }
    else if (move == "stay") {
        dealer_turn();
    }
    else if (move == "hit") {
        Card last_card = deck_.deal_card();
        player_.add_to_hand(last_card);

        if (player_.sum_hand() > 21) {
            game = Game_state::bust;
            pay_bet(0);

        }
    }
}

void Model::pay_bet(double n)
{
    player_.add_money(bet_ * n);

    // this is also an end condition
    if ((int) player_.get_money() <= 0) {
        game = Game_state::over;
    }
}

void Model::dealer_turn()
{
    while(dealer_.sum_hand() < 17) {
        // dealer waits one second between hitting.
        dealer_.add_to_hand(deck_.deal_card());
    }

    if(dealer_.sum_hand() > 21) {
        game = Game_state::win;
        pay_bet(2);

    }
    else if (dealer_.sum_hand() > player_.sum_hand()) {
        game = Game_state::lose;
        pay_bet(0);
    }
    else if (dealer_.sum_hand() < player_.sum_hand()) {

        game = Game_state::win;
        pay_bet(2);
    }
    else {
        game = Game_state::tie;
        pay_bet(1);
    }
}
