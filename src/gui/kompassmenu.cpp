#include "kompassmenu.h"

KompassMenu::KompassMenu(QStackedWidget *stack, QWidget *parent) : QFrame(parent)
{
    this->menuItems = new QMap<int, QWidget*>();
    this->chevronItems = new QMap<int, QLabel*>();
    this->stack = stack;
    this->setup();
}

void KompassMenu::setup()
{
    menuLayout = new QVBoxLayout();
    this->setLayout(menuLayout);
    this->setMinimumWidth(200);
    this->setMaximumWidth(200);
    this->setContentsMargins(0, 0, 0, 0);
}

void KompassMenu::appendMenuItem(QString icon, QString title, int stackIndex)
{
    QFont fntAwesome = QFont();
    fntAwesome.setFamily("FontAwesome");
    fntAwesome.setPixelSize(14);

    QLabel *lblIcon = new QLabel();
    lblIcon->setFont(fntAwesome);
    lblIcon->setText(icon);

    QLabel *lblTitle = new QLabel();
    lblTitle->setText(title);

    QLabel *lblChevron = new QLabel();
    lblChevron->setFont(fntAwesome);
    lblChevron->setText("\uf105");

    QHBoxLayout *itemLayout = new QHBoxLayout();
    itemLayout->setContentsMargins(20, 10, 5, 10);
    itemLayout->addWidget(lblIcon);
    itemLayout->addWidget(lblTitle);
    itemLayout->addStretch(1);
    itemLayout->addWidget(lblChevron);

    ClickableWidget *wxMenuItem = new ClickableWidget();
    wxMenuItem->setLayout(itemLayout);
    wxMenuItem->setAutoFillBackground(true);
    wxMenuItem->setStyleSheet(STYLE_DEFAULT);

    // register click handler
    QObject::connect(wxMenuItem, &ClickableWidget::clicked, this, [this, stackIndex] {
        select(stackIndex);
    });

    // add menu items to list
    menuItems->insert(stackIndex, wxMenuItem);
    chevronItems->insert(stackIndex, lblChevron);

    // add to UI
    menuLayout->addWidget(wxMenuItem);
    menuLayout->setContentsMargins(0, 0, 0, 0);
}

void KompassMenu::select(int stackIndex)
{
    for (QWidget *menuItem : *menuItems)
    {
        menuItem->setStyleSheet(STYLE_DEFAULT);
    }
    for (QLabel *chevronItem : *chevronItems)
    {
        chevronItem->setVisible(false);
    }

    QWidget *activeWidget = *menuItems->find(stackIndex);
    activeWidget->setStyleSheet(STYLE_ACTIVE);

    QLabel *activeChevron = *chevronItems->find(stackIndex);
    activeChevron->setVisible(true);

    stack->setCurrentIndex(stackIndex);
}
