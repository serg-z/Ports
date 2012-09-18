#include "pinwidget.h"

#include <QPainter>

#include <QDebug>

PinWidget::PinWidget(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(36, 44);

    m_pinName = "xxx";
    m_pinNumber = 0;

    m_pinNameFont.setPixelSize(12);
    m_pinNameFont.setBold(true);

    m_pinNumberFont.setPixelSize(12);
    m_pinNumberFont.setBold(true);

    m_pinLineSize = 3;

    m_checkable = m_checked = false;
}

void PinWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);

    // pin name
    painter.setPen(Qt::black);
    painter.setFont(m_pinNameFont);
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, m_pinName);

    // pin rect
    int nameSize = m_pinNameFont.pixelSize();
    int nameMargin = 3;
    int size = qMin(height() - nameSize - nameMargin, width());
    int hmargin = (width() - size + m_pinLineSize) * 0.5;
    int vmargin = (height() - nameSize - size + nameMargin + m_pinLineSize) * 0.5;
    size -= m_pinLineSize;
    QRect pr(hmargin, nameSize + vmargin, size, size);

    if (m_checked) {
        painter.setBrush(m_checkable ? Qt::red : Qt::green);
    } else {
        painter.setBrush(m_checkable ? Qt::lightGray : Qt::white);
    }

    painter.setPen(QPen(Qt::black, m_pinLineSize));
    painter.drawRoundedRect(pr, 7, 7);

    // pin number
    painter.setFont(m_pinNumberFont);
    painter.setPen(Qt::black);
    painter.drawText(pr, Qt::AlignCenter, QString("%1").arg(m_pinNumber));
}

void PinWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);

    if (m_checkable) {
        m_checked = !m_checked;

        emit toggled(m_checked, m_pinNumber);

        repaint();
    }
}

void PinWidget::setState(bool checked)
{
    m_checked = checked;

    repaint();
}
