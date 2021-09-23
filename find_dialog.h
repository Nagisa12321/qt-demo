#ifndef _FIND_DIALOG_H
#define _FIND_DIALOG_H
#include <QDialog>
#include <QLineEdit>

class FindDialog : public QDialog {
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);
    QString getFindText();

public slots:
    void findClicked();

private:
    QPushButton *button;
    QLineEdit *edit;
    QString findText;
};
#endif