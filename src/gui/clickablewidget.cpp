#include "clickablewidget.h"

ClickableWidget::ClickableWidget(QWidget *parent) : QFrame(parent)
{
}

ClickableWidget::~ClickableWidget()
{
}

void ClickableWidget::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}
