#include "addressbook.h"
#include <QLabel>
#include <QGridLayout>

addressbook::addressbook(QWidget *parent)
    : QWidget(parent)
{
    QLabel *nameLabel = new QLabel(tr("Name:"));
    QLabel *addressLabel = new QLabel(tr("Address:"));

    nameLine = new QLineEdit;
    addressText = new QTextEdit;

    // set the layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(nameLine, 0, 1);
    mainLayout->addWidget(addressLabel, 1, 0, Qt::AlignTop);
    mainLayout->addWidget(addressText, 1, 1);

    setLayout(mainLayout);
    setWindowTitle("Simple Address Book");
}

addressbook::~addressbook()
{
}

