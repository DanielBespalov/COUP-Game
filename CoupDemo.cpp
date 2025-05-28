#include <QApplication>
#include "GameWindow.h"
#include "StartDialog.h"
#include "Game.hpp"
#include <QMessageBox>
/**
 * danieldaniel2468@gmail.com
 */
using namespace coup;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    StartDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        Game* game = dialog.getInitializedGame();
        if (!game) {
            QMessageBox::critical(nullptr, "שגיאה", "לא נוספו שחקנים למשחק.");
            return 1;
        }

        GameWindow window(game);
        window.show();
        return app.exec();
    }

    return 0;
}
