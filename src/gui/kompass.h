#ifndef KOMPASS_H
#define KOMPASS_H

#include <QAction>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLine>
#include <QLineEdit>
#include <QListView>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QStackedWidget>
#include <QStringListModel>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QWidgetAction>
#include <QSvgRenderer>
#include <QSvgWidget>
#include <QTableView>
#include "installer.h"
#include "kompassmenu.h"
#include "kompasswindow.h"
#include "sectiontitle.h"
#include "settings.h"
#include "togglebutton.h"
#include "trayicon.h"
#include "../releasemonitor.h"
#include "../data/connectionresult.h"
#include "../data/vpnserver.h"
#include "../data/vpnservermodel.h"

class Kompass : public QObject
{
    Q_OBJECT

    public:
        Kompass();
        void showUi();
        void hideUi();

    private:
        // application status constants
        static const int EXIT_CODE_NORMAL = 0;
        static const int EXIT_CODE_NOT_LOGGED_IN = 1;

        static const int STATUS_DISABLED = 10;
        static const int STATUS_CONNECTED = 11;
        static const int STATUS_DISCONNECTED = 12;
        static const int STATUS_CONNECTING = 13;
        static const int STATUS_DISCONNECTING = 14;

        static const int CONNECTION_TRIGGER_FASTEST = 0;
        static const int CONNECTION_TRIGGER_TYPE = 1;
        static const int CONNECTION_TRIGGER_COUNTRY = 2;
        static const int CONNECTION_TRIGGER_SERVER = 3;

        // status variables
        int currentStatus = -1;
        bool updatingStatus = false;

        // ui components
        KompassWindow *ui;
        KompassMenu *menu;
        QSvgWidget *svgSpinner;
        QSvgWidget *svgSpinnerServerRefresh;
        QStackedWidget *stackMain;
        QListView *lstServersByType;
        QListView *lstServersByCountry;
        QTableView *vwServers;
        ToggleButton *tbStatus;
        ToggleButton *tbConnectType;
        ToggleButton *tbConnectCountry;
        ToggleButton *tbConnectServer;
        QLineEdit *txtStatusServer;
        QLineEdit *txtStatusCity;
        QLineEdit *txtStatusCountry;
        QLineEdit *txtStatusProtocol;
        QLineEdit *txtStatusUploaded;
        QLineEdit *txtStatusDownloaded;
        QLineEdit *txtStatusUptime;
        QLineEdit *txtFilterType;
        QLineEdit *txtFilterCountry;
        QLineEdit *txtFilterServer;
        QLineEdit *txtUsername;
        QLineEdit *txtLicense;
        QLabel *lblVersionUpdate;
        QPushButton *pbQuit;
        QPushButton *pbMinimize;
        QPushButton *pbSettings;
        QPushButton *pbRefreshServers;
        SectionTitle *titleConnnectionServerList;

        // tray menu
        TrayIcon *trayIcon;
        QSystemTrayIcon *systemTrayIcon;
        QMenu *trayIconMenu;
        QAction *actionConnect;
        QAction *actionDisconnect;
        QAction *mnStatusConnection;
        QAction *mnStatusUploaded;
        QAction *mnStatusDownloaded;
        QAction *mnStatusUptime;

        // data models
        VPNServerModel *mdlServers;
        QSortFilterProxyModel *mdlServersProxy;
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
        void setupReleaseMonitor();
        void toggleVpn(QStringList commands, bool connect, int trigger);
        ConnectionResult* connectVpn(QStringList commands);
        ConnectionResult* disconnectVpn();
        void updateUi(int status, int trigger, QString vpnStatus = nullptr);
        void quit(int exitCode);
};
#endif // KOMPASS_H
