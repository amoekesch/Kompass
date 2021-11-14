#include "kompass.h"

/**
 * Constructor
 * @brief Kompass::Kompass
 * @param parent
 */
Kompass::Kompass()
{
    currentStatus = STATUS_DISCONNECTED;

    // Dialog Setup
    this->setupUi();
    this->setupTray();
    this->setupData();
    this->setupStatusMonitor();
}

/**
 * setup controls and event listeners
 * @brief Kompass::setup
 */
void Kompass::setupUi()
{
    /**
     * -------------------------------------------
     *  create status panel
     */

    txtStatus = new QLineEdit();
    txtStatus->setText(tr("statusDisconnected"));
    txtStatus->setEnabled(false);

    pbDisconnect = new QPushButton();
    pbDisconnect->setEnabled(false);
    pbDisconnect->setText(tr("pbDisconnect"));
    pbDisconnect->setStyleSheet("font-weight: normal;");
    pbDisconnect->setMinimumWidth(140);
    pbDisconnect->setIcon(QIcon::fromTheme("network-vpn-disconnected-symbolic"));
    QObject::connect(pbDisconnect, &QPushButton::clicked, [this]()
    {
        this->disconnectVpn();
    });

    QLabel *lblServer = new QLabel(tr("lblServer"));
    lblServer->setStyleSheet("font-weight: normal;");
    QLabel *lblCity = new QLabel(tr("lblCity"));
    lblCity->setStyleSheet("font-weight: normal;");
    QLabel *lblCountry = new QLabel(tr("lblCountry"));
    lblCountry->setStyleSheet("font-weight: normal;");
    QLabel *lblProtocol = new QLabel(tr("lblProtocol"));
    lblProtocol->setStyleSheet("font-weight: normal;");
    QLabel *lblUploaded = new QLabel(tr("lblUploaded"));
    lblUploaded->setStyleSheet("font-weight: normal;");
    QLabel *lblDownloaded = new QLabel(tr("lblDownloaded"));
    lblDownloaded->setStyleSheet("font-weight: normal;");
    QLabel *lblUptime = new QLabel(tr("lblUptime"));
    lblUptime->setStyleSheet("font-weight: normal;");

    lblStatusServer = new QLabel("--");
    lblStatusServer->setStyleSheet("font-weight: normal;");
    lblStatusCity = new QLabel("--");
    lblStatusCity->setStyleSheet("font-weight: normal;");
    lblStatusCountry = new QLabel("--");
    lblStatusCountry->setStyleSheet("font-weight: normal;");
    lblStatusProtocol = new QLabel("--");
    lblStatusProtocol->setStyleSheet("font-weight: normal;");
    lblStatusUploaded = new QLabel("--");
    lblStatusUploaded->setStyleSheet("font-weight: normal;");
    lblStatusDownloaded = new QLabel("--");
    lblStatusDownloaded->setStyleSheet("font-weight: normal;");
    lblStatusUptime = new QLabel("--");
    lblStatusUptime->setStyleSheet("font-weight: normal;");

    QGridLayout *layoutStatus = new QGridLayout();
    layoutStatus->addWidget(txtStatus, 0, 0, 1, 2);
    layoutStatus->addWidget(pbDisconnect, 0, 2);
    layoutStatus->addWidget(lblServer, 1, 0);
    layoutStatus->addWidget(lblStatusServer, 1, 1, 1, 2);
    layoutStatus->addWidget(lblCity, 2, 0);
    layoutStatus->addWidget(lblStatusCity, 2, 1, 1, 2);
    layoutStatus->addWidget(lblCountry, 3, 0);
    layoutStatus->addWidget(lblStatusCountry, 3, 1, 1, 2);
    layoutStatus->addWidget(lblProtocol, 4, 0);
    layoutStatus->addWidget(lblStatusProtocol, 4, 1, 1, 2);
    layoutStatus->addWidget(lblUploaded, 5, 0);
    layoutStatus->addWidget(lblStatusUploaded, 5, 1, 1, 2);
    layoutStatus->addWidget(lblDownloaded, 6, 0);
    layoutStatus->addWidget(lblStatusDownloaded, 6, 1, 1, 2);
    layoutStatus->addWidget(lblUptime, 7, 0);
    layoutStatus->addWidget(lblStatusUptime, 7, 1, 1, 2);
    layoutStatus->setColumnStretch(1, 1);
    layoutStatus->setColumnMinimumWidth(0, 120);

    QGroupBox *boxStatus = new QGroupBox();
    boxStatus->setTitle(tr("boxHeaderStatus"));
    boxStatus->setStyleSheet("font-weight: bold;");
    boxStatus->setLayout(layoutStatus);

    /**
     * -------------------------------------------
     *  create connection box: types
     */

    serverListByType = new QVector<QString>();
    serverListByTypeModel = new QStringListModel();

    lstServersByType = new QListView();
    lstServersByType->setEnabled(false);
    lstServersByType->setStyleSheet("font-weight: normal;");
    QObject::connect(lstServersByType, &QListView::clicked, [this] {
        QModelIndexList indexes = this->lstServersByType->selectionModel()->selectedIndexes();
        pbConnectType->setEnabled(indexes.count() > 0);
    });

    txtFilterType = new QLineEdit();
    txtFilterType->setEnabled(false);
    txtFilterType->setPlaceholderText(tr("txtFilter"));
    txtFilterType->setStyleSheet("font-weight: normal;");
    QObject::connect(txtFilterType, &QLineEdit::textChanged, [this] {
        QString filter = this->txtFilterType->text().trimmed().toLower();
        if (filter.length() == 0)
        {
            this->serverListByTypeModel->setStringList(*this->serverListByType);
            return;
        }

        QVector<QString> *filteredServerList = new QVector<QString>();
        for (QString server : *this->serverListByType)
        {
            if (server.toLower().contains(filter))
            {
                filteredServerList->append(server);
            }
        }
        this->serverListByTypeModel->setStringList(*filteredServerList);
    });

    pbConnectType = new QPushButton();
    pbConnectType->setEnabled(false);
    pbConnectType->setText(tr("pbConnectType"));
    pbConnectType->setStyleSheet("font-weight: normal;");
    pbConnectType->setMinimumWidth(140);
    pbConnectType->setIcon(QIcon::fromTheme("network-vpn-symbolic"));
    QObject::connect(pbConnectType, &QPushButton::clicked, [this]()
    {
        QString type = this->lstServersByType->currentIndex().data().toString();
        type = type.replace(" ", "_");
        connectVpn(QStringList() << "connect" << type);
    });

    QGridLayout *layoutTypes = new QGridLayout();
    layoutTypes->addWidget(lstServersByType, 0, 0, 1, 2);
    layoutTypes->addWidget(txtFilterType, 1, 0);
    layoutTypes->addWidget(pbConnectType, 1, 1);

    QGroupBox *boxTypes = new QGroupBox();
    boxTypes->setTitle(tr("boxHeaderTypes"));
    boxTypes->setStyleSheet("font-weight: bold;");
    boxTypes->setLayout(layoutTypes);

    /**
     * -------------------------------------------
     *  create connection box: countries
     */

    serverListByCountry = new QVector<QString>();
    serverListByCountryModel = new QStringListModel();

    lstServersByCountry = new QListView();
    lstServersByCountry->setEnabled(false);
    lstServersByCountry->setStyleSheet("font-weight: normal;");
    QObject::connect(lstServersByCountry, &QListView::clicked, [this] {
        QModelIndexList indexes = this->lstServersByCountry->selectionModel()->selectedIndexes();
        QString country = this->lstServersByCountry->currentIndex().data().toString();
        pbConnectCountry->setEnabled(indexes.count() > 0);
    });

    txtFilterCountry = new QLineEdit();
    txtFilterCountry->setEnabled(false);
    txtFilterCountry->setPlaceholderText(tr("txtFilter"));
    txtFilterCountry->setStyleSheet("font-weight: normal;");
    QObject::connect(txtFilterCountry, &QLineEdit::textChanged, [this] {
        QString filter = this->txtFilterCountry->text().trimmed().toLower();
        if (filter.length() == 0)
        {
            this->serverListByCountryModel->setStringList(*this->serverListByCountry);
            return;
        }

        QVector<QString> *filteredServerList = new QVector<QString>();
        for (QString server : *this->serverListByCountry)
        {
            if (server.toLower().contains(filter))
            {
                filteredServerList->append(server);
            }
        }
        this->serverListByCountryModel->setStringList(*filteredServerList);
    });

    pbConnectCountry = new QPushButton();
    pbConnectCountry->setEnabled(false);
    pbConnectCountry->setText(tr("pbConnectCountry"));
    pbConnectCountry->setStyleSheet("font-weight: normal;");
    pbConnectCountry->setMinimumWidth(140);
    pbConnectCountry->setIcon(QIcon::fromTheme("network-vpn-symbolic"));
    QObject::connect(pbConnectCountry, &QPushButton::clicked, [this]()
    {
        QString country = this->lstServersByCountry->currentIndex().data().toString();
        country = country.replace(" ", "_");
        connectVpn(QStringList() << "connect" << country);
    });

    QGridLayout *layoutCountries = new QGridLayout();
    layoutCountries->addWidget(lstServersByCountry, 0, 0, 1, 2);
    layoutCountries->addWidget(txtFilterCountry, 1, 0);
    layoutCountries->addWidget(pbConnectCountry, 1, 1);

    QGroupBox *boxCountries = new QGroupBox();
    boxCountries->setTitle(tr("boxHeaderCountries"));
    boxCountries->setStyleSheet("font-weight: bold;");
    boxCountries->setLayout(layoutCountries);

    /**
     * -------------------------------------------
     *  combine types and countries into layout
     */

    QHBoxLayout *layoutConnectionTypes = new QHBoxLayout();
    layoutConnectionTypes->setContentsMargins(0, 0, 0, 0);
    layoutConnectionTypes->addWidget(boxTypes);
    layoutConnectionTypes->addSpacing(20);
    layoutConnectionTypes->addWidget(boxCountries);

    /**
     * -------------------------------------------
     *  create dialog button controls
     */

    pbQuit = new QPushButton();
    pbQuit->setText(tr("pbQuit"));
    pbQuit->setMinimumWidth(160);
    pbQuit->setIcon(QIcon::fromTheme("application-exit"));
    QObject::connect(pbQuit, &QPushButton::clicked, [this]()
    {
        this->quit(EXIT_CODE_NORMAL);
    });
    pbMinimize = new QPushButton();
    pbMinimize->setText(tr("pbMinimize"));
    pbMinimize->setMinimumWidth(160);
    pbMinimize->setIcon(QIcon::fromTheme("system-tray-symbolic"));
    QObject::connect(pbMinimize, &QPushButton::clicked, [this]()
    {
        this->hideUi();
    });
    pbSettings = new QPushButton();
    pbSettings->setText(tr("pbSettings"));
    pbSettings->setMinimumWidth(160);
    pbSettings->setIcon(QIcon::fromTheme("settings"));
    QObject::connect(pbSettings, &QPushButton::clicked, [this]()
    {
        // show dialog
        while (updatingStatus) {
            // wait
        }
        updatingStatus = true;
        Settings *dlgSettings = new Settings();
        dlgSettings->setAttribute( Qt::WA_DeleteOnClose );
        dlgSettings->activateWindow();
        dlgSettings->raise();
        dlgSettings->exec();
        ui->activateWindow();
        ui->raise();
        updatingStatus = false;
    });

    QHBoxLayout *layoutDialogControls = new QHBoxLayout();
    layoutDialogControls->setContentsMargins(0, 0, 0, 0);
    layoutDialogControls->addWidget(pbMinimize);
    layoutDialogControls->addSpacing(10);
    layoutDialogControls->addWidget(pbSettings);
    layoutDialogControls->addStretch(1);
    layoutDialogControls->addWidget(pbQuit);

    /**
     * -------------------------------------------
     *  create the main widget
     */

    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->setContentsMargins(20, 15, 20, 15);
    layoutMain->addWidget(boxStatus);
    layoutMain->addSpacing(20);
    layoutMain->addItem(layoutConnectionTypes);
    layoutMain->addSpacing(20);
    layoutMain->addItem(layoutDialogControls);

    /**
     * -------------------------------------------
     * create a status bar
     */

    lblStatusConnection = new QLabel("Disconnected");
    lblStatusConnection->setEnabled(false);

    QLabel *lblVersion = new QLabel(tr("appTitle") + " " + tr("appVersion"));
    lblVersion->setEnabled(false);

    QStatusBar *statusBar = new QStatusBar();
    statusBar->setContentsMargins(3, 0, 0, 0);
    statusBar->addPermanentWidget(lblStatusConnection, 1);
    statusBar->addPermanentWidget(lblVersion);

    /**
     * -------------------------------------------
     * create application widget
     */
    QVBoxLayout *layoutApplication = new QVBoxLayout();
    layoutApplication->setContentsMargins(0, 0, 0, 0);
    layoutApplication->addItem(layoutMain);
    layoutApplication->addWidget(statusBar);

    QWidget *widget = new QWidget();
    widget->setLayout(layoutApplication);

    ui = new QMainWindow();
    ui->setWindowIcon(QIcon::fromTheme("compass"));
    ui->setCentralWidget(widget);
    ui->setMinimumSize(1000, 680);
    ui->resize(1000, 680);
    ui->show();
    ui->activateWindow();
    ui->raise();
}

/**
 * Create a Tray Menu
 * @brief Kompass::setupTray
 */
void Kompass::setupTray()
{
    // create tray icon and register listeners
    trayIcon = new QSystemTrayIcon(QIcon::fromTheme("compass"), this);
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [this]() {
        showUi();
    });

    // create connect menu item
    actionConnect = new QAction(tr("trayActionConnect"), trayIcon);
    QObject::connect(actionConnect, &QAction::triggered, [this]() {
        connectVpn(QStringList());
    });

    // create connect menu item
    actionDisconnect = new QAction(tr("trayActionDisconnect"), trayIcon);
    QObject::connect(actionDisconnect, &QAction::triggered, [this]() {
        disconnectVpn();
    });

    // create show menu item
    QAction *actionShow = new QAction(tr("trayActionShow"), trayIcon);
    QObject::connect(actionShow, &QAction::triggered, [this]() {
        showUi();
    });

    // create quit menu item
    QAction *actionQuit = new QAction(tr("trayActionQuit"), trayIcon);
    QObject::connect(actionQuit, &QAction::triggered, [this]() {
        quit(EXIT_CODE_NORMAL);
    });

    mnStatusConnection = new QWidgetAction(this);
    mnStatusConnection->setIcon(QIcon::fromTheme("network-vpn-disconnected-symbolic"));
    mnStatusConnection->setText(tr("statusDisconnected"));
    mnStatusConnection->setEnabled(false);
    mnStatusConnection->setVisible(true);

    mnStatusUploaded = new QWidgetAction(this);
    mnStatusUploaded->setText("--");
    mnStatusUploaded->setIcon(QIcon::fromTheme("draw-arrow-up"));
    mnStatusUploaded->setEnabled(false);
    mnStatusUploaded->setVisible(false);

    mnStatusDownloaded = new QWidgetAction(this);
    mnStatusDownloaded->setText("--");
    mnStatusDownloaded->setIcon(QIcon::fromTheme("draw-arrow-down"));
    mnStatusDownloaded->setEnabled(false);
    mnStatusDownloaded->setVisible(false);

    mnStatusUptime = new QWidgetAction(this);
    mnStatusUptime->setText("--");
    mnStatusUptime->setIcon(QIcon::fromTheme("clock-applet-symbolic"));
    mnStatusUptime->setEnabled(false);
    mnStatusUptime->setVisible(false);

    // create context ,emi
    trayIconMenu = new QMenu;
    trayIconMenu->addAction(mnStatusConnection);
    trayIconMenu->addAction(mnStatusUploaded);
    trayIconMenu->addAction(mnStatusDownloaded);
    trayIconMenu->addAction(mnStatusUptime);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionConnect);
    trayIconMenu->addAction(actionDisconnect);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionShow);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionQuit);
    trayIcon->setContextMenu(trayIconMenu);

    // show the menu icon
    trayIcon->show();
}

/**
 * Load server lists to display
 * @brief Kompass::setupData
 */
void Kompass::setupData()
{
    setupDataTypes();
    setupDataCountries();
}

/**
 * Load list of VPN server types
 * @brief Kompass::setupDataTypes
 */
void Kompass::setupDataTypes()
{
    QProcess *commandTypes = new QProcess();
    commandTypes->start("nordvpn", QStringList() << "groups");
    commandTypes->waitForFinished();

    QString outputGroups = commandTypes->readAllStandardOutput();
    if (outputGroups != nullptr && outputGroups.length())
    {
        outputGroups = outputGroups.replace(QRegularExpression("\\s+"), QString());
        outputGroups = outputGroups.replace(QRegularExpression("[\\]|[\|]|[/]|[-]"), QString());
        outputGroups = outputGroups.replace("_", " ");

        serverListByType = new QVector<QString>();
        serverListByType->append(outputGroups.split(","));
        serverListByTypeModel = new QStringListModel(this);
        serverListByTypeModel->setStringList(*serverListByType);

        lstServersByType->setModel(serverListByTypeModel);
        lstServersByType->setSelectionMode(QAbstractItemView::SingleSelection);
        lstServersByType->selectionModel()->clearSelection();
        txtFilterType->setText(QString());
        pbConnectType->setEnabled(false);
    }
}

/**
 * Load list of countries VPN servers are in
 * @brief Kompass::setupDataCountries
 */
void Kompass::setupDataCountries()
{
    QProcess *commandCountries = new QProcess();
    commandCountries->start("nordvpn", QStringList() << "countries");
    commandCountries->waitForFinished();

    QString outputCountries = commandCountries->readAllStandardOutput();
    if (outputCountries != nullptr && outputCountries.length())
    {
        outputCountries = outputCountries.replace(QRegularExpression("\\s+"), QString());
        outputCountries = outputCountries.replace(QRegularExpression("[\\]|[\|]|[/]|[-]"), QString());
        outputCountries = outputCountries.replace("_", " ");

        QList<QString> countries = outputCountries.split(",");

        serverListByCountry = new QVector<QString>();
        serverListByCountry->append(countries);
        serverListByCountryModel = new QStringListModel(this);
        serverListByCountryModel->setStringList(*serverListByCountry);

        lstServersByCountry->setModel(serverListByCountryModel);
        lstServersByCountry->setSelectionMode(QAbstractItemView::SingleSelection);
        lstServersByCountry->selectionModel()->clearSelection();
        txtFilterCountry->setText(QString());
        pbConnectCountry->setEnabled(false);
    }
}

/**
 * Monitors the VPN connection and provides visual
 * status updates
 * @brief Kompass::setupStatusMonitor
 */
void Kompass::setupStatusMonitor()
{
    QTimer *statusMonitor = new QTimer();
    statusMonitor->start(2000);
    QObject::connect(statusMonitor, &QTimer::timeout, [this]() {
        if (this->updatingStatus || this->currentStatus == STATUS_CONNECTING)
        {
            // skip
            return;
        }

        // execute command
        QProcess *command = new QProcess();
        command->start("nordvpn", QStringList() << "status");
        command->waitForFinished();

        QString output = command->readAllStandardOutput();
        if (output.toLower().contains("disconnected"))
        {
            updateUi(STATUS_DISCONNECTED, nullptr);
        }
        else if (output.toLower().contains("connected"))
        {
            updateUi(STATUS_CONNECTED, output);
        }
    });
}

/**
 * Calls NordVPN binary to connect using the given list
 * if commands. If no commands are given, Kompass will
 * connect to (what NordVPN considers) the fastest server.
 * @brief Kompass::connectVpn
 * @param commands
 */
void Kompass::connectVpn(QStringList commands)
{
    // Update UI
    updateUi(STATUS_CONNECTING, nullptr);

    // execute command
    QProcess *command = new QProcess();
    if (commands.length() == 0)
    {
        command->start("nordvpn", QStringList() << "connect");
    }
    else
    {
        command->start("nordvpn", commands);
    }
    command->waitForFinished();

    QString output = command->readAllStandardOutput();
    QString error = command->readAllStandardError();

    if (error == nullptr || (error != nullptr && error.trimmed().length() == 0))
    {
        updateUi(STATUS_CONNECTED, nullptr);
    }
    else
    {
        updateUi(STATUS_DISCONNECTED, nullptr);
        QMessageBox msg = QMessageBox();
        msg.setWindowIcon(QIcon::fromTheme("compass"));
        msg.setWindowTitle(tr("appTitle"));
        msg.setText(tr("msgErrorConnecting") + error.trimmed());
        msg.setIcon(QMessageBox::Warning);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setWindowModality(Qt::WindowModality::ApplicationModal);
        msg.activateWindow();
        msg.show();
    }
}

/**
 * Disconnect the current VPN Connection
 * @brief Kompass::disconnectVpn
 */
void Kompass::disconnectVpn()
{    
    // Update UI
    updateUi(STATUS_DISABLED, nullptr);

    // execute command
    QProcess *command = new QProcess();
    command->start("nordvpn", QStringList() << "disconnect");
    command->waitForFinished();

    QString output = command->readAllStandardOutput();
    QString error = command->readAllStandardError();

    if (error == nullptr || (error != nullptr && error.trimmed().length() == 0))
    {
        updateUi(STATUS_DISCONNECTED, nullptr);
    }
    else
    {
        QMessageBox msg = QMessageBox();
        msg.setWindowIcon(QIcon::fromTheme("compass"));
        msg.setWindowTitle(tr("appTitle"));
        msg.setText("msgErrorDisconnecting" + error.trimmed());
        msg.setIcon(QMessageBox::Warning);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setWindowModality(Qt::WindowModality::ApplicationModal);
        msg.activateWindow();
        msg.show();
    }
}

/**
 * Updates the Connection Status Display
 * Enables/disables Buttons based on Connection Status
 * @brief Kompass::setStatus
 * @param status
 * @param vpnDetails
 */
void Kompass::updateUi(int status, QString vpnDetails)
{
    updatingStatus = true;

    // these buttons are always active
    pbQuit->setEnabled(true);
    pbMinimize->setEnabled(true);

    // get list selection
    QModelIndexList selectedTypesIndex = lstServersByType->selectionModel()->selectedIndexes();
    QModelIndexList selectedCountriesIndex = lstServersByCountry->selectionModel()->selectedIndexes();

    // init status values
    QMap<QString, QString> *vpnStatus = new QMap<QString, QString>();
    vpnStatus->insert("server", QString());
    vpnStatus->insert("country", QString());
    vpnStatus->insert("city", QString());
    vpnStatus->insert("ip", QString());
    vpnStatus->insert("technology", QString());
    vpnStatus->insert("protocol", QString());
    vpnStatus->insert("uploaded", QString());
    vpnStatus->insert("downloaded", QString());
    vpnStatus->insert("uptime", QString());
    if (vpnDetails != nullptr && status == STATUS_CONNECTED)
    {
        QStringList lines = vpnDetails.trimmed().split(QString("\n"));
        for (QString line : lines) {
            if (line.trimmed().toLower().startsWith("current server")) {
                vpnStatus->insert("server", line.mid(line.indexOf(":") + 1).trimmed());
            }
            if (line.trimmed().toLower().startsWith("country")) {
                vpnStatus->insert("country", line.mid(line.indexOf(":") + 1).trimmed());
            }
            if (line.trimmed().toLower().startsWith("city")) {
                vpnStatus->insert("city", line.mid(line.indexOf(":") + 1).trimmed());
            }
            if (line.trimmed().toLower().startsWith("server ip")) {
                vpnStatus->insert("ip", line.mid(line.indexOf(":") + 1).trimmed());
            }
            if (line.trimmed().toLower().startsWith("current technology")) {
                vpnStatus->insert("technology", line.mid(line.indexOf(":") + 1).trimmed());
            }
            if (line.trimmed().toLower().startsWith("current protocol")) {
                vpnStatus->insert("protocol", line.mid(line.indexOf(":") + 1).trimmed());
            }
            if (line.trimmed().toLower().startsWith("transfer")) {
                QString downloaded = line.mid(line.indexOf(":") + 1);
                downloaded = downloaded.left(downloaded.indexOf("r") - 1).trimmed();
                QString uploaded = line.mid(line.indexOf(",") + 1);
                uploaded = uploaded.left(uploaded.indexOf("s") - 1).trimmed();
                vpnStatus->insert("uploaded", uploaded);
                vpnStatus->insert("downloaded", downloaded);
            }
            if (line.trimmed().toLower().startsWith("uptime")) {
                QString uptime = line.mid(line.indexOf(":") + 1).trimmed();
                int hrs = 0;
                int min = 0;
                int sec = 0;

                if (uptime.toLower().contains("day"))
                {
                    QString d = uptime.left(uptime.toLower().indexOf("day") - 1);
                    d = d.mid(d.lastIndexOf(" "));
                    d = d.replace(QRegularExpression("\\D+"), QString());
                    hrs += 24 * d.toInt();
                }

                if (uptime.toLower().contains("hour"))
                {
                     QString h = uptime.left(uptime.toLower().indexOf("hour") - 1);
                     h = h.mid(h.lastIndexOf(" "));
                     h = h.replace(QRegularExpression("\\D+"), QString());
                     hrs += h.toInt();
                }

                if (uptime.toLower().contains("minute"))
                {
                    QString m = uptime.left(uptime.toLower().indexOf("minute") - 1);
                    m = m.mid(m.lastIndexOf(" "));
                    m = m.replace(QRegularExpression("\\D+"), QString());
                    min += m.toInt();
                }

                if (uptime.toLower().contains("second"))
                {
                    QString s = uptime.left(uptime.toLower().indexOf("second") - 1);
                    s = s.mid(s.lastIndexOf(" "));
                    s = s.replace(QRegularExpression("\\D+"), QString());
                    sec += s.toInt();
                }

                if (hrs > 0)
                {
                    uptime = QString::number(hrs) + ":" +
                             ((min < 10) ? "0" + QString::number(min) :  QString::number(min)) + ":" +
                             ((sec < 10) ? "0" + QString::number(sec) :  QString::number(sec)) +
                             " hrs";
                }
                else if (min > 0)
                {
                    uptime = QString::number(min) + ":" +
                             ((sec < 10) ? "0" + QString::number(sec) :  QString::number(sec)) +
                             " min";
                }
                else
                {
                    uptime = QString::number(sec) + " sec";
                }
                vpnStatus->insert("uptime", uptime);
            }
        }
    }

    // enable by state
    switch (status)
    {
        case STATUS_CONNECTING:
            mnStatusUploaded->setVisible(false);
            mnStatusDownloaded->setVisible(false);
            mnStatusUptime->setVisible(false);
            mnStatusConnection->setIcon(QIcon::fromTheme("network-vpn-disconnected-symbolic"));
            mnStatusConnection->setText(tr("statusConnecting"));
            mnStatusUploaded->setText("--");
            mnStatusDownloaded->setText("--");
            lblStatusConnection->setText(tr("statusConnecting"));
            txtStatus->setText(tr("statusConnecting"));
            txtStatus->setStyleSheet("font-weight: bold; color: rgb(177, 177, 0); background: rgb(255, 255, 235); selection-background-color: rgb(255, 255, 235);");
            lblStatusServer->setText("--");
            lblStatusCity->setText("--");
            lblStatusCountry->setText("--");
            lblStatusProtocol->setText("--");
            lblStatusUploaded->setText("--");
            lblStatusDownloaded->setText("--");
            lblStatusUptime->setText("--");
            actionConnect->setEnabled(false);
            actionDisconnect->setEnabled(false);
            pbDisconnect->setEnabled(false);
            pbConnectType->setEnabled(false);
            pbConnectCountry->setEnabled(false);
            lstServersByType->setEnabled(false);
            lstServersByCountry->setEnabled(false);
            txtFilterType->setEnabled(false);
            txtFilterCountry->setEnabled(false);
            pbSettings->setEnabled(false);
            break;
        case STATUS_DISCONNECTED:
            mnStatusUploaded->setVisible(false);
            mnStatusDownloaded->setVisible(false);
            mnStatusUptime->setVisible(false);
            mnStatusConnection->setIcon(QIcon::fromTheme("network-vpn-disconnected-symbolic"));
            mnStatusConnection->setText(tr("statusDisconnected"));
            mnStatusUploaded->setText("--");
            mnStatusDownloaded->setText("--");
            lblStatusConnection->setText(tr("statusDisconnected"));
            txtStatus->setText(tr("statusDisconnected"));
            txtStatus->setStyleSheet("font-weight: bold; color: rgb(177, 0, 0); background: rgb(255, 216, 216); selection-background-color: rgb(255, 216, 216);");
            lblStatusServer->setText("--");
            lblStatusCity->setText("--");
            lblStatusCountry->setText("--");
            lblStatusProtocol->setText("--");
            lblStatusUploaded->setText("--");
            lblStatusDownloaded->setText("--");
            lblStatusUptime->setText("--");
            txtFilterType->setEnabled(true);
            txtFilterCountry->setEnabled(true);
            lstServersByType->setEnabled(true);
            lstServersByCountry->setEnabled(true);
            pbConnectType->setEnabled(selectedTypesIndex.length() > 0);
            pbConnectCountry->setEnabled(selectedCountriesIndex.length() > 0);
            actionConnect->setEnabled(true);
            actionDisconnect->setEnabled(false);
            pbDisconnect->setEnabled(false);
            pbSettings->setEnabled(true);
            break;
        case STATUS_CONNECTED:
            mnStatusConnection->setIcon(QIcon::fromTheme("network-vpn-symbolic"));
            mnStatusConnection->setText(tr("statusConnected"));
            lblStatusConnection->setText(tr("statusConnected"));
            txtStatus->setText(tr("statusConnected"));
            txtStatus->setStyleSheet("font-weight: bold; color: rgb(33, 131, 33); background: rgb(229, 248, 229); selection-background-color: rgb(229, 248, 229);");
            if (vpnStatus->value("server").length() > 0) {
                lblStatusServer->setText(vpnStatus->value("server"));
            }
            if (vpnStatus->value("city").length() > 0) {
                lblStatusCity->setText(vpnStatus->value("city"));
            }
            if (vpnStatus->value("country").length() > 0) {
                lblStatusCountry->setText(vpnStatus->value("country"));
                mnStatusConnection->setText(tr("statusConnected") + ": " + vpnStatus->value("country"));
            }
            if (vpnStatus->value("protocol").length() > 0 && vpnStatus->value("technology").length() > 0) {
                lblStatusProtocol->setText(vpnStatus->value("protocol") + " [" + vpnStatus->value("technology") + "]");
            }
            if (vpnStatus->value("uploaded").length() > 0) {
                lblStatusUploaded->setText(vpnStatus->value("uploaded"));
                mnStatusUploaded->setText(vpnStatus->value("uploaded"));
                mnStatusUploaded->setVisible(true);
            }
            if (vpnStatus->value("downloaded").length() > 0) {
                lblStatusDownloaded->setText(vpnStatus->value("downloaded"));
                mnStatusDownloaded->setText(vpnStatus->value("downloaded"));
                mnStatusDownloaded->setVisible(true);
            }
            if (vpnStatus->value("uptime").length() > 0) {
                lblStatusUptime->setText(vpnStatus->value("uptime"));
                mnStatusUptime->setText(vpnStatus->value("uptime"));
                mnStatusUptime->setVisible(true);
            }
            actionDisconnect->setEnabled(true);
            pbDisconnect->setEnabled(true);
            actionConnect->setEnabled(false);
            pbConnectType->setEnabled(false);
            pbConnectCountry->setEnabled(false);
            lstServersByType->setEnabled(false);
            lstServersByCountry->setEnabled(false);
            txtFilterType->setEnabled(false);
            txtFilterCountry->setEnabled(false);
            pbSettings->setEnabled(true);
            break;
        case STATUS_DISABLED:
            actionConnect->setEnabled(false);
            actionDisconnect->setEnabled(false);
            pbDisconnect->setEnabled(false);
            pbConnectType->setEnabled(false);
            pbConnectCountry->setEnabled(false);
            lstServersByType->setEnabled(false);
            lstServersByCountry->setEnabled(false);
            txtFilterType->setEnabled(false);
            txtFilterCountry->setEnabled(false);
            pbSettings->setEnabled(false);
            break;
    }

    currentStatus = status;
    updatingStatus = false;
}

/**
 * Show the main application window
 * @brief Kompass::showKompass
 */
void Kompass::showUi()
{
    if (!ui->isVisible()) {
        ui->setVisible(true);
    }
    ui->show();
    ui->activateWindow();
    ui->raise();
}

/**
 * Hide the main application window
 * @brief Kompass::hideKompass
 */
void Kompass::hideUi()
{
    if (ui->isVisible()) {
        ui->setVisible(false);
    }
}

/**
 * Exit the Application
 * @brief Kompass::quitKompass
 * @param exitCode
 */
void Kompass::quit(int exitCode)
{
    exit(exitCode);
}
