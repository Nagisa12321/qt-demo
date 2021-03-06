#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "addressbook.h"
#include "find_dialog.h"

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
    removeButton = new QPushButton(tr("&Remove"));
    editButton = new QPushButton(tr("&Edit"));
    findButton = new QPushButton(tr("&Find"));
    saveButton = new QPushButton(tr("&Save"));
    loadButton = new QPushButton(tr("&Load"));

    addButton->show();
    editButton->show();
    removeButton->show();

    submitButton->hide();
    cancelButton->hide();

    nextButton->show();
    prevButton->show();

    findButton->show();

    saveButton->show();
    loadButton->show();

    nextButton->setEnabled(false);
    prevButton->setEnabled(false);

    removeButton->setEnabled(false);
    editButton->setEnabled(false);

    // add the 'listener'
    connect(addButton, &QPushButton::clicked, this, &Addressbook::addContact);
    connect(submitButton, &QPushButton::clicked, this, &Addressbook::submitContact);
    connect(cancelButton, &QPushButton::clicked, this, &Addressbook::cancel);

    connect(nextButton, &QPushButton::clicked, this, &Addressbook::next);
    connect(prevButton, &QPushButton::clicked, this, &Addressbook::prev);

    connect(editButton, &QPushButton::clicked, this, &Addressbook::editContact);
    connect(removeButton, &QPushButton::clicked, this, &Addressbook::removeContact);

    connect(findButton, &QPushButton::clicked, this, &Addressbook::find);

    connect(saveButton, &QPushButton::clicked, this, &Addressbook::save);
    connect(loadButton, &QPushButton::clicked, this, &Addressbook::load);

    // layout of button
    QVBoxLayout *buttonLayout1 = new QVBoxLayout;
    buttonLayout1->addWidget(addButton, Qt::AlignTop);
    buttonLayout1->addWidget(submitButton);
    buttonLayout1->addWidget(cancelButton);
    buttonLayout1->addWidget(editButton);
    buttonLayout1->addWidget(removeButton);
    buttonLayout1->addWidget(findButton);
    buttonLayout1->addWidget(saveButton);
    buttonLayout1->addWidget(loadButton);
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

void Addressbook::save() {
    QString fileName =
        QFileDialog::getSaveFileName(this,
                                     tr("Save Address Book"), "",
                                     tr("Address Book (*.abk);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_5);
    out << contacts;
}

void Addressbook::load() {
    QString fileName =
        QFileDialog::getOpenFileName(this,
                                     tr("Open Address Book"), "",
                                     tr("Address Book (*.abk);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_5);
    contacts.clear(); // clear existing contacts
    in >> contacts;

    if (contacts.isEmpty()) {
        QMessageBox::information(this, tr("No contacts in file"),
                                 tr("The file you are attempting to open contains no contacts."));
    } else {
        QMap<QString, QString>::iterator i = contacts.begin();
        nameLine->setText(i.key());
        addressText->setText(i.value());
    }

    updateInterface(Mode::NavigationMode);
}

void Addressbook::find() {
    FindDialog dialog;
    dialog.show();

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getFindText();

        if (contacts.contains(name)) {
            QMap<QString, QString>::iterator it = contacts.find(name);
            nameLine->setText(it.key());
            addressText->setText(it.value());
        } else {
            QMessageBox::information(this, tr("warnning"),
                                     tr("the name \"%1\" is not exist!").arg(name));
            return;
        }
    }
}

void Addressbook::addContact() {
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    nameLine->clear();
    addressText->clear();

    updateInterface(Mode::AddingMode);
}

void Addressbook::submitContact() {
    QString newName = nameLine->text();
    QString newAddress = addressText->toPlainText();

    if (newName.isEmpty() || newAddress.isEmpty()) {
        QMessageBox::information(this, tr("warnning"),
                                 tr("the name or the address should not be empty!"));
        return;
    } else if (contacts.contains(newName)) {
        if (currentMod == Mode::AddingMode) {
            QMessageBox::information(this, tr("warnning"),
                                     tr("the name \"%1\" has already exist!").arg(newName));
            return;
        } else {
            contacts.insert(newName, newAddress);
            QMessageBox::information(this, tr("successful"),
                                     tr("\"%1\" has been changed!").arg(newName));
        }
    } else {
        contacts.insert(newName, newAddress);
        QMessageBox::information(this, tr("successful"),
                                 tr("\"%1\" has been add to your address book!").arg(newName));
    }

    updateInterface(Mode::NavigationMode);
}

void Addressbook::cancel() {
    nameLine->setText(oldName);
    addressText->setText(oldAddress);

    updateInterface(Mode::NavigationMode);
}

void Addressbook::next() {
    QString name = nameLine->text();

    QMap<QString, QString>::iterator it = contacts.find(name);
    if (it + 1 == contacts.end())
        it = contacts.begin();
    else
        ++it;

    nameLine->setText(it.key());
    addressText->setText(it.value());
}

void Addressbook::prev() {
    QString name = nameLine->text();

    QMap<QString, QString>::iterator it = contacts.find(name);
    if (it == contacts.begin())
        it = contacts.end() - 1;
    else
        --it;

    nameLine->setText(it.key());
    addressText->setText(it.value());
}

void Addressbook::removeContact() {
    QString name = nameLine->text();

    if (contacts.contains(name)) {
        int button = QMessageBox::question(this, tr("Comfirm remove"),
                                           tr("r u sure to remove \"%1\"?").arg(name),
                                           QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes) {
            prev();
            contacts.remove(name);
            QMessageBox::information(this, tr("Remove success!"),
                                     tr("\"%1\" had been removed.").arg(name));
        }
    }

    updateInterface(Mode::NavigationMode);
}

void Addressbook::editContact() {
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();
    updateInterface(Mode::EditingMode);
    addressText->setFocus(Qt::OtherFocusReason);
}

void Addressbook::updateInterface(Mode mod) {
    currentMod = mod;

    switch (currentMod) {
    case Mode::AddingMode:
        nameLine->setReadOnly(false);
        nameLine->setFocus(Qt::OtherFocusReason);
    case Mode::EditingMode:
        addressText->setReadOnly(false);
        addButton->setEnabled(false);

        editButton->hide();
        removeButton->hide();
        saveButton->hide();
        loadButton->hide();

        submitButton->show();
        cancelButton->show();

        prevButton->setEnabled(false);
        nextButton->setEnabled(false);

        findButton->setEnabled(false);
        break;
    case Mode::NavigationMode:
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
        editButton->show();
        removeButton->show();

        int number = contacts.size();
        saveButton->show();
        loadButton->show();
        findButton->setEnabled(true);
        prevButton->setEnabled(number > 1);
        nextButton->setEnabled(number > 1);
        removeButton->show();
        editButton->show();
        removeButton->setEnabled(number);
        editButton->setEnabled(number);
        break;
    }
}

Addressbook::~Addressbook() {
}
