#ifndef SETTINGS_H
#define SETTINGS_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "kompassmenu.h"
#include "togglebutton.h"

class Settings : public QDialog
{
    Q_OBJECT

    public:
        explicit Settings (QWidget *parent = nullptr);

    private:
        QStackedWidget *stackSettings;
        KompassMenu *menu;
        QComboBox *cmbTechnology;
        QComboBox *cmbProtocol;
        ToggleButton *cbFirewall;
        ToggleButton *cbKillswitch;
        ToggleButton *cbCybersec;
        ToggleButton *cbObfuscate;
        ToggleButton *cbNotify;
        ToggleButton *cbAutoconnect;
        ToggleButton *cbIPv6;
        QString technology;
        QString protocol;
        bool firewall;
        bool killswitch;
        bool cybersec;
        bool obfuscate;
        bool autoconnect;
        bool ipv6;
        bool notify;
        void setupUi();
        void setupData();
        void displayStatus(QString errorMessage = nullptr);
        void setEnabled(bool status);
        QString saveSettings(QStringList commands);
};
#endif // SETTINGS_H
