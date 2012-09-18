#ifndef PINWIDGET_H
#define PINWIDGET_H

#include <QWidget>
#include <QFont>

class PinWidget : public QWidget
{
    friend class PortWidget;

    Q_OBJECT
public:
    explicit PinWidget(QWidget *parent, int number, const QString &header,
                       bool interactive = false, const QString &toolTip = "");

signals:
    // emitted when user toggles pin
    void toggled(bool state);
    // emitted when pin state changed using setState
    void stateChanged(bool state);
    
public slots:
    void setState(bool state);
    
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

    int m_number;
    QString m_header;
    bool m_interactive;
    bool m_state;
    int m_pinLineSize;
    QFont m_pinNameFont;
    QFont m_pinNumberFont;
};

#endif // PINWIDGET_H
