#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

class Addressbook : public QWidget {
    Q_OBJECT

public:
    Addressbook(QWidget *parent = nullptr);
    ~Addressbook();

public slots:
    void addContact();
    void submitContact();
    void cancel();
    void next();
    void prev();


private:
    QMap<QString, QString> contacts;
    QString oldName;
    QString oldAddress;
    QPushButton *addButton;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    QLineEdit *nameLine;
    QTextEdit *addressText;

    QPushButton *nextButton;
    QPushButton *prevButton;
};
#endif // ADDRESSBOOK_H
