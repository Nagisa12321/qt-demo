#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

enum Mode {
    NavigationMode,
    AddingMode,
    EditingMode
};

class Addressbook : public QWidget {
    Q_OBJECT

public:
    Addressbook(QWidget *parent = nullptr);
    ~Addressbook();

public slots:
    void addContact();
    void submitContact();
    void editContact();
    void removeContact();
    void cancel();
    void next();
    void prev();
    void find();
    void save();
    void load();

    // change the mode
    void updateInterface(Mode mod);

private:
    QMap<QString, QString> contacts;
    QString oldName;
    QString oldAddress;
    QLineEdit *nameLine;
    QTextEdit *addressText;

    // btns 
    QPushButton *submitButton;
    QPushButton *cancelButton;
    QPushButton *addButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QPushButton *findButton;
    QPushButton *saveButton;
    QPushButton *loadButton;

    // mod
    Mode currentMod;
};
#endif // ADDRESSBOOK_H
