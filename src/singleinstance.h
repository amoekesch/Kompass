#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include "./gui/kompass.h"

class SingleInstance : public QObject
{
    Q_OBJECT

    public:
        explicit SingleInstance(Kompass *kompass);
        ~SingleInstance();
        void listen(QString name);
        bool hasInstance(QString name, QStringList args);

    private:
        QString name;
        QLocalSocket *localSocket;
        QLocalServer localServer;
        Kompass *kompass;

    signals:
        void newInstance();

    public slots:
        void newConnection();
        void readyRead();
};

#endif // SINGLEINSTANCE_H
