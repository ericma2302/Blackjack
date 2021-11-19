#pragma once

#include <ge211.hxx>
#include <deck.hxx>
#include <card.hxx>
#include <player.hxx>
#include <game_state.hxx>
#include <iostream>

class Model {
public:

    Model();

    // can either hit, stay, or bet
    void player_move(std::string move, int bet);

    void dealer_turn();

    void pay_bet(double n);

    void deal_cards();

    Player get_player() const
    { return player_; }

    Player get_dealer() const
    { return dealer_; }

    Game_state get_game_state() const
    { return game; }

#ifdef CS211_TESTING
    friend struct Test_access;
#endif

private:

    Deck deck_;
    Player player_;
    Player dealer_;
    int bet_;
    Game_state game;

};