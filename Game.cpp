#include "Game.hpp"
#include "Player.hpp"
#include <algorithm>
#include "GameQtBridge.hpp"
#include "RoleType.hpp"
/**
 * danieldaniel2468@gmail.com
 */
using namespace coup;

// Helpers
static void must_be_turn(Player &p, Player *cur) {
    if (&p != cur) throw ActionException("Not your turn");
}

static void must_exist(Player &t) {
    if (!t.isActive()) throw ActionException("Target not active");
}

Game::Game() : turn_index(0) {}

void Game::registerPlayer(Player *p) {
    players_list.push_back(p);
    if (players_list.size() == 1) {
        turn_index = 0;  // רק בהתחלה
    }
}

void Game::addPlayer(const std::string& name, const std::string& role) {
    RoleType r;
    for (const auto* p : players_list) {
        if (p->name() == name) {
            throw std::invalid_argument("Player with this name already exists.");
        }
    }
    if (role == "Governor") r = RoleType::Governor;
    else if (role == "Spy") r = RoleType::Spy;
    else if (role == "Baron") r = RoleType::Baron;
    else if (role == "General") r = RoleType::General;
    else if (role == "Judge") r = RoleType::Judge;
    else if (role == "Merchant") r = RoleType::Merchant;
    else throw ActionException("Invalid role: " + role);

    auto* p = new Player(*this, name, r);
    registerPlayer(p);
}

std::vector<std::string> Game::players() const {
    std::vector<std::string> out;
    for (auto *p : players_list) {
        if (p->isActive()) out.push_back(p->name());
    }
    return out;
}

std::string Game::turn() const {
    if (players_list.empty())
        throw ActionException("No players in game");
    size_t idx = turn_index;
    if (!players_list[idx]->isActive()) {
        size_t n = players_list.size();
        do { idx = (idx + 1) % n; } while (!players_list[idx]->isActive());
    }
    return players_list[idx]->name();
}

std::string Game::winner() const {
    Player *last = nullptr;
    int count = 0;
    for (auto *p : players_list) {
        if (p->isActive()) {
            last = p;
            ++count;
        }
    }
    if (count == 1 && last) return last->name();
    throw ActionException("Game is not over");
}

void Game::advance_turn() {
    Player *cur = players_list[turn_index];
    auto sit = sanction_expiry.find(cur->name());
    if (sit != sanction_expiry.end() && sit->second == (int)turn_index) {
        cur->blockedGatherOrTax = false;
        sanction_expiry.erase(sit);
    }
    auto tit = tax_block_expiry.find(cur->name());
    if (tit != tax_block_expiry.end() && tit->second == (int)turn_index) {
        cur->blockedTax = false;
        tax_block_expiry.erase(tit);
    }
    cur->blockedArrest = false;
    cur->blockedBribe  = false;
    cur->lastArrestTarget.clear();

    size_t n = players_list.size();
    do {
        turn_index = (turn_index + 1) % n;
    } while (!players_list[turn_index]->isActive());

    Player *nxt = players_list[turn_index];
    if (nxt->role() == RoleType::Merchant && nxt->coins() >= 3) {
        nxt->coin_count += 1;
    }
}
Player& Game::getPlayer(const std::string& name) {
    for (Player* p : players_list) {
        if (p->name() == name) {
            return *p;
        }
    }
    throw std::runtime_error("Player " + name + " not found");
}

const Player& Game::getPlayer(const std::string& name) const {
    for (Player* p : players_list) {
        if (p->name() == name) {
            return *p;
        }
    }
    throw std::runtime_error("Player " + name + " not found");
}

void Game::gather(Player &p) {
    must_be_turn(p, players_list[turn_index]);
    if (p.coins() >= 10) throw ActionException("Must coup with 10 coins");
    if (p.blockedGatherOrTax) {
        advance_turn();
        throw ActionException("Gather blocked");
    }
    p.coin_count += 1;
    advance_turn();
}

void Game::tax(Player &p) {
    must_be_turn(p, players_list[turn_index]);
    if (p.coins() >= 10) throw ActionException("Must coup with 10 coins");
    if (p.blockedGatherOrTax) {
        advance_turn();
        throw ActionException("Tax blocked by sanction");
    }
    if (p.blockedTax) {
        p.blockedTax = false;
        advance_turn();
        throw ActionException("Tax blocked by Governor");
    }
    p.coin_count += (p.role() == RoleType::Governor ? 3 : 2);
    advance_turn();
}

void Game::bribe(Player &p) {
    must_be_turn(p, players_list[turn_index]);
    if (p.coins() < 4) throw ActionException("Not enough coins for bribe");
    if (p.coins() >= 10) throw ActionException("Must coup with 10 coins");
    p.coin_count -= 4;
    if (p.blockedBribe) {
        p.blockedBribe = false;
        advance_turn();
        throw ActionException("Bribe blocked by Judge");
    }
}

void Game::arrest(Player &p, Player &t) {
    must_be_turn(p, players_list[turn_index]);
    if (p.coins() >= 10) throw ActionException("Must coup with 10 coins");
    must_exist(t);
    if (&p == &t) throw ActionException("Cannot arrest self");
    if (p.blockedArrest) {
        p.blockedArrest = false;
        advance_turn();
        throw ActionException("Arrest blocked by Spy");
    }
    if (p.lastArrestTarget == t.name()) {
        advance_turn();
        throw ActionException("Cannot arrest same player twice");
    }

    if (t.role() == RoleType::Merchant) {
        int loss = std::min(t.coins(), 2);
        t.coin_count -= loss;
    } else if (t.role() != RoleType::General) {
        if (t.coins() == 0) throw ActionException("Target has no coins");
        t.coin_count -= 1;
        p.coin_count += 1;
    }

    p.lastArrestTarget = t.name();
    advance_turn();
}

void Game::sanction(Player &p, Player &t) {
    must_be_turn(p, players_list[turn_index]);
    if (p.coins() < 3) throw ActionException("Not enough coins");
    if (p.coins() >= 10) throw ActionException("Must coup with 10 coins");
    if (&p == &t) throw ActionException("Cannot sanction self");
    p.coin_count -= 3;
    if (t.role() == RoleType::Judge) p.coin_count -= 1;
    t.blockedGatherOrTax = true;
    sanction_expiry[t.name()] = turn_index;
    if (t.role() == RoleType::Baron) t.coin_count += 1;
    advance_turn();
}

void Game::coup(Player &p, Player &t) {
    must_be_turn(p, players_list[turn_index]);
    if (p.coins() < 7) throw ActionException("Not enough coins for coup");
    if (&p == &t) throw ActionException("Cannot coup self");
    must_exist(t);
    p.coin_count -= 7;
    auto it = coup_protection.find(t.name());
    if (it != coup_protection.end() && it->second) {
        coup_protection.erase(it);
    } else {
        t.active = false;
    }
    advance_turn();
}

int Game::spyView(Player &p, Player &t) {
    if (p.role() != RoleType::Spy) throw ActionException("Not a Spy");
    t.blockedArrest = true;
    return t.coins();
}

void Game::baronInvest(Player &p) {
    if (p.role() != RoleType::Baron) throw ActionException("Not a Baron");
    must_be_turn(p, players_list[turn_index]);
    if (p.coins() < 3) throw ActionException("Not enough coins");
    p.coin_count -= 3;
    p.coin_count += 6;
    advance_turn();
}

void Game::generalProtect(Player &p, Player &t) {
    if (p.role() != RoleType::General) throw ActionException("Not a General");
    if (p.coins() < 5) throw ActionException("Not enough coins");
    p.coin_count -= 5;
    coup_protection[t.name()] = true;
}

void Game::judgeBlockBribe(Player &p, Player &t) {
    if (p.role() != RoleType::Judge) throw ActionException("Not a Judge");
    t.blockedBribe = true;
}

void Game::governorBlockTax(Player &p, Player &t) {
    if (p.role() != RoleType::Governor) throw ActionException("Not a Governor");
    int gain = (t.role() == RoleType::Governor ? 3 : 2);
    if (t.coins() < gain) gain = t.coins();
    t.coin_count -= gain;
    t.blockedTax = true;
    tax_block_expiry[t.name()] = turn_index;
}


