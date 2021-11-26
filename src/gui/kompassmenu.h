#ifndef KOMPASSMENU_H
#define KOMPASSMENU_H

#include <QIcon>
#include <QLabel>
#include <QMap>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include "clickablewidget.h"

class KompassMenu : public QFrame
{
    Q_OBJECT

    const QString STYLE_DEFAULT = "font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";";
    const QString STYLE_ACTIVE = "font-weight: normal; color: " + this->palette().light().color().name() + "; background: " + this->palette().highlight().color().name() + ";";

    public:
        explicit KompassMenu(QStackedWidget *stack, QWidget *parent = nullptr);
        void appendMenuItem(QString icon, QString title, int stackIndex);
        void select(int stackIndex);

    private:
        QStackedWidget *stack;
        QMap<int, QWidget*> *menuItems;
        QMap<int, QLabel*> *chevronItems;
        QVBoxLayout *menuLayout;
        void setup();
};

#endif // KOMPASSMENU_H
