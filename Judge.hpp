#ifndef COUP_JUDGE_HPP
#define COUP_JUDGE_HPP

#include "Player.hpp"
#include "RoleType.hpp"
#include "Game.hpp"


namespace coup {

class Judge : public Player {
public:
    Judge(Game &game, const std::string &name) : Player(game, name, RoleType::Judge) {}
    // Judge can undo (block) a bribe action of another player
    void undo(Player &target) {
        // Judge can only undo bribe actions during the target's turn
        if (_game.turn() != target.name()) {
            throw ActionException("Cannot undo this action");
        }
        _game.judgeBlockBribe(*this, target);
    }
};

}

#endif // COUP_JUDGE_HPP
