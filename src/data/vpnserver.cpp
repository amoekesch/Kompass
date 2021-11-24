#include "vpnserver.h"

VPNServer::VPNServer(QJsonObject jsonData)
{
    QJsonArray jsonLocations = jsonData.find("locations")->toArray();
    QJsonArray jsonTechnologies = jsonData.find("technologies")->toArray();

    QJsonObject jsonLocation = jsonLocations.at(0).toObject();
    QJsonObject jsonCountry = jsonLocation.find("country")->toObject();
    QJsonObject jsonCity = jsonCountry.find("city")->toObject();

    this->jsonData = jsonData;
    id = jsonData.find("id")->toInt();
    name = jsonData.find("name")->toString().trimmed();
    host = jsonData.find("hostname")->toString().trimmed();
    country = jsonCountry.find("name")->toString().trimmed();
    city = jsonCity.find("name")->toString().trimmed();

    technologies = QStringList();
    for(int i = 0; i <= jsonTechnologies.count(); i++)
    {
        QString technology = jsonTechnologies.at(i).toObject().find("name")->toString().trimmed();
        if (technology.length() > 0)
        {
            technologies.append(technology);
        }
    }
}

QJsonObject VPNServer:: toJson()
{
    return jsonData;
}

int VPNServer::getId()
{
    return id;
}

QString VPNServer::getName()
{
    return name;
}

QString VPNServer::getHost()
{
    return host;
}

QString VPNServer::getCity()
{
    return city;
}

QString VPNServer::getCountry()
{
    return country;
}

QStringList VPNServer::getTechnologies()
{
    return technologies;
}
