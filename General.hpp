#ifndef COUP_GENERAL_HPP
#define COUP_GENERAL_HPP

#include "Player.hpp"
#include "RoleType.hpp"
#include "Game.hpp"

/**
 * danieldaniel2468@gmail.com
 */
namespace coup {

class General : public Player {
public:
    General(Game &g, const std::string &name)
      : Player(g,name,RoleType::General) {}

    // Protect from next coup
    void protect(Player &target) {
        _game.generalProtect(*this, target);
    }
};

}

#endif // COUP_GENERAL_HPP
