#include "releasemonitor.h"

ReleaseMonitor::ReleaseMonitor()
{
    init();
}

void ReleaseMonitor::init()
{
    monitor = new QTimer();
    QObject::connect(monitor, &QTimer::timeout, this, &ReleaseMonitor::evaluate);
}

void ReleaseMonitor::start(QString currentRelease)
{
    this->currentRelease = currentRelease;

    if (monitor->isActive())
    {
        monitor->stop();
    }
    monitor->start(INTERVAL);
}

void ReleaseMonitor::stop()
{
    if (monitor->isActive())
    {
        monitor->stop();
    }
}

QString ReleaseMonitor::getUpdatedReleaseVersion()
{
    return this->updatedReleaseVersion;
}

QString ReleaseMonitor::getUpdatedReleaseUrl()
{
    return this->updatedReleaseUrl;
}

void ReleaseMonitor::evaluate()
{
    // create request
    QNetworkRequest *request = new QNetworkRequest(QUrl(API_URL));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // perform request
    QNetworkAccessManager *nam = new QNetworkAccessManager();
    QNetworkReply *reply = nam->get(*request);

    // handle completion
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]
    {
        reply->deleteLater();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        if (jsonDoc.isObject())
        {
            QJsonObject jsonRelease = jsonDoc.object();
            updatedReleaseVersion = jsonRelease.find("tag_name")->toString("");
            if (updatedReleaseVersion != nullptr && updatedReleaseVersion.length() > 0)
            {
                // clean strings
                QRegularExpression rxNumeric = QRegularExpression("[A-Za-z\\s]");
                QRegularExpression rxDecimal = QRegularExpression("[\\D]");
                QString currentVersion = currentRelease.remove(rxNumeric);
                QString latestVersion = updatedReleaseVersion.remove(rxNumeric);
                if (currentVersion.count(".") > 1)
                {
                    currentVersion = currentVersion.left(currentVersion.indexOf(".") + 1) + currentVersion.mid(currentVersion.indexOf(".") + 1).remove(rxDecimal);
                }
                if (latestVersion.count(".") > 1)
                {
                    latestVersion = latestVersion.left(latestVersion.indexOf(".") + 1) + latestVersion.mid(latestVersion.indexOf(".") + 1).remove(rxDecimal);
                }

                // compare
                if (latestVersion.toDouble() > currentVersion.toDouble())
                {
                    // extract the download URL
                    if (jsonRelease.find("assets")->isArray())
                    {
                        QJsonArray assets = jsonRelease.find("assets")->toArray();
                        if (assets.size() > 0)
                        {
                            QJsonObject asset = assets.at(0).toObject();
                            updatedReleaseUrl = asset.find("browser_download_url")->toString("");
                        }
                    }

                    // inform the UI
                    emit(updateAvailable());
                }
            }
        }
    });
}
