#ifndef INSTALLER_H
#define INSTALLER_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QSvgWidget>
#include "kompassmenu.h"
#include "../releasemonitor.h"

class Installer : public QDialog
{
    Q_OBJECT

    const int STEP_DOWNLOAD = 0;
    const int STEP_INSTALL = 1;
    const int STEP_RESTART = 0;

    public:
        explicit Installer(ReleaseMonitor *monitor, QWidget *parent = nullptr);

    private:
        int updateStep = STEP_DOWNLOAD;
        ReleaseMonitor *monitor;
        QSvgWidget *spinner;
        QPushButton *pbDownload;
        QPushButton *pbInstall;
        QPushButton *pbRestart;
        QPushButton *pbCancel;
        QLabel *lblInstructions;
        QLabel *lblStatus;
        QLabel *lblSudo;
        QLineEdit *txtSudo;
        void setupUi();
        void setupListeners();
        void cancel();
};
#endif // INSTALLER_H
