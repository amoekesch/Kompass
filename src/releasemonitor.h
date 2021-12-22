#ifndef RELEASEMONITOR_H
#define RELEASEMONITOR_H

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QProcess>
#include <QRegularExpression>
#include <QTimer>
#include <QUrl>

class ReleaseMonitor : public QObject
{
    Q_OBJECT

    const int INTERVAL = 3 * 60 * 60 * 1000; // 3 hrs
    const QString API_URL = "https://api.github.com/repos/amoekesch/Kompass/releases/latest";
    const QString DOWNLOAD_PATH = "/tmp";
    const QString APPLICATION_PATH = QCoreApplication::applicationDirPath();
    const QString EXECUTABLE_PATH = QCoreApplication::applicationFilePath();

    public:
        ReleaseMonitor();
        void start(QString currentRelease);
        void stop();
        void evaluate();
        void download();
        void install(QString password);
        void restart();
        void clean();
        bool needsSudo();
        bool isSudoer();
        QString getUpdatedReleaseVersion();
        QString getUpdatedReleaseUrl();
        QString getUpdateError();

    private:
        QTimer *monitor;
        QString currentRelease;
        QString updatedReleaseVersion;
        QString updatedReleaseUrl;
        QString updateError;
        QString updateFile;
        void init();
        bool installSudo(QString password, QFile *srcFile, QFile *dstFile);
        bool installNormal(QFile *srcFile, QFile *dstFile);

    signals:
        void updateAvailable();
        void updateDownloaded();
        void updateInstalled();
        void updateCanceled();
};

#endif // RELEASEMONITOR_H
