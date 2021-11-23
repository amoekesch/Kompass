#ifndef SECTIONTITLE_H
#define SECTIONTITLE_H

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class SectionTitle : public QWidget
{
    Q_OBJECT

    public:
        explicit SectionTitle(QWidget *parent = nullptr);
        void setTitle(QString title);
        void addControls(QList<QWidget*> controls);

    private:
        QLabel *lblTitle;
        QHBoxLayout *layout;
};

#endif // SECTIONTITLE_H
