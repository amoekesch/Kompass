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
        Settings();

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
        void setupUi();
        void setupData();
        void displaySuccess();
        void displayError(QString message);
        QString saveSettings(QStringList commands);
};
#endif // SETTINGS_H
