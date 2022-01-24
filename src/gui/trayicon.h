#ifndef TRAYICON_H
#define TRAYICON_H

#include <QIcon>
#include <QMap>
#include <QObject>
#include <QSettings>

class TrayIcon : public QObject
{
    Q_OBJECT

    public:
        const QIcon iconBlue = QIcon(":/img/vpn-status-blue.png");
        const QIcon iconGreen = QIcon(":/img/vpn-status-green.png");
        const QIcon iconRed = QIcon(":/img/vpn-status-red.png");
        const QIcon iconWhite = QIcon(":/img/vpn-status-white.png");
        const QIcon iconBlack = QIcon(":/img/vpn-status-black.png");
        const QIcon iconGrey25 = QIcon(":/img/vpn-status-grey25.png");
        const QIcon iconGrey50 = QIcon(":/img/vpn-status-grey50.png");
        const QIcon iconGrey75 = QIcon(":/img/vpn-status-grey75.png");

        const int indexBlue = 0;
        const int indexGreen = 1;
        const int indexRed = 2;
        const int indexWhite = 3;
        const int indexBlack = 4;
        const int indexGrey25 = 5;
        const int indexGrey50 = 6;
        const int indexGrey75 = 7;

        explicit TrayIcon(QObject *parent = nullptr);
        QIcon getIcon(bool connected);
        QMap<int, QIcon> getIcons();
        int getCurrentIndexConnected();
        int getCurrentIndexDisconnected();
        void setCurrentIndexConnected(int index);
        void setCurrentIndexDisconnected(int index);

    private:
        const QString CFG_FOLDER = "de.moekesch.Kompass";
        const QString CFG_FILE = "Kompass";
        const QString KEY_CONNECTED = "iconConnected";
        const QString KEY_DISCONNECTED = "iconDisconnected";

        int defaultIndexConnected = indexBlue;
        int defaultIndexDisconnected = indexGrey50;
        int currentIndexConnected;
        int currentIndexDisconnected;

        QMap<int, QIcon> *icons;

        void storeConfiguration();
        void readConfiguration();
        void initialize();
};

#endif // TRAYICON_H
