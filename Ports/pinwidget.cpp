#include "pinwidget.h"

#include <QPainter>

#include <QDebug>

PinWidget::PinWidget(QWidget *parent, int number, const QString &header,
                     bool interactive, const QString &toolTip) :
    QWidget(parent),
    m_number(number),
    m_header(header),
    m_interactive(interactive),
    m_state(false),
    m_pinLineSize(3)
{
    setMinimumSize(36, 60);

    m_pinNameFont.setPixelSize(12);
    m_pinNameFont.setBold(true);

    m_pinNumberFont.setPixelSize(12);
    m_pinNumberFont.setBold(true);

    setToolTip(toolTip);
}

void PinWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);

    // header
    painter.setPen(Qt::black);
    painter.setFont(m_pinNameFont);
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, m_header);

    // rounded rect
    int nameSize = m_pinNameFont.pixelSize();
    int nameMargin = 3;
    int size = qMin(height() - nameSize - nameMargin, width() - 2);
    int hmargin = (width() - size + m_pinLineSize) * 0.5;
    int vmargin = (height() - nameSize - size + nameMargin + m_pinLineSize) * 0.5;
    size -= m_pinLineSize;
    QRect pr(hmargin, nameSize + vmargin, size, size);

    if (m_state) {
        painter.setBrush(m_interactive ? Qt::red : Qt::green);
    } else {
        painter.setBrush(m_interactive ? Qt::lightGray : Qt::white);
    }

    painter.setPen(QPen(Qt::black, m_pinLineSize));
    painter.drawRoundedRect(pr, 7, 7);

    // inner text
    painter.setFont(m_pinNumberFont);
    painter.setPen(Qt::black);
    painter.drawText(pr, Qt::AlignCenter, QString("%1").arg(m_number));
}

void PinWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);

    if (m_interactive) {
        m_state = !m_state;

        repaint();

        emit toggled(m_state);
    }
}

void PinWidget::setState(bool state)
{
    if (m_state != state) {
        m_state = state;

        repaint();

        emit stateChanged(state);
    }
}
