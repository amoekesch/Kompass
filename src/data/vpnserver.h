#ifndef VPNSERVER_H
#define VPNSERVER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

class VPNServer
{
    public:
        explicit VPNServer(QJsonObject jsonData);
        int getId();
        QString getName();
        QString getHost();
        QString getCity();
        QString getCountry();
        QStringList getTechnologies();
        QJsonObject toJson();

    private:
        int id;
        QString name;
        QString host;
        QString city;
        QString country;
        QStringList technologies;
        QJsonObject jsonData;
};
#endif // VPNSERVER_H
