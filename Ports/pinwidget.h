#ifndef PINWIDGET_H
#define PINWIDGET_H

#include <QWidget>
#include <QFont>

class PinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PinWidget(QWidget *parent = 0);

    QString m_pinName;
    int m_pinNumber;
    bool m_checkable;

signals:
    void toggled(bool checked, int pinNumber);
    
public slots:
    void setState(bool checked);
    
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

    QFont m_pinNameFont;
    QFont m_pinNumberFont;
    int m_pinLineSize;
    bool m_checked;
};

#endif // PINWIDGET_H
