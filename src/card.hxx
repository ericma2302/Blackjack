#pragma once
#include <iostream>

class Card
{
public:
    Card(int value, std::string name, std::string suit);

    int get_value() const
    { return value; }

    std::string get_name() const
    { return name; }

    std::string get_suite() const
    { return suite; }

    bool card_active() const
    { return is_active; }

    void deactivate_card()
    { is_active = false; }

    void activate_card()
    { is_active = true; }

private:

    int value;
    std::string name, suite;
    bool is_active;

};