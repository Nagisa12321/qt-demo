#include "addressbook.h"
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>

Addressbook::Addressbook(QWidget *parent) :
    QWidget(parent) {
    QLabel *nameLabel = new QLabel(tr("Name:"));
    QLabel *addressLabel = new QLabel(tr("Address:"));

    nameLine = new QLineEdit;
    addressText = new QTextEdit;

    // set read only
    nameLine->setReadOnly(true);
    addressText->setReadOnly(true);

    // add the button
    addButton = new QPushButton(tr("&Add"));
    submitButton = new QPushButton(tr("&Submit"));
    cancelButton = new QPushButton(tr("&Cancel"));
    nextButton = new QPushButton(tr("&Next"));
    prevButton = new QPushButton(tr("&Prev"));

    addButton->show();
    submitButton->hide();
    cancelButton->hide();

    nextButton->show();
    prevButton->show();
    nextButton->setEnabled(false);
    prevButton->setEnabled(false);

    // connect to the 'listener'
    connect(addButton, &QPushButton::clicked, this, &Addressbook::addContact);
    connect(submitButton, &QPushButton::clicked, this, &Addressbook::submitContact);
    connect(cancelButton, &QPushButton::clicked, this, &Addressbook::cancel);

    connect(nextButton, &QPushButton::clicked, this, &Addressbook::next);
    connect(prevButton, &QPushButton::clicked, this, &Addressbook::prev);

    // layout of button
    QVBoxLayout *buttonLayout1 = new QVBoxLayout;
    buttonLayout1->addWidget(addButton, Qt::AlignTop);
    buttonLayout1->addWidget(submitButton);
    buttonLayout1->addWidget(cancelButton);
    buttonLayout1->addStretch();

    // layout if next/prev
    QHBoxLayout *buttonLayout2 = new QHBoxLayout;
    buttonLayout2->addWidget(prevButton);
    buttonLayout2->addWidget(nextButton);

    // set the main layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(nameLine, 0, 1);
    mainLayout->addWidget(addressLabel, 1, 0, Qt::AlignTop);
    mainLayout->addWidget(addressText, 1, 1);
    mainLayout->addLayout(buttonLayout1, 1, 2);
    mainLayout->addLayout(buttonLayout2, 2, 1);

    setLayout(mainLayout);
    setWindowTitle("Simple Address Book");
}

void Addressbook::addContact() {
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    nameLine->clear();
    addressText->clear();

    nameLine->setReadOnly(false);
    nameLine->setFocus(Qt::OtherFocusReason);
    addressText->setReadOnly(false);

    addButton->setEnabled(false);
    submitButton->show();
    cancelButton->show();
}

void Addressbook::submitContact() {
    QString newName = nameLine->text();
    QString newAddress = addressText->toPlainText();

    if (newName.isEmpty() || newAddress.isEmpty()) {
        QMessageBox::information(this, tr("warnning"),
                                 tr("the name or the address should not be empty!"));
        return;
    }

    if (contacts.contains(newName)) {
        QMessageBox::information(this, tr("warnning"),
                                 tr("the name \"%1\" has already exist!").arg(newName));
        return;
    }

    contacts.insert(newName, newAddress);
    QMessageBox::information(this, tr("successful"),
                             tr("\"%1\" has been add to your address book!").arg(newName));

    // restore
    if (contacts.isEmpty()) {
        nameLine->clear();
        addressText->clear();
    }

    nameLine->setReadOnly(true);
    addressText->setReadOnly(true);

    addButton->setEnabled(true);
    cancelButton->hide();
    submitButton->hide();

    int number = contacts.size();
    prevButton->setEnabled(number > 1);
    nextButton->setEnabled(number > 1);
}

void Addressbook::cancel() {
    nameLine->setText(oldName);
    nameLine->setReadOnly(true);

    addressText->setText(oldAddress);
    addressText->setReadOnly(true);

    addButton->setEnabled(true);
    submitButton->hide();
    cancelButton->hide();
}

void Addressbook::next() {
    QString name = nameLine->text();

    QMap<QString, QString>::iterator it = contacts.find(name);
    if (it + 1 == contacts.end()) 
        it = contacts.begin();
    else ++it;
        
    nameLine->setText(it.key());
    addressText->setText(it.value());
}

void Addressbook::prev() {
    QString name = nameLine->text();

    QMap<QString, QString>::iterator it = contacts.find(name);
    if (it == contacts.begin()) 
        it = contacts.end() - 1;
    else --it;
        
    nameLine->setText(it.key());
    addressText->setText(it.value());
}

Addressbook::~Addressbook() {
}
