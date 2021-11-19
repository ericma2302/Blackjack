#pragma once
#include <vector>
#include <card.hxx>

class Player {

public:

    Player(double money);

    double get_money() const
    { return money; }

    std::vector<Card> get_hand() const
    { return hand; }

    void add_money(double bet)
    { money += bet; }

    void add_to_hand(Card card) {
        hand.push_back(card);
    }

    void clear_hand() {
        hand.clear();
    }

    int sum_hand();

private:
    double money;
    std::vector<Card> hand;
};