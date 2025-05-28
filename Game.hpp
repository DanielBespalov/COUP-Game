#ifndef COUP_GAME_HPP
#define COUP_GAME_HPP

#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include "Player.hpp"

namespace coup {

class Player;  // forward

// Thrown on illegal game actions
class ActionException : public std::runtime_error {
public:
    ActionException(const std::string &msg) : std::runtime_error(msg) {}
};

enum class ActionType { None, Gather, Tax, Bribe, Arrest, Sanction, Coup };

class Game {
private:
    size_t turn_index;

    std::unordered_map<std::string,int> sanction_expiry;
    std::unordered_map<std::string,int> tax_block_expiry;
    std::unordered_map<std::string,bool> coup_protection;

    void advance_turn();

public:
    Game();
    void registerPlayer(Player *p);
    std::vector<Player*> players_list;
    std::vector<std::string> players() const;
    std::string turn() const;
    std::string winner() const;

    Player& getPlayer(const std::string& name);
    const Player& getPlayer(const std::string& name) const;

    void gather(Player &p);
    void tax(Player &p);
    void bribe(Player &p);
    void arrest(Player &p, Player &t);
    void sanction(Player &p, Player &t);
    void coup(Player &p, Player &t);
    void addPlayer(const std::string& name, const std::string& role);

    int spyView(Player &p, Player &t);
    void baronInvest(Player &p);
    void generalProtect(Player &p, Player &t);
    void judgeBlockBribe(Player &p, Player &t);
    void governorBlockTax(Player &p, Player &t);

    int centralCoins() const {
        return 50;
    }

    void nextTurn_qt() {
        advance_turn();
    }
    size_t getTurnIndex() const { return turn_index; }

};

} // namespace coup

#endif // COUP_GAME_HPP
