
#include "card.hxx"
#include <iostream>

Card::Card(int value, std::string name, std::string suite)
            : value(value),
                name(name),
              suite(suite),
              is_active(true)
{}

