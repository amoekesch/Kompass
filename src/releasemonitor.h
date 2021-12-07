#ifndef RELEASEMONITOR_H
#define RELEASEMONITOR_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QRegularExpression>
#include <QTimer>

class ReleaseMonitor : public QObject
{
    Q_OBJECT

    const int INTERVAL = 3 * 60 * 60 * 1000; // 3 hrs
    const QString API_URL = "https://api.github.com/repos/amoekesch/Kompass/releases/latest";

    public:
        ReleaseMonitor();
        void start(QString currentRelease);
        void stop();
        void evaluate();
        QString getUpdatedReleaseVersion();
        QString getUpdatedReleaseUrl();

    private:
        QTimer *monitor;
        QString currentRelease;
        QString updatedReleaseVersion;
        QString updatedReleaseUrl;
        void init();

    signals:
        void updateAvailable();
};

#endif // RELEASEMONITOR_H
