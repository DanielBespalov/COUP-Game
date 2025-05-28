#include <string>
#include "Player.hpp"
#include "Game.hpp"


namespace coup {

Player::Player(Game &game, const std::string &name, RoleType role)
    : _game(game), _name(name), _role(role), coin_count(0), active(true),
      blockedArrest(false), blockedGatherOrTax(false),
      blockedTax(false), blockedBribe(false) {
    _game.registerPlayer(this);
}

const std::string& Player::name() const {
    return _name;
}

RoleType Player::role() const {
    return _role;
}

int Player::coins() const {
    return coin_count;
}

bool Player::isActive() const {
    return active;
}

void Player::gather() {
    _game.gather(*this);
}

void Player::tax() {
    _game.tax(*this);
}

void Player::bribe() {
    _game.bribe(*this);
}

void Player::arrest(Player &p) {
    _game.arrest(*this, p);
}

void Player::sanction(Player &p) {
    _game.sanction(*this, p);
}

void Player::coup(Player &p) {
    _game.coup(*this, p);
}

} // namespace coup
