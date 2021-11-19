#pragma once

#include "model.hxx"
#include <map>
#include <string>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const&);

    void draw(ge211::Sprite_set& set, ge211::Posn<int> cursor, int& i);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    bool click_hit_button(ge211::Posn<int> cursor);

    bool click_stay_button(ge211::Posn<int> cursor);

    void update_bet(std::string bet);

    void update_money(int money);

    bool dealer_is_dealing;

private:

    Model const& model_;

    std::map<std::string, ge211::Image_sprite> all_cards;
    ge211::Rectangle_sprite hit_button, stay_button, background;
    ge211::Font sans_25{"sans.ttf", 25};
    ge211::Font sans_30{"sans.ttf", 30};
    ge211::Text_sprite bet, hit, stay, dealer, player, money, result;
    ge211::Timer timer;


    void add_hand_up_to(ge211::Sprite_set& sprites, Player player_, int y, int
    i);

    bool click_button(ge211::Posn<int> cursor, int x, int y);

};