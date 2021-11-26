#ifndef CLICKABLEWIDGET_H
#define CLICKABLEWIDGET_H

#include <QFrame>
#include <QMouseEvent>
#include <QWidget>

class ClickableWidget : public QFrame
{
    Q_OBJECT

    public:
        explicit ClickableWidget(QWidget *parent = nullptr);
        ~ClickableWidget();

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLEWIDGET_H
