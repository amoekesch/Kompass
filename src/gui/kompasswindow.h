#ifndef KOMPASSWINDOW_H
#define KOMPASSWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>

class KompassWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit KompassWindow(QWidget *parent = nullptr);
        void closeEvent(QCloseEvent *event);

    private:
        QApplication *app;
};
#endif // KOMPASSWINDOW_H
