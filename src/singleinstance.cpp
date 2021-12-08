#include "singleinstance.h"

SingleInstance::SingleInstance(Kompass *kompass) : QObject(kompass)
{
    this->kompass = kompass;
    connect(&localServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

SingleInstance::~SingleInstance()
{
    if (name != nullptr)
    {
        localServer.removeServer(name);
    }
    localServer.close();
}

void SingleInstance::listen(QString name)
{
    this->name = name;
    this->localServer.removeServer(name);
    this->localServer.listen(name);
}

bool SingleInstance::hasInstance(QString name, QStringList args)
{
    QLocalSocket socket;
    socket.connectToServer(name, QLocalSocket::ReadWrite);
    if (socket.waitForConnected())
    {
        QByteArray buffer;
        foreach(QString arg, args)
        {
            buffer.append(QString(arg + "\n").toUtf8());
        }
        socket.write(buffer);
        socket.waitForBytesWritten();
        return true;
    }
    return false;
}

void SingleInstance::newConnection()
{
    emit newInstance();

    localSocket = localServer.nextPendingConnection();
    connect(localSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void SingleInstance::readyRead()
{
    // read arguments / ignore for now
    QByteArray args = localSocket->readAll();
    kompass->showUi();
    localSocket->deleteLater();
}
