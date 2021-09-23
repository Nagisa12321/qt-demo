#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include "find_dialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent) {
    QLabel *label = new QLabel(tr("Enter name of contact: "));
    edit = new QLineEdit;

    button = new QPushButton(tr("&Find"));
    findText = "";

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label);
    layout->addWidget(edit);
    layout->addWidget(button);

    this->setLayout(layout);
    this->setWindowTitle("Find");

    connect(button, &QPushButton::clicked, this, &FindDialog::findClicked);
    connect(button, &QPushButton::clicked, this, &FindDialog::accept);
}

void FindDialog::findClicked() {
    QString name = edit->text();

    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Empty Field"),
              tr("Please enter a name."));
        return;
    } else {
        findText = name;
        edit->clear();
        hide();
    }
}

QString FindDialog::getFindText() {
    return this->findText;
}