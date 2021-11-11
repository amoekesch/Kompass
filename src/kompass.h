#ifndef KOMPASS_H
#define KOMPASS_H

#include <QAction>
#include <QErrorMessage>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QStatusBar>
#include <QStringListModel>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QWidgetAction>

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

    // status variables
    int currentStatus = -1;
    bool updatingStatus = false;

    // ui components
    QMainWindow *ui;
    QListView *lstServersByType;
    QListView *lstServersByCountry;
    QLabel *lblStatusConnection;
    QLabel *lblStatusServer;
    QLabel *lblStatusCity;
    QLabel *lblStatusCountry;
    QLabel *lblStatusProtocol;
    QLabel *lblStatusUploaded;
    QLabel *lblStatusDownloaded;
    QLabel *lblStatusUptime;
    QLineEdit *txtStatus;
    QLineEdit *txtFilterType;
    QLineEdit *txtFilterCountry;
    QPushButton *pbDisconnect;
    QPushButton *pbConnectType;
    QPushButton *pbConnectCountry;

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
    void setupStatusMonitor();
    void connectVpn(QString server = nullptr);
    void disconnectVpn();
    void updateUi(int status, QString vpnStatus = nullptr);
    void showUi();
    void hideUi();
    void quit(int exitCode);
};
#endif // KOMPASS_H
