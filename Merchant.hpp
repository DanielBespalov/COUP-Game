#ifndef COUP_MERCHANT_HPP
#define COUP_MERCHANT_HPP

#include "Player.hpp"
#include "RoleType.hpp"
#include "Game.hpp"


namespace coup {

class Merchant : public Player {
public:
    Merchant(Game &g, const std::string &name)
      : Player(g,name,RoleType::Merchant) {}
    // Passive start-of-turn bonus handled in Game::advance_turn()
};

}

#endif // COUP_MERCHANT_HPP
