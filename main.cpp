#include "addressbook.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Addressbook book;
    book.show();
    return a.exec();
}
