#include <player.hxx>

Player::Player(double money)
        : money(money)
{}

int Player::sum_hand()
{
    int sum = 0;
    bool hasAce = false;
    for(Card card : hand) {
        if(card.get_name() == "A") {
            if(hasAce) {
                sum++;
            }
            else {
                sum += 11;
                hasAce = true;
            }
        }
        else {
            sum += card.get_value();
        }
    }

    if(hasAce && sum > 21) {
        sum -= 10;
    }

    return sum;
}