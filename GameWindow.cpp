// GameWindow.cpp
#include "GameWindow.h"
#include "GameQtBridge.hpp"
#include "Game.hpp"
#include <QInputDialog>
#include <QMessageBox>
#include <QMap>
#include <QPair>
#include <QSet>

/**
 * danieldaniel2468@gmail.com
 */
using namespace coup;

GameQtBridge bridge;

GameWindow::GameWindow(Game* game, QWidget* parent) : QMainWindow(parent), game(game) {
    central = new QWidget(this);
    setCentralWidget(central);

    mainLayout = new QVBoxLayout(central);

    turnLabel = new QLabel(this);
    coinLabel = new QLabel(this);

    playerBox = new QGroupBox("שחקנים פעילים", this);
    playerLayout = new QVBoxLayout();
    playerBox->setLayout(playerLayout);

    actionsBox = new QGroupBox("פעולות זמינות", this);
    actionsLayout = new QVBoxLayout();
    actionsBox->setLayout(actionsLayout);

    mainLayout->addWidget(turnLabel);
    mainLayout->addWidget(coinLabel);
    mainLayout->addWidget(playerBox);
    mainLayout->addWidget(actionsBox);

    refreshUI();
}

void GameWindow::refreshUI() {
    if ((int)(game->players().size()) == 1) {
    QMessageBox::information(this, "ניצחון!", QString::fromStdString("המנצח הוא: " + game->winner()));
    } else {
        game->nextTurn_qt();
    }
    QLayoutItem* item;
    while ((item = playerLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    while ((item = actionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QString currentName = bridge.currentPlayerName_qt(game);
    QString currentRole = bridge.currentPlayerRole_qt(game);
    int currentCoins = bridge.currentPlayerCoins_qt(game);

    QLabel* turn = new QLabel("תור: " + currentName + " (" + currentRole + ")");
    QLabel* coins = new QLabel("מטבעות: " + QString::number(currentCoins));
    playerLayout->addWidget(turn);
    playerLayout->addWidget(coins);

    QSet<QString> seenNames;
    for (const QString& playerName : bridge.activePlayers_qt(game)) {
        if (!seenNames.contains(playerName)) {
            playerLayout->addWidget(new QLabel(playerName));
            seenNames.insert(playerName);
        }
    }

   
    createActionButtons(currentRole);
}


void GameWindow::clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) delete widget;
        delete item;
    }
}

void GameWindow::createActionButtons(const QString& role) {
    auto addActionButton = [&](const QString& text, const QString& tip) {
        QPushButton* btn = new QPushButton(text, this);
        btn->setToolTip(tip);
        connect(btn, &QPushButton::clicked, this, &GameWindow::handleAction);
        actionsLayout->addWidget(btn);
    };

    // Universal actions
    addActionButton("gather", "קח מטבע אחד מהאוצר");
    addActionButton("tax", "גבה מסים (3 מטבעות)");
    addActionButton("bribe", "שוחד כדי לבצע פעולה נוספת");
    addActionButton("arrest", "עצור שחקן אחר");
    addActionButton("sanction", "סנקציה על שחקן");
    addActionButton("coup", "הפלה של שחקן אחר");

    // Role-specific
    QMap<QString, QPair<QString, QString>> roleMap = {
        {"Governor", {"cancel_tax", "ביטול גביית מס של שחקן אחר"}},
        {"Spy", {"spy", "ריגול - צפה בכמות מטבעות וחסימת מעצר"}},
        {"Baron", {"invest", "השקעה - שלם 3 וקבל 6"}},
        {"General", {"block_coup", "הגנה מפני הפלה"}},
        {"Judge", {"cancel_bribe", "ביטול שוחד"}},
        {"Merchant", {"bonus_coin", "בונוס פסיבי בתחילת תור"}}
    };

    if (roleMap.contains(role)) {
        auto action = roleMap[role];
        addActionButton(action.first, action.second);
    }

    // Utility
    addActionButton("end_turn", "סיים את תורך");
}

void GameWindow::handleAction() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString action = btn->text();
    try {
        if (action == "spy") {
            bool ok;
            QStringList list = QStringList::fromVector(QVector<QString>(bridge.activePlayers_qt(game).begin(), bridge.activePlayers_qt(game).end()));
            QString targetName = QInputDialog::getItem(this, "Coup", "בחר שחקן לריגול", list, 0, false, &ok);
            if (ok && !targetName.isEmpty()) {
                Player* attacker = game->players_list[game->getTurnIndex()];
                Player* target = &game->getPlayer(targetName.toStdString());
                int coins = game->spyView(*attacker, *target);
                QMessageBox::information(this, "Spy", QString("%1 מחזיק %2 מטבעות").arg(targetName).arg(coins));
                refreshUI();
                return;
            }
        }

        else if (action == "coup") {
            bool ok;
            QStringList list = QStringList::fromVector(QVector<QString>(bridge.activePlayers_qt(game).begin(), bridge.activePlayers_qt(game).end()));
            QString targetName = QInputDialog::getItem(this, "Coup", "בחר שחקן להפיכה", list, 0, false, &ok);
            if (ok && !targetName.isEmpty()) {
                Player* attacker = game->players_list[game->getTurnIndex()];
                Player* target = &game->getPlayer(targetName.toStdString());

                if (attacker == target) {
                    throw std::invalid_argument("אסור לבצע הפיכה על עצמך");
                }

                game->coup(*attacker, *target);
                refreshUI();
            }
        }

        else {
            bridge.performAction_qt(game, action);
        }

        game->nextTurn_qt();

        if (game->players().size() == 1) {
            QString winner = QString::fromStdString(game->winner());
            QMessageBox::information(this, "ניצחון!", "המנצח הוא: " + winner);
        }

        refreshUI();
    } catch (const std::exception& ex) {
        showError(QString::fromStdString(ex.what()));
    }
}



void GameWindow::showError(const QString& message) {
    QMessageBox::critical(this, "שגיאה", message);
}
