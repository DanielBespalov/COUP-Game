#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace coup;

TEST_CASE("Basic Gathering Round") {
    Game g;
    Governor moshe(g, "Moshe");
    Spy yossi(g, "Yossi");
    Baron meirav(g, "Meirav");
    General reut(g, "Reut");
    Judge gilad(g, "Gilad");

    moshe.gather();
    yossi.gather();
    meirav.gather();
    reut.gather();
    gilad.gather();

    CHECK(moshe.coins() == 1);
    CHECK(yossi.coins() == 1);
    CHECK(meirav.coins() == 1);
    CHECK(reut.coins() == 1);
    CHECK(gilad.coins() == 1);
}

TEST_CASE("Spy attempts invalid turn") {
    Game g;
    Governor moshe(g, "Moshe");
    Spy yossi(g, "Yossi");

    CHECK_THROWS(yossi.gather());
}

TEST_CASE("Governor tax bonus and undo") {
    Game g;
    Governor moshe(g, "Moshe");
    Spy yossi(g, "Yossi");
    Baron meirav(g, "Meirav");
    General reut(g, "Reut");
    Judge gilad(g, "Gilad");

    moshe.gather();
    yossi.gather();
    meirav.gather();
    reut.gather();
    gilad.gather();

    moshe.tax();
    yossi.tax();
    CHECK_THROWS(gilad.undo(moshe));
    moshe.undo(yossi);

    CHECK(yossi.coins() == 1);
}

TEST_CASE("Baron invests successfully") {
    Game g;
    Governor moshe(g, "Moshe");
    Spy yossi(g, "Yossi");
    Baron meirav(g, "Meirav");
    General reut(g, "Reut");
    Judge gilad(g, "Gilad");

    moshe.gather();
    yossi.gather();
    meirav.gather();
    reut.gather();
    gilad.gather();
    
    moshe.gather();
    yossi.gather();
    meirav.gather();
    reut.gather();
    gilad.gather();

    moshe.gather();
    yossi.gather();
    meirav.gather();
    reut.gather();
    gilad.gather();

    moshe.gather();
    yossi.gather();
    meirav.invest();

    CHECK(meirav.coins() == 6);
}

TEST_CASE("Baron performs coup") {
    Game g;
    Baron meirav(g, "Meirav");
    General reut(g, "Reut");
    Judge gilad(g, "Gilad");
    Governor moshe(g, "Moshe");
    Spy yossi(g, "Yossi");
    

    meirav.gather(); // Meirav: 1
    reut.gather();
    gilad.gather();
    moshe.gather();
    yossi.gather();

    meirav.gather(); // 2
    reut.gather();
    gilad.gather();
    moshe.gather();
    yossi.gather();

    meirav.tax(); // now: 5
    reut.gather();
    gilad.gather();
    moshe.gather();
    yossi.gather();

    meirav.tax(); // now: 8
    reut.gather();
    gilad.gather();
    moshe.gather();
    yossi.gather();

    meirav.tax(); // now: 8
    reut.gather();
    gilad.gather();
    moshe.gather();
    yossi.gather();

    meirav.coup(moshe);

    CHECK(moshe.isActive() == false);
}

TEST_CASE("Spy blocks arrest") {
    Game g;
    Governor moshe(g, "Moshe");
    Spy yossi(g, "Yossi");
    Baron meirav(g, "Meirav");

    moshe.gather();
    g.spyView(yossi, meirav);

    CHECK(meirav.isBlockedArrest());
}

TEST_CASE("Judge blocks bribe") {
    Game g;
    Spy yossi(g, "Yossi");
    Judge gilad(g, "Gilad");

    yossi.gather();
    gilad.gather();
    yossi.gather();
    gilad.gather();
    yossi.gather();
    gilad.gather();
    yossi.gather(); // now 4 coins
    gilad.gather();


    yossi.bribe();
    gilad.undo(yossi);
    CHECK(yossi.coins() == 0);
    CHECK(yossi.isBlockedBribe());
}

TEST_CASE("General protects against coup") {
    Game g;
    General reut(g, "Reut");
    Baron meirav(g, "Meirav");

    reut.gather(); 
    meirav.gather(); 

    reut.gather(); 
    meirav.gather(); 

    reut.gather(); 
    meirav.gather(); 

    reut.gather(); 
    meirav.gather(); 

    reut.gather();
    meirav.gather();
        
    reut.gather();
    meirav.gather(); 
         
    reut.gather();
    meirav.gather();

    reut.protect(reut);
    reut.gather();

    meirav.coup(reut);

    CHECK(reut.isActive());
    CHECK(meirav.coins() == 0);
}

TEST_CASE("Merchant passive bonus") {
    Game g;
    Merchant m(g, "Gilad");

    m.gather(); // 1
    m.gather(); // 2
    m.gather(); // 3
    m.gather(); // 4 -> triggers passive bonus if needed

    CHECK(m.coins() >= 4); // Should be 4 if bonus applied
}

TEST_CASE("Winner declared correctly") {
    Game g;
    Governor moshe(g, "Moshe");
    Baron meirav(g, "Meirav");

     for (int i = 0; i < 7; ++i) {
        moshe.gather();
        meirav.gather();
    }
    moshe.gather();
    meirav.coup(moshe);

    CHECK(g.players().size() == 1);
    CHECK(g.winner() == "Meirav");
}

TEST_CASE("Spy cannot spyView without being Spy") {
    Game g;
    Governor gov(g, "Moshe");
    Merchant merc(g, "Dan");

    gov.gather();
    merc.gather();

    CHECK_THROWS(g.spyView(gov, merc));
}

TEST_CASE("Judge cannot undo himself") {
    Game g;
    Judge judge(g, "Gilad");
    Spy spy(g, "Yossi");

    judge.gather();
    spy.gather();
    judge.gather();
    spy.gather();
    judge.gather();
    spy.gather();
    judge.gather();
    spy.gather();
    judge.gather();
    spy.bribe();
    CHECK_THROWS(judge.undo(judge));
}

TEST_CASE("Governor blocks tax successfully") {
    Game g;
    Governor gov(g, "Moshe");
    Spy spy(g, "Yossi");

    gov.gather();
    spy.gather();
    gov.gather();
    spy.gather();
    gov.gather();

    spy.tax(); 
    gov.undo(spy); 
    CHECK(spy.coins() == 2);
}

TEST_CASE("Sanction disables gather/tax for target") {
    Game g;
    Governor gov(g, "Moshe");
    Merchant merc(g, "Dan");

    gov.gather();
    merc.gather();
    gov.gather();
    merc.gather();
    gov.gather();
    merc.gather();

    gov.sanction(merc);
    CHECK(merc.isBlockedGatherOrTax());

    CHECK_THROWS(merc.gather());
    CHECK_THROWS(merc.tax());
}

TEST_CASE("Illegal coup on self throws") {
    Game g;
    General gen(g, "Reut");

    for (int i = 0; i < 7; ++i) {
        gen.gather();
    }

    CHECK_THROWS_WITH(gen.coup(gen), "Cannot coup self");
}

TEST_CASE("Cannot gather when sanctioned") {
    Game g;
    Governor gov(g, "Moshe");
    Merchant merc(g, "Dan");

    gov.gather();
    merc.gather();
    gov.gather();
    merc.gather();
    gov.gather();
    merc.gather();
    gov.sanction(merc);

    CHECK_THROWS(merc.gather());
}

TEST_CASE("Cannot tax when sanctioned") {
    Game g;
    Governor gov(g, "Moshe");
    Spy spy(g, "Yossi");

    gov.gather();
    spy.gather();
    gov.gather();
    spy.gather();
    gov.gather();
    spy.gather();

    gov.sanction(spy);

    CHECK_THROWS(spy.tax());
}

TEST_CASE("Cannot sanction self") {
    Game g;
    Governor gov(g, "Moshe");

    for (int i = 0; i < 3; ++i) {
        gov.gather();
    }

    CHECK_THROWS(gov.sanction(gov));
}

TEST_CASE("Spy arrest fails after being blocked by spyView") {
    Game g;
    Spy spy(g, "Yossi");
    Merchant merc(g, "Dan");

    spy.gather();
    merc.gather();
    g.spyView(spy,merc);
    spy.gather();

    for (int i = 0; i < 2; ++i) {
        merc.gather();
        spy.gather();
    }

    CHECK_THROWS(spy.arrest(merc));
}

TEST_CASE("Baron compensation after sanction by Judge") {
    Game g;
    Judge judge(g, "Gilad");
    Baron baron(g, "Baruch");

    judge.gather(); 
    baron.gather();
    judge.gather(); 
    baron.gather();
    judge.gather(); 
    baron.gather();
    judge.gather(); // total: 4
    baron.gather();

    judge.sanction(baron);

    CHECK(judge.coins() == 1);  // Paid 3 + 1
    CHECK(baron.coins() == 5);  // Compensation
}

TEST_CASE("Baron cannot invest with less than 3 coins") {
    Game g;
    Baron b(g, "Bar");

    b.gather(); 
    b.gather(); // only 2
    CHECK_THROWS(b.invest());
}

TEST_CASE("General cannot protect others with less than 5 coins") {
    Game g;
    General gen(g, "Reut");
    Spy spy(g, "Yossi");

    for (int i = 0; i < 4; ++i) {
        gen.gather(); spy.gather();
    }

    CHECK_THROWS(gen.protect(spy));
}

TEST_CASE("Merchant bonus triggers only once at turn start") {
    Game g;
    Merchant m(g, "Merc");

    m.gather(); m.gather(); m.gather();
    
    m.gather();  // bonus triggers here
    int after = m.coins();   

    m.gather();  // no bonus now
    int final = m.coins();   

    CHECK(after == 6);
    CHECK(final == 8);
}

