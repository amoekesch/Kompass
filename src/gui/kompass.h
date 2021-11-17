#ifndef KOMPASS_H
#define KOMPASS_H

#include <QAction>
#include <QErrorMessage>
#include <QGridLayout>
#include <QLabel>
#include <QLine>
#include <QLineEdit>
#include <QListView>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QStackedWidget>
#include <QStringListModel>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QWidgetAction>
#include <QSvgRenderer>
#include <QSvgWidget>
#include "clickablelabel.h"
#include "kompasswindow.h"
#include "sectiontitle.h"
#include "settings.h"
#include "togglebutton.h"
#include "../data/connectionresult.h"

QT_BEGIN_NAMESPACE
    namespace Ui { class Kompass;}
QT_END_NAMESPACE

class Kompass : public QObject
{
    Q_OBJECT

    public:
        Kompass();

    private:
        // application status constants
        static const int EXIT_CODE_NORMAL = 0;
        static const int EXIT_CODE_NOT_LOGGED_IN = 1;
        static const int STATUS_DISABLED = 10;
        static const int STATUS_CONNECTED = 11;
        static const int STATUS_DISCONNECTED = 12;
        static const int STATUS_CONNECTING = 13;
        static const int STATUS_DISCONNECTING = 14;

        // status variables
        int currentStatus = -1;
        bool updatingStatus = false;

        // ui components
        KompassWindow *ui;
        QSvgWidget *svgSpinner;
        QStackedWidget *stackMain;
        ClickableLabel *mnStatus;
        ClickableLabel *mnTypes;
        ClickableLabel *mnCountries;
        QListView *lstServersByType;
        QListView *lstServersByCountry;;
        ToggleButton *tbStatus;
        ToggleButton *tbConnectType;
        ToggleButton *tbConnectCountry;
        QLineEdit *txtStatusServer;
        QLineEdit *txtStatusCity;
        QLineEdit *txtStatusCountry;
        QLineEdit *txtStatusProtocol;
        QLineEdit *txtStatusUploaded;
        QLineEdit *txtStatusDownloaded;
        QLineEdit *txtStatusUptime;
        QLineEdit *txtFilterType;
        QLineEdit *txtFilterCountry;
        QLineEdit *txtUsername;
        QLineEdit *txtLicense;
        QPushButton *pbQuit;
        QPushButton *pbMinimize;
        QPushButton *pbSettings;

        // tray menu
        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;
        QAction *actionConnect;
        QAction *actionDisconnect;
        QAction *mnStatusConnection;
        QAction *mnStatusUploaded;
        QAction *mnStatusDownloaded;
        QAction *mnStatusUptime;

        // data models
        QVector<QString> *serverListByType;
        QStringListModel *serverListByTypeModel;
        QVector<QString> *serverListByCountry;
        QStringListModel *serverListByCountryModel;

        // methods
        void setupUi();
        void setupTray();
        void setupData();
        void setupDataAccount();
        void setupDataTypes();
        void setupDataCountries();
        void setupStatusMonitor();
        void toggleVpn(QStringList commands, bool connect);
        ConnectionResult* connectVpn(QStringList commands);
        ConnectionResult* disconnectVpn();
        void updateUi(int status, QString vpnStatus = nullptr);
        void showUi();
        void hideUi();
        void quit(int exitCode);
};
#endif // KOMPASS_H
