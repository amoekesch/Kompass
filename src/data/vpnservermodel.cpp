#include "vpnservermodel.h"

VPNServerModel::VPNServerModel(QObject *parent) : QAbstractTableModel(parent)
{
    this->reload();
}

void VPNServerModel::loadServers(int offset)
{
    // create request
    QString url = QString(API_URL).arg(MAX_SERVERS_PER_REQUEST).arg((offset -1) * MAX_SERVERS_PER_REQUEST);
    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // perform request
    QNetworkAccessManager *nam = new QNetworkAccessManager();
    QNetworkReply *reply = nam->get(*request);

    // handle completion
    connect(reply, &QNetworkReply::finished, this, [this, offset, reply] {
        reply->deleteLater();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonArray jsonResponse = jsonDoc.array();


        for (QJsonValue serverDetails : jsonResponse) {
            bool validServer = false;
            VPNServer server = VPNServer(serverDetails.toObject());
            QStringList serverTechnologies = server.getTechnologies();
            QString techString = server.getTechnologies().join("|").toUpper();

            if (this->technology.toUpper().contains("LYNX") && techString.contains("WIRE"))
            {
                validServer = true;
            }
            else if (techString.contains(this->protocol))
            {
                for (QString serverTechnology : server.getTechnologies())
                {
                    if ( serverTechnology.toUpper().contains(protocol) &&
                         serverTechnology.toUpper().contains(technology) &&
                         serverTechnology.toUpper().contains("OBFUSCATE") &&
                         obfuscate
                       )
                    {
                        validServer = true;
                        break;
                    }
                    if ( serverTechnology.toUpper().contains(protocol) &&
                         serverTechnology.toUpper().contains(technology) &&
                        !serverTechnology.toUpper().contains("OBFUSCATE") &&
                        !obfuscate
                       )
                    {
                        validServer = true;
                        break;
                    }
                }
            }

            if (!serverIDs.contains(server.getId()) && validServer)
            {
                appendServer(server);
            }
        }

        // keeprequesting until less than max count
        if (jsonResponse.count() == MAX_SERVERS_PER_REQUEST)
        {
            loadServers(offset + 1);
        }
        else
        {
            updating = false;
            emit dataChanged(index(0, 0), index(rowCount(), COLUMN_COUNT));
        }
    });
}

void VPNServerModel::appendServer(VPNServer server)
{
    beginInsertRows({}, this->servers.count(), this->servers.count());
    this->serverIDs.append(server.getId());
    this->servers.append(server);
    endInsertRows();
}

VPNServer VPNServerModel::getServer(int index)
{
    return this->servers[index];
}

bool VPNServerModel::isUpdating()
{
    return this->updating;
}

void VPNServerModel::clear()
{
    beginRemoveRows({}, 0, this->servers.count() - 1);
    this->serverIDs.clear();
    this->servers.clear();
    endRemoveRows();
}

void VPNServerModel::reload()
{
    updating = true;
    this->clear();
    this->loadSettings();
    this->loadServers(0);
}

void VPNServerModel::loadSettings()
{
    QProcess *command = new QProcess();
    command->start("nordvpn", QStringList() << "settings");
    command->waitForFinished();
    QString output = command->readAllStandardOutput();

    // parse output
    QStringList lines = output.trimmed().split(QString("\n"));
    for (QString line : lines)
    {
        if (line.trimmed().toLower().contains("technology"))
        {
            technology = line.mid(line.indexOf(":") + 1).trimmed();
        }
        else if (line.trimmed().toLower().contains("protocol"))
        {
            protocol = line.mid(line.indexOf(":") + 1).trimmed();
        }
        else if (line.trimmed().toLower().contains("obfuscate"))
        {
            obfuscate = line.mid(line.indexOf(":") + 1).trimmed() == "enabled";
        }
    }
}
