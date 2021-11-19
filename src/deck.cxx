
#include "deck.hxx"
#include "vector"
#include <iostream>


Deck::Deck()
    :random_card(52)
{
    all_suits.push_back("C");
    all_suits.push_back("S");
    all_suits.push_back("D");
    all_suits.push_back("H");

    for(int i = 1; i <= 11; i++) {
        for(int j = 0; j < 4; j++) {
            if (i == 1) {
                deck.push_back(Card(11, "A", all_suits[j]));
            }

            else if (i <= 10) {
                deck.push_back(Card(i, std::to_string(i), all_suits[j]));
            }

            else {
                deck.push_back(Card(10, "J", all_suits[j]));
                deck.push_back(Card(10, "Q", all_suits[j]));
                deck.push_back(Card(10, "K", all_suits[j]));
            }
        }
    }
}

Card Deck::deal_card()
{
    int random = random_card.next();
    while(!deck.at(random).card_active()) {
        random = random_card.next();
    }

    deck.at(random).deactivate_card();
    return deck.at(random);
}

void Deck::set_all_active()
{
    for(Card& card : deck) {
        card.activate_card();
    }
}


