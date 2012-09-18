#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pinwidget.h"

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void updatePins();
    void pinToggled(bool checked, int pinNumber);

private:
    void setPinParameters(PinWidget* pin, bool checkable, int pinNumber, const QString &pinName, const QString &toolTip);

    Ui::MainWindow *ui;
    QTimer m_timer;
};

#endif // MAINWINDOW_H
