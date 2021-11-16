#include "kompasswindow.h"

KompassWindow::KompassWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowIcon(QIcon::fromTheme("compass"));
    this->setMinimumSize(1000, 680);
    this->setMaximumSize(1000, 680);
    this->setFixedSize(1000, 680);
    this->resize(1000, 680);
}

void KompassWindow::closeEvent(QCloseEvent *event)
{
    this->setVisible(false);
    event->ignore();
}
