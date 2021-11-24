#ifndef VPNSERVERMODEL_H
#define VPNSERVERMODEL_H

#include <QAbstractTableModel>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include "vpnserver.h"

class VPNServerModel : public QAbstractTableModel
{
    const QString API_URL = "https://api.nordvpn.com/v1/servers?limit=%1&offset=%2";
    const int MAX_SERVERS_PER_REQUEST = 1000;
    const int COLUMN_COUNT = 5;

    Q_OBJECT

    public:
        explicit VPNServerModel(QObject *parent = nullptr);
        VPNServer getServer(int index);
        bool isUpdating();
        void clear();
        void reload();
        void append(const VPNServer &vpnServer);
        int columnCount(const QModelIndex &) const override { return COLUMN_COUNT; }
        int rowCount(const QModelIndex &parent = QModelIndex()) const { return servers.count(); };
        QVariant data(const QModelIndex &index, int role) const override {
            if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
            auto vpnServer = servers[index.row()];
            switch (index.column()) {
                case 0: return vpnServer.getCountry();
                case 1: return vpnServer.getCity();
                case 2: return vpnServer.getName();
                case 3: return vpnServer.getHost();
                case 4: return vpnServer.getTechnologies().join(", ");
                default: return {};
            };
        }
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
            if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
            switch (section) {
                case 0: return tr("serverTableHeaderCountry");
                case 1: return tr("serverTableHeaderCity");
                case 2: return tr("serverTableHeaderName");
                case 3: return tr("serverTableHeaderHost");
                case 4: return tr("serverTableHeaderTechnology");
                default: return {};
            }
        }
    private:
        QList<int> serverIDs;
        QList<VPNServer> servers;
        QString technology;
        QString protocol;
        bool obfuscate;
        bool updating;
        void loadSettings();
        void loadServers(int offset);
        void appendServer(VPNServer server);
};

#endif // VPNSERVERMODEL_H
