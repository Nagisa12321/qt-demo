#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>

class addressbook : public QWidget
{
    Q_OBJECT

public:
    addressbook(QWidget *parent = nullptr);
    ~addressbook();

private:
    QLineEdit *nameLine;
    QTextEdit *addressText;
};
#endif // ADDRESSBOOK_H
