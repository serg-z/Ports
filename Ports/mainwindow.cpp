#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "InpOut.h"

#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timer(this)
{
    ui->setupUi(this);

    setFixedSize(sizeHint());

    QRect geom(geometry());
    geom.moveBottomLeft(QApplication::desktop()->geometry().bottomLeft());
    setGeometry(geom);

    if (InpOut::Simulated())
        setWindowTitle("Ports - [SIMULATION]");
    else
        setWindowTitle(QString("Ports - %1").arg(InpOut::Initialized() ? "[INITIALIZED]" : "[NOT INITIALIZED]"));

    setPinParameters(ui->pinWidget1, true, 1, "CR0", "!STROBE");
    setPinParameters(ui->pinWidget2, true, 2, "db0", "DB0");
    setPinParameters(ui->pinWidget3, true, 3, "db1", "DB1");
    setPinParameters(ui->pinWidget4, true, 4, "db2", "DB2");
    setPinParameters(ui->pinWidget5, true, 5, "db3", "DB3");
    setPinParameters(ui->pinWidget6, true, 6, "db4", "DB4");
    setPinParameters(ui->pinWidget7, true, 7, "db5", "DB5");
    setPinParameters(ui->pinWidget8, true, 8, "db6", "DB6");
    setPinParameters(ui->pinWidget9, true, 9, "db7", "DB7");
    setPinParameters(ui->pinWidget10, false, 10, "SR6", "! ACK");
    setPinParameters(ui->pinWidget11, false, 11, "SR7", "-BUSY");
    setPinParameters(ui->pinWidget12, false, 12, "SR5", "PE (Paper End)");
    setPinParameters(ui->pinWidget13, false, 13, "SR4", "SLCT");
    setPinParameters(ui->pinWidget14, true, 14, "CR1", "!LF/CR");
    setPinParameters(ui->pinWidget15, false, 15, "SR3", "! ERROR");
    setPinParameters(ui->pinWidget16, true, 16, "CR2", "! INITIALIZE");
    setPinParameters(ui->pinWidget17, true, 17, "CR3", "!SLIN");
    setPinParameters(ui->pinWidget18, false, 18, "GND", "GND");
    setPinParameters(ui->pinWidget19, false, 19, "---", "GND");
    setPinParameters(ui->pinWidget20, false, 20, "---", "GND");
    setPinParameters(ui->pinWidget21, false, 21, "---", "GND");
    setPinParameters(ui->pinWidget22, false, 22, "---", "GND");
    setPinParameters(ui->pinWidget23, false, 23, "---", "GND");
    setPinParameters(ui->pinWidget24, false, 24, "---", "GND");
    setPinParameters(ui->pinWidget25, false, 25, "GND", "GND");

    ui->pinWidget18->setState(0);
    ui->pinWidget19->setState(0);
    ui->pinWidget20->setState(0);
    ui->pinWidget21->setState(0);
    ui->pinWidget22->setState(0);
    ui->pinWidget23->setState(0);
    ui->pinWidget24->setState(0);
    ui->pinWidget25->setState(0);

    m_timer.setSingleShot(false);
    connect(&m_timer, SIGNAL(timeout()), SLOT(updatePins()));
    m_timer.start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePins()
{
    unsigned short base = 0x378;

    unsigned short state;

    // data base (+0)
    state = InpOut::Read(base);

    ui->pinWidget2->setState(state & 0x1); // db0
    ui->pinWidget3->setState(state & 0x2); // db1
    ui->pinWidget4->setState(state & 0x4); // db2
    ui->pinWidget5->setState(state & 0x8); // db3
    ui->pinWidget6->setState(state & 0x10); // db4
    ui->pinWidget7->setState(state & 0x20); // db5
    ui->pinWidget8->setState(state & 0x40); // db6
    ui->pinWidget9->setState(state & 0x80); // db7

    // state (+1)
    state = InpOut::Read(base + 1);

    // 0 - 2 - not used
    ui->pinWidget15->setState(state & 0x8); // !error
    ui->pinWidget13->setState(state & 0x10); // slct
    ui->pinWidget12->setState(state & 0x20); // pe
    ui->pinWidget10->setState(state & 0x40); // !ack
    ui->pinWidget11->setState(state & 0x80); // !busy

    // control (+2)
    state = InpOut::Read(base + 2);

    ui->pinWidget1->setState(state & 0x1); // !strobe
    ui->pinWidget14->setState(state & 0x2); // !lf/cr
    ui->pinWidget16->setState(state & 0x4); // !initialize
    ui->pinWidget17->setState(state & 0x8); // !slin
    // irq
    // data i/o
    // 6, 7 - not used
}

void MainWindow::pinToggled(bool /*checked*/, int pinNumber)
{
    unsigned short base = 0x378;

    unsigned short state;

    // data base (+0)
    if (pinNumber > 1 && pinNumber < 10) {
        state = InpOut::Read(base);
        InpOut::Write(base, state ^ 0x1 << (pinNumber - 2));
    }

    // control base (+2)
    state = InpOut::Read(base + 2);

    switch (pinNumber)  {
        case 1: InpOut::Write(base + 2, state ^ 0x1); break;
        case 14: InpOut::Write(base + 2, state ^ 0x2); break;
        case 16: InpOut::Write(base + 2, state ^ 0x4); break;
        case 17: InpOut::Write(base + 2, state ^ 0x8); break;
    }
}

void MainWindow::setPinParameters(PinWidget* pin, bool checkable, int pinNumber, const QString &pinName, const QString &toolTip)
{
    pin->m_checkable = checkable;
    pin->m_pinNumber = pinNumber;
    pin->m_pinName = pinName;
    pin->setToolTip(toolTip);

    if (checkable)
        connect(pin, SIGNAL(toggled(bool,int)), SLOT(pinToggled(bool,int)));
}
