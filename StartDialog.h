#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QList>
#include <memory>
#include "Game.hpp"
/**
 * danieldaniel2468@gmail.com
 */
class StartDialog : public QDialog {
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    coup::Game* getInitializedGame() const;

private slots:
    void addPlayer();
    void finishSetup();

private:
    QVBoxLayout* layout;
    QList<QPair<QString, QString>> players;
    std::unique_ptr<coup::Game> initializedGame;
    QLineEdit* nameEdit;
    QComboBox* roleBox;
};
