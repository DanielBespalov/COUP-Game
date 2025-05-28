#ifndef COUP_PLAYER_HPP
#define COUP_PLAYER_HPP

#include <string>
#include "RoleType.hpp"
/**
 * danieldaniel2468@gmail.com
 */
namespace coup {

class Game; // Forward declaration 

class Player {
    friend class Game; 
public:
    Player(Game& game, const std::string &name, RoleType role);

    const std::string& name() const;
    RoleType role() const;
    int coins() const;
    bool isActive() const;

    void gather();
    void tax();
    void bribe();
    void arrest(Player &p);
    void sanction(Player &p);
    void coup(Player &p);
    bool isBlockedGatherOrTax() const { return blockedGatherOrTax; }
    bool isBlockedBribe() const { return blockedBribe; }
    bool isBlockedArrest() const { return blockedArrest; }

protected:
    Game& _game; 
    std::string _name;
    RoleType _role;
    int coin_count;
    bool active;

    bool blockedArrest;
    bool blockedGatherOrTax;
    bool blockedTax;
    bool blockedBribe;
    std::string lastArrestTarget;
};

} // namespace coup

#endif // COUP_PLAYER_HPP
