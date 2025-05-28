#ifndef COUP_GAMEQTBRIDGE_HPP
#define COUP_GAMEQTBRIDGE_HPP

#include "Player.hpp"
#include "RoleType.hpp"
#include "Game.hpp"
#include <QString>
#include <vector>
#include <stdexcept>
/**
 * danieldaniel2468@gmail.com
 */
namespace coup {

class GameQtBridge {
public:
    void performAction_qt(Game* game, const QString& action) {
        Player* cur = game->players_list[game->getTurnIndex()];
        std::string act = action.toStdString();

        if (act == "gather") game->gather(*cur);
        else if (act == "tax") game->tax(*cur);
        else if (act == "bribe") game->bribe(*cur);
        else if (act == "invest") game->baronInvest(*cur);
        else if (act == "bonus_coin") cur->gather();  
        else if (act == "end_turn") game->nextTurn_qt();
        else throw std::runtime_error("Unsupported action: " + act);
    }

    void performActionOnTarget_qt(Game* game, const QString& action, const QString& targetName) {
        Player* cur = game->players_list[game->getTurnIndex()];
        Player& target = game->getPlayer(targetName.toStdString());
        std::string act = action.toStdString();

        if (act == "arrest") game->arrest(*cur, target);
        else if (act == "sanction") game->sanction(*cur, target);
        else if (act == "coup") game->coup(*cur, target);
        else if (act == "spy") spyView(*game, *cur, target);
        else if (act == "cancel_tax") game->governorBlockTax(*cur, target);
        else if (act == "cancel_bribe") game->judgeBlockBribe(*cur, target);
        else if (act == "block_coup") game->generalProtect(*cur, target);
        else throw std::runtime_error("Unsupported target action: " + act);
    }

    QString currentPlayerName_qt(Game* game) const {
        return QString::fromStdString(game->turn());
    }

    QString currentPlayerRole_qt(Game* game) const {
        return QString::fromStdString(to_string(game->players_list[game->getTurnIndex()]->role()));
    }

    int currentPlayerCoins_qt(Game* game) const {
        return game->players_list[game->getTurnIndex()]->coins();
    }

    std::vector<QString> activePlayers_qt(Game* game, bool onlyOthers = false, const std::string& exclude = "") const {
        std::vector<QString> names;
        for (auto* p : game->players_list) {
            if (!p->isActive()) continue;
            if (onlyOthers && p->name() == exclude) continue;
            names.push_back(QString::fromStdString(p->name()));
        }
        return names;
    }

    bool hasWinner_qt(Game* game) const {
        return game->players().size() == 1;
    }

    QString winner_qt(Game* game) const {
        return QString::fromStdString(game->winner());
    }

    int spyView(Game& game, Player& spy, Player& target) const {
        game.spyView(spy, target);
        return target.coins();
    }
};

} // namespace coup

#endif // COUP_GAMEQTBRIDGE_HPP
