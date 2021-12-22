#include "installer.h"

Installer::Installer(ReleaseMonitor *monitor, QWidget *parent) : QDialog(parent)
{
    this->monitor = monitor;
    setupUi();
    setupListeners();
}

void Installer::setupUi()
{
    /**
     * -------------------------------------------
     *  set dialog basics
     */

    this->setWindowTitle(tr("dialogTitle"));
    this->setWindowIcon(QIcon(":/img/kompass.png"));
    this->setContentsMargins(0, 0, 0, 0);
    this->setMinimumSize(800, 430);
    this->setMaximumSize(800, 430);
    this->resize(800,350);
    this->setWindowModality(Qt::WindowModality::ApplicationModal);

    /**
     * -------------------------------------------
     *  Install Widget
     */

    QLabel *lblTitle = new QLabel(tr("lblTitle").arg(monitor->getUpdatedReleaseVersion()));
    lblTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: " + lblTitle->palette().highlight().color().name() + ";");

    lblInstructions = new QLabel(tr("lblInstructionsDownload").arg(monitor->getUpdatedReleaseVersion(), monitor->getUpdatedReleaseUrl()));
    lblInstructions->setWordWrap(true);
    lblInstructions->setOpenExternalLinks(true);
    lblInstructions->setTextInteractionFlags(Qt::TextBrowserInteraction);

    lblSudo = new QLabel(tr("lblSudo"));
    lblSudo->setWordWrap(true);
    lblSudo->setOpenExternalLinks(true);
    lblSudo->setVisible(false);

    lblStatus = new QLabel();
    lblStatus->setWordWrap(true);
    lblStatus->setVisible(false);
    lblStatus->setStyleSheet("color: rgb(206, 58, 58)");

    txtSudo = new QLineEdit();
    txtSudo->setEchoMode(QLineEdit::EchoMode::Password);
    txtSudo->setVisible(false);

    QVBoxLayout *layoutInstall = new QVBoxLayout();
    layoutInstall->addWidget(lblTitle);
    layoutInstall->addSpacing(30);
    layoutInstall->addWidget(lblInstructions);
    layoutInstall->addSpacing(10);
    layoutInstall->addWidget(lblSudo);
    layoutInstall->addWidget(txtSudo);
    layoutInstall->addSpacing(30);
    layoutInstall->addWidget(lblStatus);
    layoutInstall->addStretch(1);

    QWidget *wInstall = new QWidget();
    wInstall->setLayout(layoutInstall);

    /**
     * -------------------------------------------
     *  add panel into stack
     */

    QStackedWidget *stackInstall = new QStackedWidget();
    stackInstall->setContentsMargins(20, 25, 20, 20);
    stackInstall->addWidget(wInstall);

    /**
     * -------------------------------------------
     *  create menu
     */
    KompassMenu *menu = new KompassMenu(stackInstall);
    menu->appendMenuItem("\uf0a1", tr("mnInstall"), 0);
    menu->select(0);

    QVBoxLayout *layoutMenu = new QVBoxLayout();
    layoutMenu->setContentsMargins(0, 28, 0, 20);
    layoutMenu->addWidget(menu);
    layoutMenu->addStretch(1);

    QWidget *wxMenuWrapper = new QWidget();
    wxMenuWrapper->setMinimumWidth(200);
    wxMenuWrapper->setMaximumWidth(200);
    wxMenuWrapper->setAutoFillBackground(true);
    wxMenuWrapper->setStyleSheet("background: " + this->palette().light().color().name() + ";");
    wxMenuWrapper->setLayout(layoutMenu);

    /**
     * -------------------------------------------
     *  create button controls
     */

    spinner = new QSvgWidget();
    spinner->load(QString(":/img/spinner.svg"));
    spinner->setFixedWidth(18);
    spinner->setFixedHeight(18);
    spinner->setGeometry(0, 0, 18, 18);
    spinner->setStyleSheet("background: transparent;");
    spinner->setVisible(false);

    pbCancel = new QPushButton();
    pbCancel->setText(tr("pbCancel"));
    pbCancel->setIcon(QIcon(":/img/close.png"));
    pbCancel->setMinimumWidth(180);
    QObject::connect(pbCancel, &QPushButton::clicked, [this]()
    {
        this->cancel();
    });

    pbDownload = new QPushButton();
    pbDownload->setText(tr("pbDownload"));
    pbDownload->setIcon(QIcon(":/img/arrow-down.png"));
    pbDownload->setMinimumWidth(180);
    QObject::connect(pbDownload, &QPushButton::clicked, [this]()
    {
        spinner->setVisible(true);
        pbDownload->setEnabled(false);
        lblStatus->setText("");
        lblStatus->setVisible(false);
        monitor->download();
    });

    pbInstall = new QPushButton();
    pbInstall->setText(tr("pbInstall"));
    pbInstall->setIcon(QIcon(":/img/cogs.png"));
    pbInstall->setMinimumWidth(180);
    pbInstall->setVisible(false);
    QObject::connect(pbInstall, &QPushButton::clicked, [this]()
    {
        spinner->setVisible(true);
        pbInstall->setEnabled(false);
        lblStatus->setText("");
        lblStatus->setVisible(false);
        monitor->install(txtSudo->text());
    });

    pbRestart = new QPushButton();
    pbRestart->setText(tr("pbRestart"));
    pbRestart->setIcon(QIcon(":/img/restart.png"));
    pbRestart->setMinimumWidth(180);
    pbRestart->setVisible(false);
    QObject::connect(pbRestart, &QPushButton::clicked, [this]()
    {
        pbRestart->setEnabled(false);
        spinner->setVisible(true);
        lblStatus->setText("");
        lblStatus->setVisible(false);
        monitor->restart();
    });

    QHBoxLayout *layoutDialogControls = new QHBoxLayout();
    layoutDialogControls->setContentsMargins(20, 0, 20, 15);
    layoutDialogControls->addStretch(1);
    layoutDialogControls->addWidget(spinner);
    layoutDialogControls->addSpacing(5);
    layoutDialogControls->addWidget(pbRestart);
    layoutDialogControls->addWidget(pbInstall);
    layoutDialogControls->addWidget(pbDownload);
    layoutDialogControls->addSpacing(5);
    layoutDialogControls->addWidget(pbCancel);

    /**
     * -------------------------------------------
     *  create dialog layout and add all components
     */

    QGridLayout *layoutMain = new QGridLayout();
    layoutMain->setContentsMargins(0, 0, 0, 0);
    layoutMain->addWidget(wxMenuWrapper, 0, 0, 3, 1);
    layoutMain->addWidget(stackInstall, 0, 1, 1, 1);
    layoutMain->addItem(layoutDialogControls, 2, 1, 1, 1);
    layoutMain->rowStretch(2);
    layoutMain->columnStretch(1);

    this->setLayout(layoutMain);
}

void Installer::setupListeners()
{
    // exception listener
    connect(monitor, &ReleaseMonitor::updateCanceled, [this] {
        QString err = monitor->getUpdateError();

        // handle dialog status
        lblSudo->setVisible(updateStep == STEP_INSTALL && monitor->needsSudo());
        txtSudo->setVisible(updateStep == STEP_INSTALL && monitor->needsSudo());
        txtSudo->setText("");
        pbDownload->setEnabled(true);
        pbDownload->setVisible(updateStep == STEP_DOWNLOAD);
        pbInstall->setEnabled(true);this->
        pbInstall->setVisible(updateStep == STEP_INSTALL);
        pbRestart->setEnabled(true);
        pbRestart->setVisible(updateStep == STEP_RESTART);
        spinner->setVisible(false);

        lblStatus->setText(err);
        lblStatus->setVisible(true);
    });

    // download complete listener
    connect(monitor, &ReleaseMonitor::updateDownloaded, [this] {
        updateStep = STEP_INSTALL;

        pbDownload->setVisible(false);
        pbRestart->setVisible(false);
        txtSudo->setText("");
        lblSudo->setVisible(monitor->needsSudo());
        txtSudo->setVisible(monitor->needsSudo());

        lblInstructions->setText(tr("lblInstructionsInstall").arg(monitor->getUpdatedReleaseVersion())),
        pbInstall->setVisible(true);

        spinner->setVisible(false);
    });

    // installation complete listener
    connect(monitor, &ReleaseMonitor::updateInstalled, [this] {
        updateStep = STEP_RESTART;

        pbDownload->setVisible(false);
        pbInstall->setVisible(false);
        txtSudo->setText("");
        lblSudo->setVisible(false);
        txtSudo->setVisible(false);

        lblInstructions->setText(tr("lblInstructionsRestart").arg(monitor->getUpdatedReleaseVersion())),
        pbRestart->setVisible(true);

        spinner->setVisible(false);
    });
}

void Installer::cancel()
{
    this->monitor->clean();
    this->close();
}
