#include "model.hxx"
#include <catch.hxx>

struct Test_access{

    Model& m;
    Deck& d;

    Test_access(Model& m)
            :m(m),
             d(m.deck_)
    { }

    int& bet() {
        return m.bet_;
    }

    ge211::Random_source<int>& random_card() {
        return d.random_card;
    }

    Player& player() {
        return m.player_;
    }

    Player& dealer() {
        return m.dealer_;
    }
};


TEST_CASE("Player Betting")
{
    Model m;
    Test_access t{m};

    //things that should throw
    CHECK_THROWS_AS(m.player_move("bet",200),std::invalid_argument);
    CHECK_THROWS_AS(m.player_move("bet",101),std::invalid_argument);
    CHECK_THROWS_AS(m.player_move("bet",-5),std::invalid_argument);


    // edge case
    CHECK_NOTHROW(m.player_move("bet",100));

    CHECK(t.bet() == 100);

    CHECK(m.get_game_state() == Game_state::playing);

    //game over if bets 0

    m.player_move("bet",0);

    CHECK(m.get_game_state() == Game_state::over);
}

TEST_CASE("DEALER STAYS AND BEATS PLAYER") {
    Model m;
    Test_access t{m};

    //Dealer at 20
    t.dealer().add_to_hand(Card(10,"Queen","S"));
    t.dealer().add_to_hand(Card(10,"K","S"));

    //Player is at 16
    t.player().add_to_hand(Card(10,"K","C"));
    t.player().add_to_hand(Card(6,"6","C"));

    //Dealer turn should just make the player lose his bet if the player
    // stays now

    t.bet() = 50;
    t.player().add_money(-t.bet());

    m.player_move("stay",t.bet());

    // dealer didn't draw any more cards
    CHECK(t.dealer().get_hand().size() == 2);
    //player lost $50
    CHECK(t.player().get_money() == 50);
}

TEST_CASE("DEALER STAYS AND BEATS PLAYER - GAME OVER") {
    Model m;
    Test_access t{m};

    //Dealer at 20
    t.dealer().add_to_hand(Card(10,"Queen","S"));
    t.dealer().add_to_hand(Card(10,"K","S"));

    //Player is at 16
    t.player().add_to_hand(Card(10,"K","C"));
    t.player().add_to_hand(Card(6,"6","C"));

    //Dealer turn should just make the player lose his bet if the player
    // stays now

    t.bet() = 100;
    t.player().add_money(-t.bet());

    m.player_move("stay",t.bet());

    // dealer didn't draw any more cards
    CHECK(t.dealer().get_hand().size() == 2);
    //player lost $50
    CHECK(t.player().get_money() == 0);
    CHECK(m.get_game_state() == Game_state::over);
}


TEST_CASE("DEALER STAYS AND PUSH") {
    Model m;
    Test_access t{m};

    //Dealer at 20
    t.dealer().add_to_hand(Card(10,"Queen","S"));
    t.dealer().add_to_hand(Card(7,"7","S"));

    //Player is at 16
    t.player().add_to_hand(Card(10,"K","C"));
    t.player().add_to_hand(Card(7,"7","C"));

    //Dealer turn should just make the player lose his bet if the player
    // stays now

    t.bet() = 50;

    m.player_move("stay",t.bet());
    t.player().add_money(-t.bet());

    // dealer didn't draw any more cards
    CHECK(t.dealer().get_hand().size() == 2);
    //player stays at $100
    CHECK(t.player().get_money() == 100);
}


TEST_CASE("DEALER STAYS AND LOSES") {
    Model m;
    Test_access t{m};

    //Dealer at 20
    t.dealer().add_to_hand(Card(10,"Queen","S"));
    t.dealer().add_to_hand(Card(10,"K","S"));

    //Player is at 16
    t.player().add_to_hand(Card(10,"K","C"));
    t.player().add_to_hand(Card(5,"5","C"));
    t.player().add_to_hand(Card(6,"6","C"));


    //Dealer turn should just make the player lose his bet if the player
    // stays now

    t.bet() = 50;
    t.player().add_money(-t.bet());

    m.player_move("stay",t.bet());

    // dealer didn't draw any more cards
    CHECK(t.dealer().get_hand().size() == 2);
    //player lost $50
    CHECK(t.player().get_money() == 150);
}

TEST_CASE("BLACKJACK") {
    Model m;
    Test_access t{m};

    //deal the player a K and an ace
    t.random_card().stub_with({1, 51, 10, 15});

    m.player_move("bet",50);

    //Player should make $75 instantly
    CHECK(t.player().get_money() == 175);
}

TEST_CASE("DEALER BUSTS") {
    Model m;
    Test_access t{m};

    //Dealer at 5
    t.dealer().add_to_hand(Card(3,"3","S"));
    t.dealer().add_to_hand( Card(2, "2", "S"));

    //Player stays at 15
    t.player().add_to_hand(Card(10,"K","C"));
    t.player().add_to_hand(Card(5,"5","C"));


    //Dealer is dealt two Ks when the player stays
    t.random_card().stub_with({50, 51});

    t.bet() = 50;
    t.player().add_money(-t.bet());

    m.player_move("stay",t.bet());

    // dealer drew 2 more cards before busting
    CHECK(t.dealer().get_hand().size() == 4);
    //player won $50
    CHECK(t.player().get_money() == 150);
}

TEST_CASE("PLAYER BUSTS") {
    Model m;
    Test_access t{m};

    //Dealer at 10
    t.dealer().add_to_hand(Card(5,"5","S"));

    //Player is at 15
    t.player().add_to_hand(Card(10,"K","C"));
    t.player().add_to_hand(Card(5,"5","C"));


    //player is dealt a K
    t.random_card().stub_with({50});

    t.bet() = 50;
    t.player().add_money(-t.bet());

    m.player_move("hit",t.bet());

    //player lost $50
    CHECK(t.player().get_money() == 50);
}

TEST_CASE("PLAYER BUSTS AFTER 1 CARD W ALL MONEY") {
    Model m;
    Test_access t{m};

    //Dealer at 10
    t.dealer().add_to_hand(Card(10,"K","S"));

    //Player is at 15
    t.player().add_to_hand(Card(10,"K","C"));
    t.player().add_to_hand(Card(5,"5","C"));


    //player is dealt a K
    t.random_card().stub_with({50});

    t.bet() = 100;
    t.player().add_money(-t.bet());

    m.player_move("hit",t.bet());

    //player lost all money
    CHECK(t.player().get_money() == 0);
    CHECK(m.get_game_state() == Game_state::over);
}

TEST_CASE("PLAYER BUSTS AFTER 2 CARDS W ALL MONEY") {
    Model m;
    Test_access t{m};

    //Dealer at 10
    t.dealer().add_to_hand(Card(5,"5","S"));

    //Player is at 5
    t.player().add_to_hand(Card(3,"3","C"));
    t.player().add_to_hand(Card(2,"2","C"));


    //player is dealt a K
    t.random_card().stub_with({50,51});

    t.bet() = 100;
    t.player().add_money(-t.bet());

    m.player_move("hit",t.bet());

    CHECK_FALSE(m.get_game_state() == Game_state::over);

    m.player_move("hit",t.bet());
    //player lost all money
    CHECK(t.player().get_money() == 0);
    CHECK(m.get_game_state() == Game_state::over);
}

TEST_CASE("PLAYER HITS TO 21 - DEALER STAYS AT 17") {
    Model m;
    Test_access t{m};


    //Dealer at 10
    t.dealer().add_to_hand(Card(5,"5","S"));
    t.dealer().add_to_hand(Card(5,"5","H"));


    //Player is at 11
    t.player().add_to_hand(Card(6,"6","C"));
    t.player().add_to_hand(Card(5,"5","C"));


    //player is dealt a K, dealer given a 7
    t.random_card().stub_with({50,24});

    t.bet() = 50;
    t.player().add_money(-t.bet());

    m.player_move("hit", t.bet());

    CHECK(t.player().sum_hand() == 21);

    m.player_move("stay", t.bet());

    CHECK(t.dealer().sum_hand() == 17);

    CHECK(t.player().get_money() == 150);
}
