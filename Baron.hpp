#ifndef COUP_BARON_HPP
#define COUP_BARON_HPP

#include "Player.hpp"
#include "Game.hpp"


namespace coup {

class Baron : public Player {
public:
    Baron(Game &g, const std::string &name)
      : Player(g,name,RoleType::Baron) {}

    // Invest 3 → get 6
    void invest() {
        _game.baronInvest(*this);
    }
};

}

#endif // COUP_BARON_HPP
