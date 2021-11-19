#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const card_x = 65;
static int const buffer = 5;
static int const button_width = 100;
static int const button_height = 40;
static int const hit_x = 190;
static int const stay_x = 320;
static int const button_y = 450;
static Color const grey = {128, 128, 128};
static Color const red = {150, 0, 0};

View::View(Model const& model)
        : dealer_is_dealing(false),
          model_(model),
          hit_button({button_width, button_height}, grey),
          stay_button({button_width, button_height}, grey),
          background({initial_window_dimensions().width,
                     initial_window_dimensions().height},
                     Color(0, 100, 0)),
          bet("Enter New Bet ('c' to clear): ", sans_25),
          hit("Hit", sans_25),
          stay("Stay", sans_25),
          dealer("Dealer's cards", sans_25),
          player("Player's cards", sans_25),
          money("Money: 100", sans_25),
          result("Your Turn", sans_25),
          timer()
{
    for (int i = 2; i <= 10; i++) {
        all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                                 (std::to_string(i) + "C", ge211::Image_sprite
                                         (std::to_string(i) +
                                          "_of_clubs.png")));
        all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                                 (std::to_string(i) + "H", ge211::Image_sprite
                                         (std::to_string(i) +
                                          "_of_hearts.png")));
        all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                                 (std::to_string(i) + "D", ge211::Image_sprite
                                         (std::to_string(i) +
                                          "_of_diamonds.png")));
        all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                                 (std::to_string(i) + "S", ge211::Image_sprite
                                         (std::to_string(i) +
                                          "_of_spades.png")));
    }

    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("JC", ge211::Image_sprite
                                     ("jack_of_clubs.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("JH", ge211::Image_sprite
                                     ("jack_of_hearts.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("JS", ge211::Image_sprite
                                     ("jack_of_spades.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("JD", ge211::Image_sprite
                                     ("jack_of_diamonds.png")));

    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("QC", ge211::Image_sprite
                                     ("queen_of_clubs.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("QH", ge211::Image_sprite
                                     ("queen_of_hearts.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("QD", ge211::Image_sprite
                                     ("queen_of_diamonds.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("QS", ge211::Image_sprite
                                     ("queen_of_spades.png")));

    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("KC", ge211::Image_sprite
                                     ("king_of_clubs.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("KH", ge211::Image_sprite
                                     ("king_of_hearts.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("KD", ge211::Image_sprite
                                     ("king_of_diamonds.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("KS", ge211::Image_sprite
                                     ("king_of_spades.png")));

    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("AC", ge211::Image_sprite
                                     ("ace_of_clubs.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("AH", ge211::Image_sprite
                                     ("ace_of_hearts.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("AD", ge211::Image_sprite
                                     ("ace_of_diamonds.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>
                             ("AS", ge211::Image_sprite
                                     ("ace_of_spades.png")));
    all_cards.insert(std::pair<std::string, ge211::Image_sprite>("face_down",
            ge211::Image_sprite
    ("face_down.png")));
}

void View::draw(Sprite_set& set, ge211::Posn<int> cursor, int& i)
{
    set.add_sprite(background, {0, 0}, -1);
    ge211::Text_sprite::Builder builder(sans_25);

    if(model_.get_game_state() == Game_state::playing &&
        !dealer_is_dealing) {

        update_money(model_.get_player().get_money());
        set.add_sprite(hit_button, {hit_x, button_y});
        set.add_sprite(stay_button, {stay_x, button_y});

        builder << "Hit";
        hit.reconfigure(builder);
        set.add_sprite(hit, {hit_x + 30, button_y - 5}, 1);

        builder.message("Stay");
        stay.reconfigure(builder);
        set.add_sprite(stay, {stay_x + 25, button_y - 5}, 1);

        builder.message("Your Turn");
        builder.font(sans_30);
        result.reconfigure(builder);
        set.add_sprite(result, {235, 30});

        set.add_sprite(all_cards.find("face_down")->second,
                       {50, 150}, 1);
        set.add_sprite(all_cards.find("face_down")->second,
                       {50 + card_x + buffer, 150}, 1); //face_down cards
    }
    else if (!dealer_is_dealing) {
        if(model_.get_game_state() == Game_state::bust) {
            builder.color(red);
            builder.message("You Busted");
        }
        else if(model_.get_game_state() == Game_state::lose) {
            builder.color(red);
            builder.message("You Lost");
        }
        else if(model_.get_game_state() == Game_state::win) {
            builder.color({0, 0, 200});
            builder.message("You Won");
        }
        else if(model_.get_game_state() == Game_state::tie) {
            builder.color(Color::white());
            builder.message("You Tied");
        }
        else if(model_.get_game_state() == Game_state::start){
            builder.color(Color::white());
            builder.message("Blackjack");
        }
        else {
            builder.color(red);
            builder.message("Game Over");
        }

        builder.font(sans_30);
        result.reconfigure(builder);
        set.add_sprite(result, {235, 30});
        update_money(model_.get_player().get_money());

        if(model_.get_game_state() != Game_state::over) {
            set.add_sprite(bet, {150, 450});
        }
        else {
            builder.font(sans_25);
            builder.color(Color::white());
            builder.message("Press any key to exit");
            bet.reconfigure(builder);

            set.add_sprite(bet, {170, 450});
        }
    }

    if(model_.get_game_state() != Game_state::start) {
        set.add_sprite(player, {20, 260});
        set.add_sprite(dealer, {20, 100});
    }
    set.add_sprite(money, {240, 510});
    add_hand_up_to(set, model_.get_player(), 310,  model_.get_player()
    .get_hand().size());

    if(!dealer_is_dealing) {
        add_hand_up_to(set, model_.get_dealer(), 150, model_.get_dealer()
                               .get_hand().size());
    }
    else { //animation for dealer's turn
        builder.color(Color::white());
        builder.message("Dealer's Turn");
        builder.font(sans_30);
        result.reconfigure(builder);
        set.add_sprite(result, {230, 30});

        add_hand_up_to(set, model_.get_dealer(), 150, i);
        if(i == model_.get_dealer().get_hand().size()) {
            dealer_is_dealing = false;
        }

        timer.reset();
        if(i > 2) {
            while (timer.elapsed_time() < (ge211::time::Duration) 1) {}
        }
        i++;
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    Dimensions starting_dim = Dimensions{600, 600};
    return starting_dim;
}

std::string
View::initial_window_title() const
{
    return "Blackjack";
}

void View::add_hand_up_to(ge211::Sprite_set& sprites, Player player_, int y,
                          int index)
{
    int x = 50;
    for(int i = 0; i < index; i++) {
        Card card = player_.get_hand().at(i);
        sprites.add_sprite(all_cards.find(
                card.get_name() + card.get_suite())->second, {x, y});

        x += card_x + buffer;
    }
}

bool View::click_button(ge211::Posn<int> cursor, int x, int y)
{
    if(cursor.x < x + button_width
        && cursor.x > x - button_width
        && cursor.y < y + button_height
        && cursor.y > y - button_height) {
        return true;
    }
    return false;
}

bool View::click_hit_button(ge211::Posn<int> cursor)
{
    return click_button(cursor, hit_x, button_y);
}

bool View::click_stay_button(ge211::Posn<int> cursor)
{
    return click_button(cursor, stay_x, button_y);
}

void View::update_bet(std::string bet_)
{
    bet = ge211::Text_sprite("Enter New Bet ('c' to clear): " + bet_, sans_25);
}

void View::update_money(int money_)
{
    std::string money__ = "Money: " + std::to_string(money_);
    money = ge211::Text_sprite(money__, sans_25);
}


