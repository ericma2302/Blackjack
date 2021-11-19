#pragma once

#include <card.hxx>
#include <iostream>
#include <vector>
#include <ge211.hxx>

class Deck {
public:
    Deck();

    Card deal_card();

    void set_all_active();

    std::vector<Card> deck;

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif

private:

    std::vector<std::string> all_suits;
    ge211::Random_source<int> random_card;
};