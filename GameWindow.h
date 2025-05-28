// GameWindow.h
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QString>

#include "Game.hpp"
/**
 * danieldaniel2468@gmail.com
 */
class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(coup::Game* game, QWidget* parent = nullptr);

private slots:
    void handleAction();

private:
    coup::Game* game;

    QWidget* central;
    QVBoxLayout* mainLayout;

    QLabel* turnLabel;
    QLabel* coinLabel;

    QGroupBox* playerBox;
    QVBoxLayout* playerLayout;

    QGroupBox* actionsBox;
    QVBoxLayout* actionsLayout;

    void refreshUI();
    void createActionButtons(const QString& role);
    void showError(const QString& message);
    void clearLayout(QLayout* layout);
};

#endif // GAMEWINDOW_H
