#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QPlastiqueStyle>

#ifdef __linux__
#include <QMessageBox>

#include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    QApplication::setStyle(new QPlastiqueStyle);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    
    return a.exec();
}
