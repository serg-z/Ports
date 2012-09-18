#include "portwidget.h"

#include <QDebug>

PortWidget::PortWidget(QWidget *parent) :
    QWidget(parent)
{
}

void PortWidget::setPinState(int pinNumber, bool state)
{
    auto it = m_pins.find(pinNumber);

    if (it != m_pins.end())
        it.value()->setState(state);
    else
        qWarning() << QString("Pin %1 not found").arg(pinNumber);
}

void PortWidget::pinToggled(bool state)
{
    emit pinToggled(static_cast<PinWidget*>(sender())->m_number, state);
}

PinWidget* PortWidget::createPin(int number, const QString &header, bool interactive, const QString toolTip)
{
    auto it = m_pins.find(number);

    if (it != m_pins.end()) {
        qWarning() << QString("Pin %1 already created").arg(number);
        return it.value();
    }

    PinWidget *p = new PinWidget(this, number, header, interactive, toolTip);

    connect(p, SIGNAL(toggled(bool)), SLOT(pinToggled(bool)));

    m_pins[number] = p;

    return p;
}
