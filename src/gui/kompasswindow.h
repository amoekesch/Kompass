#ifndef KOMPASSWINDOW_H
#define KOMPASSWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
    namespace Ui { class KompassWindow;}
QT_END_NAMESPACE

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
