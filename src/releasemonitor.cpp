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

QString ReleaseMonitor::getUpdateError()
{
    return this->updateError;
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
                    emit updateAvailable();
                }
            }
        }
    });
}

void ReleaseMonitor::download()
{
    // check URL
    if (this->updatedReleaseUrl == nullptr || this->updatedReleaseUrl.length() == 0) {
        this->updateError = tr("msgErrorNoRelease");
        emit updateCanceled();
        return;
    }

    // create download request
    QNetworkRequest *request = new QNetworkRequest(QUrl::fromUserInput(this->updatedReleaseUrl));
    QNetworkAccessManager *nam = new QNetworkAccessManager();
    QNetworkReply *reply = nam->get(*request);

    // handle completion
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]
    {
        if (!reply->error())
        {
            // create a unique filename
            this->updateFile = DOWNLOAD_PATH +
                                    "/Kompass_" +
                                    updatedReleaseVersion + "_" +
                                    QString::number(QDateTime::currentMSecsSinceEpoch());

            // store the downloaded file
            QFile file(updateFile);
            if (file.open(QIODevice::ReadWrite)) {
                file.write(reply->readAll());
                file.close();
                emit updateDownloaded();
            }
            else
            {
                this->updateError = tr("msgErrorReadOnlyDirectory").arg(DOWNLOAD_PATH);
                emit updateCanceled();
            }
            reply->deleteLater();
        }
        else
        {
            this->updateError = tr("msgErrorDownloading").arg(updatedReleaseVersion, reply->errorString());
            emit updateCanceled();
            reply->deleteLater();
        }
    });
}

void ReleaseMonitor::install(QString password)
{
    // define extraction directory and create it
    QString foldername = this->updateFile + "_extracted";
    QDir srcDir(foldername);
    srcDir.mkpath(foldername);

    // extract downloaded file (download already makes sure folder is writable)
    QProcess *commandExtract = new QProcess();
    commandExtract->start("tar", QStringList() << "-xf" << updateFile << "-C" << foldername);
    commandExtract->waitForFinished();

    // run install
    QFile *srcFile = new QFile(foldername + "/Kompass/Kompass");
    QFile *dstFile = new QFile(EXECUTABLE_PATH);
    bool result = false;
    if (needsSudo())
    {
        result = installSudo(password, srcFile, dstFile);
    }
    else
    {
        result = installNormal(srcFile, dstFile);
    }

    // do some clean-up if things completed successfully and inform the user
    if (result)
    {
        // clean and inform the UI
        clean();
        emit updateInstalled();
    }
}

bool ReleaseMonitor::installNormal(QFile *srcFile, QFile *dstFile)
{
    bool deleted = dstFile->remove();
    if (!deleted) {
        this->updateError = tr("msgErrorDeletePrevious").arg(updatedReleaseVersion);
        emit updateCanceled();
        return false;
    }

    bool copied = srcFile->copy(dstFile->fileName());
    if (!copied) {
        this->updateError = tr("msgErrorOverwrite").arg(updatedReleaseVersion);
        emit updateCanceled();
        return false;
    }
}

bool ReleaseMonitor::installSudo(QString password, QFile *srcFile, QFile *dstFile)
{
    // delete previous
    QString rm = "echo '" + password + "' | sudo -S \\rm '" + dstFile->fileName() + "'";
    QProcess *commandDelete = new QProcess();
    commandDelete->start("/bin/sh", QStringList() << "-c" << rm);
    commandDelete->waitForFinished();

    // verify
    if (dstFile->exists()) {
        this->updateError = tr("msgErrorDeletePreviousSudo").arg(updatedReleaseVersion);
        emit updateCanceled();
        return false;
    }

    // copy new version
    QString cp = "echo '" + password + "' | sudo -S \\cp '" + srcFile->fileName() + "' '" + dstFile->fileName() + "'";
    QProcess *commandCopy = new QProcess();
    commandCopy->start("/bin/sh", QStringList() << "-c" << cp);
    commandCopy->waitForFinished();

    // verify
    if (!dstFile->exists()) {
        this->updateError = tr("msgErrorOverwriteSudo").arg(updatedReleaseVersion);
        emit updateCanceled();
        return false;
    }

    return true;
}

void ReleaseMonitor::restart()
{
    // some housekeeping
    this->clean();

    // start new application
    QProcess restart;
    restart.startDetached(EXECUTABLE_PATH, QStringList());

    // quit current
    qApp->quit();
}

void ReleaseMonitor::clean()
{
    if (updateFile != nullptr && updateFile.length() > 0)
    {
        // delete extracted folder / ignore output
        QString foldername = this->updateFile + "_extracted";
        QDir srcDir(foldername);
        srcDir.removeRecursively();

        // delete downloaded file / ignore output
        QFile downloadedFile(this->updateFile);
        downloadedFile.remove();
    }
}

bool ReleaseMonitor::needsSudo()
{
    QFileInfo pathInfo(APPLICATION_PATH);
    return !pathInfo.isWritable();
}

bool ReleaseMonitor::isSudoer()
{
    QProcess *commandGroups = new QProcess();
    commandGroups->start("groups");
    commandGroups->waitForFinished();
    QString outputGroups = commandGroups->readAllStandardOutput();
    if (outputGroups != nullptr && outputGroups.length())
    {
        QStringList lines = outputGroups.trimmed().replace(QRegularExpression("[\\n\\r]"), " ").split(QString(" "));
        return lines.contains("sudo", Qt::CaseInsensitive);
    }
    return false;
}
