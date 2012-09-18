#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "InpOut.h"
#include "portlptwidget.h"

#include <QDesktopWidget>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timer(this)
{
    ui->setupUi(this);

    // move to top left corner
    QRect geom(geometry());
    geom.moveTopLeft(QApplication::desktop()->geometry().topLeft());
    setGeometry(geom);

    // set up dock widgets
    ui->dockWidgetLpt1->setFixedSize(ui->dockWidgetLpt1->sizeHint());
    ui->dockWidgetLpt2->setFixedSize(ui->dockWidgetLpt1->sizeHint());
    ui->dockWidgetCom->setFixedSize(ui->dockWidgetLpt1->sizeHint());

    tabifyDockWidget(ui->dockWidgetLpt1, ui->dockWidgetLpt2);
    tabifyDockWidget(ui->dockWidgetLpt2, ui->dockWidgetCom);
    ui->dockWidgetLpt1->raise();

    ui->dockWidgetCom->hide();

    if (InpOut::Simulated())
        ui->dockWidgetLpt1->setWindowTitle("LPT1 - [SIMULATION]");
    else
        ui->dockWidgetLpt1->setWindowTitle(QString("LPT1 - %1").arg(InpOut::Initialized() ? "[INITIALIZED]" : "[NOT INITIALIZED]"));

    setFixedSize(sizeHint());

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

//    ui->pinWidget2->setState(state & 0x1); // db0
//    ui->pinWidget3->setState(state & 0x2); // db1
//    ui->pinWidget4->setState(state & 0x4); // db2
//    ui->pinWidget5->setState(state & 0x8); // db3
//    ui->pinWidget6->setState(state & 0x10); // db4
//    ui->pinWidget7->setState(state & 0x20); // db5
//    ui->pinWidget8->setState(state & 0x40); // db6
//    ui->pinWidget9->setState(state & 0x80); // db7

    // state (+1)
    state = InpOut::Read(base + 1);

    // 0 - 2 - not used
//    ui->pinWidget15->setState(state & 0x8); // !error
//    ui->pinWidget13->setState(state & 0x10); // slct
//    ui->pinWidget12->setState(state & 0x20); // pe
//    ui->pinWidget10->setState(state & 0x40); // !ack
//    ui->pinWidget11->setState(state & 0x80); // !busy

    // control (+2)
    state = InpOut::Read(base + 2);

//    ui->pinWidget1->setState(state & 0x1); // !strobe
//    ui->pinWidget14->setState(state & 0x2); // !lf/cr
//    ui->pinWidget16->setState(state & 0x4); // !initialize
//    ui->pinWidget17->setState(state & 0x8); // !slin
    // irq
    // data i/o
    // 6, 7 - not used
}

void MainWindow::pinToggled(bool state)
{
    const PinWidget &pin = *static_cast<PinWidget*>(sender());

    int pinNumber = 0;//pin.m_number;

    unsigned short base = 0x378;

    unsigned short v;

    // data base (+0)
    if (pinNumber > 1 && pinNumber < 10) {
        v = InpOut::Read(base);
        InpOut::Write(base, v ^ 0x1 << (pinNumber - 2));
    }

    // control base (+2)
    v = InpOut::Read(base + 2);

    switch (pinNumber)  {
        case 1: InpOut::Write(base + 2, v ^ 0x1); break;
        case 14: InpOut::Write(base + 2, v ^ 0x2); break;
        case 16: InpOut::Write(base + 2, v ^ 0x4); break;
        case 17: InpOut::Write(base + 2, v ^ 0x8); break;
    }
}

void MainWindow::setPinParameters(PinWidget *pin, bool checkable, int number, const QString &header, const QString &toolTip)
{
/*    pin->m_interactive = checkable;
    pin->m_number = number;
    pin->m_header = header;
    pin->setToolTip(toolTip);

    if (checkable)
        connect(pin, SIGNAL(toggled(bool)), SLOT(pinToggled(bool)));*/
}
