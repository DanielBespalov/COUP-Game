#include "StartDialog.h"
#include <QLabel>
#include <QMessageBox>

using namespace coup;
/**
 * danieldaniel2468@gmail.com
 */
StartDialog::StartDialog(QWidget *parent)
    : QDialog(parent) {

    nameEdit = new QLineEdit(this);
    roleBox = new QComboBox(this);

    roleBox->addItem("Governor");
    roleBox->addItem("Spy");
    roleBox->addItem("Baron");
    roleBox->addItem("General");
    roleBox->addItem("Judge");
    roleBox->addItem("Merchant");

    QPushButton *addBtn = new QPushButton("הוסף שחקן", this);
    QPushButton *finishBtn = new QPushButton("התחל משחק", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("בחר שם ותפקיד", this));
    layout->addWidget(nameEdit);
    layout->addWidget(roleBox);
    layout->addWidget(addBtn);
    layout->addWidget(finishBtn);
    setLayout(layout);

    connect(addBtn, &QPushButton::clicked, this, &StartDialog::addPlayer);
    connect(finishBtn, &QPushButton::clicked, this, &StartDialog::finishSetup);
}

void StartDialog::addPlayer() {
    QString name = nameEdit->text().trimmed();
    QString role = roleBox->currentText();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "שגיאה", "יש להזין שם שחקן");
        return;
    }

    for (const auto& p : players) {
        if (p.first == name) {
            QMessageBox::warning(this, "שגיאה", "שחקן כבר קיים.");
            return;
        }
    }

    if (players.size() >= 6) {
        QMessageBox::warning(this, "שגיאה", "לא ניתן להוסיף יותר מ־6 שחקנים.");
        return;
    }

    players.append(qMakePair(name, role));
    nameEdit->clear();
}


void StartDialog::finishSetup() {
    if (players.size() < 2) {
        QMessageBox::warning(this, "שגיאה", "חייבים לפחות שני שחקנים.");
        return;
    }

    initializedGame = std::make_unique<Game>();
    for (const auto& p : players) {
        initializedGame->addPlayer(p.first.toStdString(), p.second.toStdString());
    }
    accept();
}


Game* StartDialog::getInitializedGame() const {
    return initializedGame ? initializedGame.get() : nullptr;
}
