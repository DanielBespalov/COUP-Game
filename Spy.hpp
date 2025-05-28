#ifndef COUP_SPY_HPP
#define COUP_SPY_HPP

#include "Player.hpp"
#include "Game.hpp"

/**
 * danieldaniel2468@gmail.com
 */
namespace coup {

class Spy : public Player {
public:
    Spy(Game &g, const std::string &name)
      : Player(g,name,RoleType::Spy) {}

    // View coins + block next arrest
    int viewCoins(Player &target) {
        return _game.spyView(*this, target);
    }
};

}

#endif // COUP_SPY_HPP
