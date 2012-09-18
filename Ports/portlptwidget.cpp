#include "portlptwidget.h"

#include <QBoxLayout>

#include <QDebug>

PortLptWidget::PortLptWidget(QWidget *parent) :
    PortWidget(parent)
{
    QVBoxLayout *vlo = new QVBoxLayout;

    // first row
    QHBoxLayout *row = new QHBoxLayout;
    row->setContentsMargins(0, 0, 0, 0);
    row->setSpacing(0);

    row->addStretch();
    row->addWidget(createPin(13, "SR4", false, "SLCT"));
    row->addWidget(createPin(12, "SR5", false, "PE (Paper End)"));
    row->addWidget(createPin(11, "SR7", false, "-BUSY"));
    row->addWidget(createPin(10, "SR6", false, "! ACK"));
    row->addWidget(createPin(9, "db7", true, "DB7"));
    row->addWidget(createPin(8, "db6", true, "DB6"));
    row->addWidget(createPin(7, "db5", true, "DB5"));
    row->addWidget(createPin(6, "db4", true, "DB4"));
    row->addWidget(createPin(5, "db3", true, "DB3"));
    row->addWidget(createPin(4, "db2", true, "DB2"));
    row->addWidget(createPin(3, "db1", true, "DB1"));
    row->addWidget(createPin(2, "db0", true, "DB0"));
    row->addWidget(createPin(1, "CR0", true, "!STROBE"));
    row->addStretch();

    vlo->addLayout(row);

    // second row
    row = new QHBoxLayout;
    row->setContentsMargins(0, 0, 0, 0);
    row->setSpacing(0);

    row->addStretch();
    row->addWidget(createPin(25, "GND", false, "GND"));
    row->addWidget(createPin(24, "---", false, "GND"));
    row->addWidget(createPin(23, "---", false, "GND"));
    row->addWidget(createPin(22, "---", false, "GND"));
    row->addWidget(createPin(21, "---", false, "GND"));
    row->addWidget(createPin(20, "---", false, "GND"));
    row->addWidget(createPin(19, "---", false, "GND"));
    row->addWidget(createPin(18, "GND", false, "GND"));
    row->addWidget(createPin(17, "CR3", true, "!SLIN"));
    row->addWidget(createPin(16, "CR2", true, "! INITIALIZE"));
    row->addWidget(createPin(15, "SR3", false, "! ERROR"));
    row->addWidget(createPin(14, "CR1", true, "!LF/CR"));
    row->addStretch();

    vlo->addLayout(row);

    setLayout(vlo);

    setFixedSize(sizeHint());
}
