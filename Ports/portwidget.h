#ifndef PORTWIDGET_H
#define PORTWIDGET_H

#include "pinwidget.h"

#include <QWidget>
#include <QMap>

class PortWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PortWidget(QWidget *parent = 0);
    
signals:
    // emitted when user toggles pin
    void pinToggled(int pinNumber, bool state);
    // emitted when pin state changed using setState
    void pinStateChanged(int pinNumber, bool state);
    
public slots:
    void setPinState(int number, bool state);

protected slots:
    void pinToggled(bool state);

protected:
    PinWidget* createPin(int number, const QString &header, bool interactive = false, const QString toolTip = "");

    QMap<int, PinWidget*> m_pins;
};

#endif // PORTWIDGET_H
