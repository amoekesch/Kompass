#ifndef SECTIONTITLE_H
#define SECTIONTITLE_H

#import <QHBoxLayout>
#import <QLabel>
#import <QWidget>

class SectionTitle : public QWidget
{
    Q_OBJECT

    public:
        explicit SectionTitle(QWidget *parent = nullptr);
        void setTitle(QString title);
        QLabel *lblTitle;
};

#endif // SECTIONTITLE_H