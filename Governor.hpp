#ifndef COUP_GOVERNOR_HPP
#define COUP_GOVERNOR_HPP

#include "Player.hpp"
#include "RoleType.hpp"
#include "Game.hpp"


namespace coup {

class Governor : public Player {
public:
    Governor(Game &g, const std::string &name)
      : Player(g,name,RoleType::Governor) {}

    // Undo someone’s tax (out-of-turn)
    void undo(Player &target) {
        _game.governorBlockTax(*this, target);
    }
};

}

#endif // COUP_GOVERNOR_HPP
