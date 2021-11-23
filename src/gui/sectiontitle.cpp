#include "sectiontitle.h"

SectionTitle::SectionTitle(QWidget *parent) : QWidget(parent)
{
    lblTitle = new QLabel();
    QString clr = lblTitle->palette().mid().color().name();
    lblTitle->setStyleSheet("font-weight: bold; font-size: 15px; color: " + clr + ";");

    QWidget *lnTitlePre = new QWidget;
    lnTitlePre->setFixedWidth(20);
    lnTitlePre->setFixedHeight(2);
    lnTitlePre->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lnTitlePre->setStyleSheet(QString("background-color: " + clr + ";"));

    QWidget *lnTitlePost = new QWidget;
    lnTitlePost->setFixedHeight(2);
    lnTitlePost->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    lnTitlePost->setStyleSheet(QString("background-color: " + clr + ";"));

    layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 15);
    layout->addWidget(lnTitlePre);
    layout->addWidget(lblTitle);
    layout->addWidget(lnTitlePost);
    layout->setStretch(2, 1);

    this->setLayout(layout);
}

void SectionTitle::setTitle(QString title)
{
    lblTitle->setText(title);
}

void SectionTitle::addControls(QList<QWidget*> controls)
{
    for (QWidget *control : controls)
    {
        layout->addWidget(control);
    }
}
