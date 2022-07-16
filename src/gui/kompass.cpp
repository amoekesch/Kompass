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
    this->setupReleaseMonitor();
}

/**
 * setup controls and event listeners
 * @brief Kompass::setup
 */
void Kompass::setupUi()
{   
    /**
     * -------------------------------------------
     * create status widget
     */
    QWidget *wxSpinner = new QWidget();
    wxSpinner->setContentsMargins(0, 0, 30, 0);

    svgSpinner = new QSvgWidget();
    svgSpinner->load(QString(":/img/spinner.svg"));
    svgSpinner->setFixedWidth(32);
    svgSpinner->setFixedHeight(32);
    svgSpinner->setGeometry(0, 0, 32, 32);
    svgSpinner->setStyleSheet("background: transparent;");

    QHBoxLayout *layoutSpinner = new QHBoxLayout();
    layoutSpinner->setContentsMargins(0, 0, 0, 0);
    layoutSpinner->addWidget(svgSpinner);
    wxSpinner->setLayout(layoutSpinner);

    QLabel *lblStatus = new QLabel(tr("lblStatus"));
    lblStatus->setStyleSheet("font-size: 22px; font-weight: bold; color: " + lblStatus->palette().highlight().color().name() + ";");
    tbStatus = new ToggleButton(16, 20, true);
    tbStatus->overrideTextColor(true, QColor::fromRgb(180, 239, 180));
    tbStatus->overrideTrackColor(true, QColor::fromRgb(180, 239, 180));
    tbStatus->overrideThumbColor(true, QColor::fromRgb(58, 206, 58));
    tbStatus->overrideTextColor(false, QColor::fromRgb(236, 180, 180));
    tbStatus->overrideTrackColor(false, QColor::fromRgb(236, 180, 180));
    tbStatus->overrideThumbColor(false, QColor::fromRgb(206, 58, 58));
    tbStatus->overrideOpacity(1);
    tbStatus->setEnabled(false);
    QObject::connect(tbStatus, &ToggleButton::clicked, [this]() {
        toggleVpn(QStringList() << "connect", tbStatus->isChecked(), CONNECTION_TRIGGER_FASTEST);
    });

    QHBoxLayout *layoutConnect = new QHBoxLayout();
    layoutConnect->setContentsMargins(0, 0, 0, 50);
    layoutConnect->addWidget(lblStatus);
    layoutConnect->addStretch(1);
    layoutConnect->addWidget(wxSpinner);
    layoutConnect->addWidget(tbStatus);

    SectionTitle *titleConnnectionStatus = new SectionTitle();
    titleConnnectionStatus->setTitle(tr("titleConnectionStatus"));

    SectionTitle *titleAccountDetails = new SectionTitle();
    titleAccountDetails->setTitle(tr("titleAccountDetails"));

    QLabel *lblServer = new QLabel(tr("lblServer"));
    lblServer->setStyleSheet("font-weight: bold;");
    txtStatusServer = new QLineEdit("--");
    txtStatusServer->setReadOnly(true);

    QLabel *lblCity = new QLabel(tr("lblCity"));
    lblCity->setStyleSheet("font-weight: bold;");
    txtStatusCity = new QLineEdit("--");
    txtStatusCity->setReadOnly(true);

    QLabel *lblCountry = new QLabel(tr("lblCountry"));
    lblCountry->setStyleSheet("font-weight: bold;");
    txtStatusCountry = new QLineEdit("--");
    txtStatusCountry->setReadOnly(true);

    QLabel *lblProtocol = new QLabel(tr("lblProtocol"));
    lblProtocol->setStyleSheet("font-weight: bold;");
    txtStatusProtocol = new QLineEdit("--");
    txtStatusProtocol->setReadOnly(true);

    QLabel *lblUploaded = new QLabel(tr("lblUploaded"));
    lblUploaded->setStyleSheet("font-weight: bold;");
    txtStatusUploaded = new QLineEdit("--");
    txtStatusUploaded->setReadOnly(true);

    QLabel *lblDownloaded = new QLabel(tr("lblDownloaded"));
    lblDownloaded->setStyleSheet("font-weight: bold;");
    txtStatusDownloaded = new QLineEdit("--");
    txtStatusDownloaded->setReadOnly(true);

    QLabel *lblUptime = new QLabel(tr("lblUptime"));
    lblUptime->setStyleSheet("font-weight: bold;");
    txtStatusUptime = new QLineEdit("--");
    txtStatusUptime->setReadOnly(true);

    QLabel *lblUsername = new QLabel(tr("lblUsername"));
    lblUsername->setStyleSheet("font-weight: bold;");
    txtUsername = new QLineEdit("--");
    txtUsername->setReadOnly(true);

    QLabel *lblLicense = new QLabel(tr("lblLicense"));
    lblLicense->setStyleSheet("font-weight: bold;");
    txtLicense = new QLineEdit("--");
    txtLicense->setReadOnly(true);

    QLabel *lblVersion = new QLabel(tr("appTitle") + " " + tr("appVersion"));
    lblVersion->setEnabled(false);
    lblVersion->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    lblVersion->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    lblVersionUpdate = new QLabel("");
    lblVersionUpdate->setOpenExternalLinks(true);
    lblVersionUpdate->setTextInteractionFlags(Qt::TextBrowserInteraction);
    lblVersionUpdate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    lblVersionUpdate->setAlignment(Qt::AlignCenter);

    QGridLayout *layoutStatus = new QGridLayout();
    layoutStatus->setContentsMargins(0, 0, 0, 0);
    layoutStatus->addItem(layoutConnect, 0, 0, 1, 3);
    layoutStatus->addWidget(titleConnnectionStatus, 1, 0, 1, 3);
    layoutStatus->addWidget(lblServer, 2, 0);
    layoutStatus->addWidget(txtStatusServer, 2, 1, 1, 2);
    layoutStatus->addWidget(lblCity, 3, 0);
    layoutStatus->addWidget(txtStatusCity, 3, 1, 1, 2);
    layoutStatus->addWidget(lblCountry, 4, 0);
    layoutStatus->addWidget(txtStatusCountry, 4, 1, 1, 2);
    layoutStatus->addWidget(lblProtocol, 5, 0);
    layoutStatus->addWidget(txtStatusProtocol, 5, 1, 1, 2);
    layoutStatus->addWidget(lblUploaded, 6, 0);
    layoutStatus->addWidget(txtStatusUploaded, 6, 1, 1, 2);
    layoutStatus->addWidget(lblDownloaded, 7, 0);
    layoutStatus->addWidget(txtStatusDownloaded, 7, 1, 1, 2);
    layoutStatus->addWidget(lblUptime, 8, 0);
    layoutStatus->addWidget(txtStatusUptime, 8, 1, 1, 2);
    layoutStatus->addItem(new QSpacerItem(0, 30, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 9, 0, 1, 3);
    layoutStatus->addWidget(titleAccountDetails, 10, 0, 1, 3);
    layoutStatus->addWidget(lblUsername, 11, 0);
    layoutStatus->addWidget(txtUsername, 11, 1, 1, 2);
    layoutStatus->addWidget(lblLicense, 12, 0);
    layoutStatus->addWidget(txtLicense, 12, 1, 1, 2);
    layoutStatus->addWidget(lblVersionUpdate, 14, 0, 1, 2);
    layoutStatus->addWidget(lblVersion, 14, 2, 1, 1);
    layoutStatus->setColumnStretch(1, 1);
    layoutStatus->setRowStretch(13, 1);
    layoutStatus->setColumnMinimumWidth(0, 130);

    QWidget *wxStatus = new QWidget();
    wxStatus->setLayout(layoutStatus);

    /**
     * -------------------------------------------
     * create types widget
     */

    SectionTitle *titleConnnectionType = new SectionTitle();
    titleConnnectionType->setTitle(tr("titleConnnectionType"));

    serverListByType = new QVector<QString>();
    serverListByTypeModel = new QStringListModel();

    lstServersByType = new QListView();
    lstServersByType->setEnabled(false);
    lstServersByType->setHorizontalScrollMode(QListView::ScrollPerPixel);
    lstServersByType->setVerticalScrollMode(QListView::ScrollPerPixel);
    lstServersByType->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(lstServersByType, &QListView::clicked, [this] {
        QModelIndexList indexes = this->lstServersByType->selectionModel()->selectedIndexes();
        tbConnectType->setEnabled(indexes.count() > 0);
    });

    txtFilterType = new QLineEdit();
    txtFilterType->setEnabled(false);
    txtFilterType->setPlaceholderText(tr("txtFilter"));
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

    QLabel *lblConnectType = new QLabel(tr("lblConnectType"));
    lblConnectType->setStyleSheet("font-size: 22px; font-weight: bold; color: " + lblStatus->palette().highlight().color().name() + ";");
    tbConnectType = new ToggleButton(16, 20, true);
    tbConnectType->overrideTextColor(true, QColor::fromRgb(180, 239, 180));
    tbConnectType->overrideTrackColor(true, QColor::fromRgb(180, 239, 180));
    tbConnectType->overrideThumbColor(true, QColor::fromRgb(58, 206, 58));
    tbConnectType->overrideTextColor(false, QColor::fromRgb(236, 180, 180));
    tbConnectType->overrideTrackColor(false, QColor::fromRgb(236, 180, 180));
    tbConnectType->overrideThumbColor(false, QColor::fromRgb(206, 58, 58));
    tbConnectType->overrideOpacity(1);
    tbConnectType->setEnabled(false);
    QObject::connect(tbConnectType, &ToggleButton::clicked, [this]() {
        QString type = this->lstServersByType->currentIndex().data().toString();
        type = type.replace(" ", "_");
        menu->select(0);
        toggleVpn(QStringList() << "connect" << type, tbConnectType->isChecked(), CONNECTION_TRIGGER_TYPE);
    });

    QHBoxLayout *layoutConnectType = new QHBoxLayout();
    layoutConnectType->setContentsMargins(0, 0, 0, 50);
    layoutConnectType->addWidget(lblConnectType);
    layoutConnectType->addStretch(1);
    layoutConnectType->addWidget(tbConnectType);

    QVBoxLayout *layoutTypes = new QVBoxLayout();
    layoutTypes->setContentsMargins(0, 0, 0, 0);
    layoutTypes->addItem(layoutConnectType);
    layoutTypes->addWidget(titleConnnectionType);
    layoutTypes->addWidget(lstServersByType);
    layoutTypes->addWidget(txtFilterType);

    QWidget *wxTypes = new QWidget();
    wxTypes->setLayout(layoutTypes);

    /**
     * -------------------------------------------
     * create countries widget
     */

    SectionTitle *titleConnnectionCountry = new SectionTitle();
    titleConnnectionCountry->setTitle(tr("titleConnnectionCountry"));

    serverListByCountry = new QVector<QString>();
    serverListByCountryModel = new QStringListModel();

    lstServersByCountry = new QListView();
    lstServersByCountry->setEnabled(false);
    lstServersByCountry->setHorizontalScrollMode(QListView::ScrollPerPixel);
    lstServersByCountry->setVerticalScrollMode(QListView::ScrollPerPixel);
    lstServersByCountry->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(lstServersByCountry, &QListView::clicked, [this] {
        QModelIndexList indexes = this->lstServersByCountry->selectionModel()->selectedIndexes();
        tbConnectCountry->setEnabled(indexes.count() > 0);
    });

    txtFilterCountry = new QLineEdit();
    txtFilterCountry->setEnabled(false);
    txtFilterCountry->setPlaceholderText(tr("txtFilter"));
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

    QLabel *lblConnectCountry = new QLabel(tr("lblConnectCountry"));
    lblConnectCountry->setStyleSheet("font-size: 22px; font-weight: bold; color: " + lblStatus->palette().highlight().color().name() + ";");
    tbConnectCountry = new ToggleButton(16, 20, true);
    tbConnectCountry->overrideTextColor(true, QColor::fromRgb(180, 239, 180));
    tbConnectCountry->overrideTrackColor(true, QColor::fromRgb(180, 239, 180));
    tbConnectCountry->overrideThumbColor(true, QColor::fromRgb(58, 206, 58));
    tbConnectCountry->overrideTextColor(false, QColor::fromRgb(236, 180, 180));
    tbConnectCountry->overrideTrackColor(false, QColor::fromRgb(236, 180, 180));
    tbConnectCountry->overrideThumbColor(false, QColor::fromRgb(206, 58, 58));
    tbConnectCountry->overrideOpacity(1);
    tbConnectCountry->setEnabled(false);
    QObject::connect(tbConnectCountry, &ToggleButton::clicked, [this]() {
        QString country = this->lstServersByCountry->currentIndex().data().toString();
        country = country.replace(" ", "_");
        menu->select(0);
        toggleVpn(QStringList() << "connect" << country, tbConnectCountry->isChecked(), CONNECTION_TRIGGER_COUNTRY);
    });
    QHBoxLayout *layoutConnectCountry = new QHBoxLayout();
    layoutConnectCountry->setContentsMargins(0, 0, 0, 50);
    layoutConnectCountry->addWidget(lblConnectCountry);
    layoutConnectCountry->addStretch(1);
    layoutConnectCountry->addWidget(tbConnectCountry);

    QVBoxLayout *layoutCountries = new QVBoxLayout();
    layoutCountries->setContentsMargins(0, 0, 0, 0);
    layoutCountries->addItem(layoutConnectCountry);
    layoutCountries->addWidget(titleConnnectionCountry);
    layoutCountries->addWidget(lstServersByCountry);
    layoutCountries->addWidget(txtFilterCountry);

    QWidget *wxCountries = new QWidget();
    wxCountries->setLayout(layoutCountries);

    /**
     * -------------------------------------------
     * create servers widget
     */

    SectionTitle *titleConnnectionServerDetails = new SectionTitle();
    titleConnnectionServerDetails->setTitle(tr("titleConnnectionServerDetails"));

    titleConnnectionServerList = new SectionTitle();
    titleConnnectionServerList->setTitle(tr("titleConnnectionServerList"));

    QLabel *lblServerDetailsHost = new QLabel(tr("lblServerDetailsHost"));
    lblServerDetailsHost->setStyleSheet("font-weight: bold;");
    QLineEdit *txtServerDetailsHost = new QLineEdit();
    txtServerDetailsHost->setReadOnly(true);

    QLabel *lblServerDetailsCountry = new QLabel(tr("lblServerDetailsCountry"));
    lblServerDetailsCountry->setStyleSheet("font-weight: bold;");
    QLineEdit *txtServerDetailsCountry = new QLineEdit();
    txtServerDetailsCountry->setReadOnly(true);

    QLabel *lblServerDetailsCity = new QLabel(tr("lblServerDetailsCity"));
    lblServerDetailsCity->setStyleSheet("font-weight: bold;");
    QLineEdit *txtServerDetailsCity = new QLineEdit();
    txtServerDetailsCity->setReadOnly(true);

    QStringListModel *mdlServerDetailsTechnologies = new QStringListModel();
    QLabel *lblServerDetailsTechnologies = new QLabel(tr("lblServerDetailsTechnologies"));
    lblServerDetailsTechnologies->setStyleSheet("font-weight: bold;");
    QListView *lstServerDetailsTechnologies = new QListView();
    lstServerDetailsTechnologies->setMinimumHeight(80);
    lstServerDetailsTechnologies->setMaximumHeight(80);
    lstServerDetailsTechnologies->setModel(mdlServerDetailsTechnologies);
    lstServerDetailsTechnologies->setFocusPolicy(Qt::NoFocus);
    lstServerDetailsTechnologies->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QGridLayout *layoutServerDetails = new QGridLayout();
    layoutServerDetails->addWidget(lblServerDetailsHost, 0, 0, 1, 1);
    layoutServerDetails->addWidget(txtServerDetailsHost, 0, 1, 1, 1);
    layoutServerDetails->addWidget(lblServerDetailsCity, 1, 0, 1, 1);
    layoutServerDetails->addWidget(txtServerDetailsCity, 1, 1, 1, 1);
    layoutServerDetails->addWidget(lblServerDetailsCountry, 2, 0, 1, 1);
    layoutServerDetails->addWidget(txtServerDetailsCountry, 2, 1, 1, 1);
    layoutServerDetails->addWidget(lblServerDetailsTechnologies, 3, 0, 1, 1);
    layoutServerDetails->addWidget(lstServerDetailsTechnologies, 3, 1, 2, 1);
    layoutServerDetails->addItem(new QSpacerItem(0, 30, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 5, 0, 1, 2);
    layoutServerDetails->setContentsMargins(0, 0, 0, 0);
    layoutServerDetails->setColumnMinimumWidth(0, 130);

    QWidget *wxServerDetails = new QWidget();
    wxServerDetails->setLayout(layoutServerDetails);

    mdlServers = new VPNServerModel();
    vwServers = new QTableView();
    vwServers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vwServers->horizontalHeader()->setStretchLastSection(true);
    QObject::connect(mdlServers, &QAbstractTableModel::dataChanged, this, [this] {
        titleConnnectionServerList->setTitle(tr("titleConnnectionServerListCount").arg(mdlServers->rowCount()));
        vwServers->resizeColumnsToContents();
        svgSpinnerServerRefresh->setVisible(false);
        pbRefreshServers->setVisible(true);
    });

    mdlServersProxy = new QSortFilterProxyModel();
    mdlServersProxy->setSourceModel(mdlServers);
    mdlServersProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mdlServersProxy->setFilterKeyColumn(-1);

    vwServers->setModel(mdlServersProxy);
    vwServers->setSortingEnabled(true);
    vwServers->sortByColumn(2, Qt::AscendingOrder);
    vwServers->setSelectionBehavior(QAbstractItemView::SelectRows);
    vwServers->setSelectionMode(QAbstractItemView::SingleSelection);
    vwServers->setHorizontalScrollMode(QTableView::ScrollPerPixel);
    vwServers->setVerticalScrollMode(QTableView::ScrollPerPixel);
    vwServers->verticalHeader()->hide();
    QObject::connect(vwServers->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this, txtServerDetailsHost, txtServerDetailsCity, txtServerDetailsCountry, mdlServerDetailsTechnologies] {
        tbConnectServer->setEnabled(false);
        txtServerDetailsHost->setText("");
        txtServerDetailsCountry->setText("");
        txtServerDetailsCity->setText("");
        mdlServerDetailsTechnologies->setStringList(QStringList());

        QItemSelection proxySelect = vwServers->selectionModel()->selection();
        if (proxySelect.length() > 0)
        {
            QModelIndexList objIndexes = mdlServersProxy->mapSelectionToSource(proxySelect).indexes();
            VPNServer vpnServer = mdlServers->getServer(objIndexes.at(0).row());

            txtServerDetailsHost->setText(vpnServer.getHost());
            txtServerDetailsCountry->setText(vpnServer.getCountry());
            txtServerDetailsCity->setText(vpnServer.getCity());
            mdlServerDetailsTechnologies->setStringList(vpnServer.getTechnologies());
            tbConnectServer->setEnabled(true);
        }
    });

    svgSpinnerServerRefresh = new QSvgWidget();
    svgSpinnerServerRefresh->load(QString(":/img/spinner.svg"));
    svgSpinnerServerRefresh->setFixedWidth(18);
    svgSpinnerServerRefresh->setFixedHeight(18);
    svgSpinnerServerRefresh->setGeometry(0, 0, 18, 18);
    svgSpinnerServerRefresh->setStyleSheet("background: transparent;");
    svgSpinnerServerRefresh->setVisible(true);

    QFont faButtonRefresh = QFont();
    faButtonRefresh.setFamily("FontAwesome");
    faButtonRefresh.setPixelSize(16);

    pbRefreshServers = new QPushButton();
    pbRefreshServers->setFont(faButtonRefresh);
    pbRefreshServers->setText("\uf021");
    pbRefreshServers->setFlat(true);
    pbRefreshServers->setMinimumWidth(20);
    pbRefreshServers->setMaximumWidth(20);
    pbRefreshServers->setVisible(false);
    QObject::connect(pbRefreshServers, &QPushButton::clicked, this, [this] {
        svgSpinnerServerRefresh->setVisible(true);
        pbRefreshServers->setVisible(false);
        titleConnnectionServerList->setTitle(tr("titleConnnectionServerList"));
        mdlServers->reload();
    });

    QList<QWidget*> *serverTitleControls = new QList<QWidget*>();
    serverTitleControls->append(svgSpinnerServerRefresh);
    serverTitleControls->append(pbRefreshServers);
    titleConnnectionServerList->addControls(*serverTitleControls);

    txtFilterServer = new QLineEdit();
    txtFilterServer->setPlaceholderText(tr("txtFilter"));
    txtFilterServer->setEnabled(false);
    QObject::connect(txtFilterServer, &QLineEdit::textChanged, [this] {
        QString filter = txtFilterServer->text().trimmed().toLower();
        mdlServersProxy->setFilterFixedString(filter);
    });

    QLabel *lblConnectServer = new QLabel(tr("lblConnectServer"));
    lblConnectServer->setStyleSheet("font-size: 22px; font-weight: bold; color: " + lblStatus->palette().highlight().color().name() + ";");
    tbConnectServer = new ToggleButton(16, 20, true);
    tbConnectServer->overrideTextColor(true, QColor::fromRgb(180, 239, 180));
    tbConnectServer->overrideTrackColor(true, QColor::fromRgb(180, 239, 180));
    tbConnectServer->overrideThumbColor(true, QColor::fromRgb(58, 206, 58));
    tbConnectServer->overrideTextColor(false, QColor::fromRgb(236, 180, 180));
    tbConnectServer->overrideTrackColor(false, QColor::fromRgb(236, 180, 180));
    tbConnectServer->overrideThumbColor(false, QColor::fromRgb(206, 58, 58));
    tbConnectServer->overrideOpacity(1);
    tbConnectServer->setEnabled(false);
    QObject::connect(tbConnectServer, &ToggleButton::clicked, [this]() {
        QItemSelection proxySelect = vwServers->selectionModel()->selection();
        QModelIndexList objIndexes = mdlServersProxy->mapSelectionToSource(proxySelect).indexes();
        VPNServer vpnServer = mdlServers->getServer(objIndexes.at(0).row());
        QString server = vpnServer.getHost().left(vpnServer.getHost().indexOf("."));
        menu->select(0);
        toggleVpn(QStringList() << "connect" << server, tbConnectServer->isChecked(), CONNECTION_TRIGGER_SERVER);
    });
    QHBoxLayout *layoutConnectServer = new QHBoxLayout();
    layoutConnectServer->setContentsMargins(0, 0, 0, 50);
    layoutConnectServer->addWidget(lblConnectServer);
    layoutConnectServer->addStretch(1);
    layoutConnectServer->addWidget(tbConnectServer);

    QVBoxLayout *layoutServers = new QVBoxLayout();
    layoutServers->setContentsMargins(0, 0, 0, 0);
    layoutServers->addItem(layoutConnectServer);
    layoutServers->addWidget(titleConnnectionServerDetails);
    layoutServers->addWidget(wxServerDetails);
    layoutServers->addWidget(titleConnnectionServerList);
    layoutServers->addWidget(vwServers);
    layoutServers->addWidget(txtFilterServer);
    layoutServers->setStretch(4, 1);

    QWidget *wxServers = new QWidget();
    wxServers->setLayout(layoutServers);

    /**
     * -------------------------------------------
     *  Button Controls
     */

    QFont faButtonControls = QFont();
    faButtonControls.setFamily("FontAwesome");
    faButtonControls.setPixelSize(15);

    pbQuit = new QPushButton();
    pbQuit->setToolTip(tr("pbQuitToolTip"));
    pbQuit->setFlat(true);
    pbQuit->setFont(faButtonControls);
    pbQuit->setText("\uf011");
    pbQuit->setMinimumWidth(20);
    pbQuit->setMaximumWidth(20);
    QObject::connect(pbQuit, &QPushButton::clicked, [this]()
    {
        this->quit(EXIT_CODE_NORMAL);
    });

    pbMinimize = new QPushButton();
    pbMinimize->setToolTip(tr("pbMinimizeToolTip"));
    pbMinimize->setFlat(true);
    pbMinimize->setFont(faButtonControls);
    pbMinimize->setText("\uf2d1");
    pbMinimize->setMinimumWidth(20);
    pbMinimize->setMaximumWidth(20);
    QObject::connect(pbMinimize, &QPushButton::clicked, [this]()
    {
        this->hideUi();
    });

    pbSettings = new QPushButton();
    pbSettings->setToolTip(tr("pbSettingsToolTip"));
    pbSettings->setFlat(true);
    pbSettings->setFont(faButtonControls);
    pbSettings->setText("\uf013");
    pbSettings->setMinimumWidth(20);
    pbSettings->setMaximumWidth(20);
    QObject::connect(pbSettings, &QPushButton::clicked, [this]()
    {
        // show dialog
        while (updatingStatus) {
            // wait
        }
        updatingStatus = true;
        Settings *dlgSettings = new Settings(trayIcon, ui);
        dlgSettings->setAttribute(Qt::WA_DeleteOnClose);
        dlgSettings->setModal(true);
        dlgSettings->show();
        dlgSettings->activateWindow();
        dlgSettings->raise();

        ui->activateWindow();
        ui->raise();

        // reload server types
        svgSpinnerServerRefresh->setVisible(true);
        pbRefreshServers->setVisible(false);
        titleConnnectionServerList->setTitle(tr("titleConnnectionServerList"));
        mdlServers->reload();

        // reload lists
        setupDataTypes();
        setupDataCountries();
        updatingStatus = false;
    });

    QHBoxLayout *layoutButtons = new QHBoxLayout();
    layoutButtons->setContentsMargins(0, 0, 0, 0);
    layoutButtons->addStretch(1);
    layoutButtons->addWidget(pbMinimize);
    layoutButtons->addSpacing(20);
    layoutButtons->addWidget(pbSettings);
    layoutButtons->addSpacing(20);
    layoutButtons->addWidget(pbQuit);
    layoutButtons->addStretch(1);

    /**
     * -------------------------------------------
     *  create main application window
     */

    ui = new KompassWindow();

    /**
     * -------------------------------------------
     *  add status, types, and servers to main stack
     */

    stackMain = new QStackedWidget();
    stackMain->setContentsMargins(0, 25, 20, 20);
    stackMain->addWidget(wxStatus);
    stackMain->addWidget(wxTypes);
    stackMain->addWidget(wxCountries);
    stackMain->addWidget(wxServers);

    menu = new KompassMenu(stackMain);
    menu->appendMenuItem("\uf0e4", tr("mnStatus"), 0);
    menu->appendMenuItem("\uf07c", tr("mnTypes"), 1);
    menu->appendMenuItem("\uf024", tr("mnCountries"), 2);
    menu->appendMenuItem("\uf233", tr("mnServers"), 3);
    menu->select(0);

    QVBoxLayout *layoutMenu = new QVBoxLayout();
    layoutMenu->setContentsMargins(0, 28, 0, 20);
    layoutMenu->addWidget(menu);
    layoutMenu->addStretch(1);
    layoutMenu->addItem(layoutButtons);

    QWidget *wxMenuWrapper = new QWidget();
    wxMenuWrapper->setMinimumWidth(200);
    wxMenuWrapper->setMaximumWidth(200);
    wxMenuWrapper->setAutoFillBackground(true);
    wxMenuWrapper->setStyleSheet("background: " + this->ui->palette().light().color().name() + ";");
    wxMenuWrapper->setLayout(layoutMenu);

    /**
     * -------------------------------------------
     *  add menu and stack to central widget
     */

    QHBoxLayout *layoutCentral = new QHBoxLayout();
    layoutCentral->setContentsMargins(0, 0, 0, 0);
    layoutCentral->addWidget(wxMenuWrapper);
    layoutCentral->addSpacing(20);
    layoutCentral->addWidget(stackMain);

    QWidget *wxCentral = new QWidget();
    wxCentral->setContentsMargins(0, 0, 0, 0);
    wxCentral->setLayout(layoutCentral);

    /**
     * -------------------------------------------
     *  set central window
     *  main class takes care of showing the application
     */

    ui->setWindowTitle(tr("appTitle"));
    ui->setCentralWidget(wxCentral);
}

/**
 * Create a Tray Menu
 * @brief Kompass::setupTray
 */
void Kompass::setupTray()
{
    // create tray icon and register listeners
    trayIcon = new TrayIcon(this->ui);
    systemTrayIcon = new QSystemTrayIcon(trayIcon->getIcon(false), this);
    QObject::connect(systemTrayIcon, &QSystemTrayIcon::activated, [this]() {
        showUi();
    });

    // create connect menu item
    actionConnect = new QAction(tr("trayActionConnect"), systemTrayIcon);
    QObject::connect(actionConnect, &QAction::triggered, [this]() {
        toggleVpn(QStringList() << "connect", true, CONNECTION_TRIGGER_FASTEST);
    });

    // create connect menu item
    actionDisconnect = new QAction(tr("trayActionDisconnect"), systemTrayIcon);
    QObject::connect(actionDisconnect, &QAction::triggered, [this]() {
        toggleVpn(QStringList(), false, CONNECTION_TRIGGER_FASTEST);
    });

    // create show menu item
    QAction *actionShow = new QAction(tr("trayActionShow"), systemTrayIcon);
    QObject::connect(actionShow, &QAction::triggered, [this]() {
        showUi();
    });

    // create quit menu item
    QAction *actionQuit = new QAction(tr("trayActionQuit"), systemTrayIcon);
    QObject::connect(actionQuit, &QAction::triggered, [this]() {
        quit(EXIT_CODE_NORMAL);
    });

    mnStatusConnection = new QWidgetAction(this);
    mnStatusConnection->setText(tr("statusDisconnected"));
    mnStatusConnection->setIcon(trayIcon->getIcon(false));
    mnStatusConnection->setEnabled(false);
    mnStatusConnection->setVisible(true);

    mnStatusUploaded = new QWidgetAction(this);
    mnStatusUploaded->setText("--");
    mnStatusUploaded->setIcon(QIcon(":/img/arrow-up.png"));
    mnStatusUploaded->setEnabled(false);
    mnStatusUploaded->setVisible(false);

    mnStatusDownloaded = new QWidgetAction(this);
    mnStatusDownloaded->setText("--");
    mnStatusDownloaded->setIcon(QIcon(":/img/arrow-down.png"));
    mnStatusDownloaded->setEnabled(false);
    mnStatusDownloaded->setVisible(false);

    mnStatusUptime = new QWidgetAction(this);
    mnStatusUptime->setText("--");
    mnStatusUptime->setIcon(QIcon(":/img/clock.png"));
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
    systemTrayIcon->setContextMenu(trayIconMenu);

    // show the menu icon
    systemTrayIcon->show();
}

/**
 * Load server lists to display
 * @brief Kompass::setupData
 */
void Kompass::setupData()
{
    setupDataAccount();
    setupDataTypes();
    setupDataCountries();
}

/**
 * Load account details
 * @brief Kompass::setupDataTypes
 */
void Kompass::setupDataAccount()
{
    QProcess *commandAccount = new QProcess();
    commandAccount->start("nordvpn", QStringList() << "account");
    commandAccount->waitForFinished();

    QString outputAccount = commandAccount->readAllStandardOutput();
    if (outputAccount != nullptr && outputAccount.length())
    {
        QStringList lines = outputAccount.trimmed().split(QString("\n"));
        for (QString line : lines) {
            if (line.trimmed().toLower().startsWith("email")) {
                QString username = line.mid(line.indexOf(":") + 1).trimmed();
                txtUsername->setText(username);
            }
            if (line.trimmed().toLower().startsWith("vpn service")) {
                QString service = line.mid(line.indexOf(":") + 1).trimmed();
                txtLicense->setText(service);
            }
        }
    }
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
        if (outputGroups.contains("new feature", Qt::CaseInsensitive))
        {
            outputGroups = outputGroups.mid(outputGroups.indexOf("feature"));
            outputGroups = outputGroups.mid(outputGroups.indexOf("\r"));
        }

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
        tbConnectType->setEnabled(false);
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
        if (outputCountries.contains("new feature", Qt::CaseInsensitive))
        {
            outputCountries = outputCountries.mid(outputCountries.indexOf("feature"));
            outputCountries = outputCountries.mid(outputCountries.indexOf("\r"));
        }

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
        tbConnectCountry->setEnabled(false);
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
        if (this->updatingStatus || this->currentStatus == STATUS_CONNECTING  || this->currentStatus == STATUS_DISCONNECTING)
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
            updateUi(STATUS_DISCONNECTED, -1, nullptr);
        }
        else if (output.toLower().contains("connected"))
        {
            updateUi(STATUS_CONNECTED, -1, output);
        }
    });
}

/**
 * Monitors Github for new Kompass Releases
 * @brief Kompass::setupReleaseMonitor
 */
void Kompass::setupReleaseMonitor()
{
    ReleaseMonitor *monitor = new ReleaseMonitor();

    // handle update available
    connect(monitor, &ReleaseMonitor::updateAvailable, [this, monitor] {
        lblVersionUpdate->setStyleSheet("font-size: 15px; color: rgb(58, 206, 58); background: rgb(235, 250, 235); padding: 10px;");
        lblVersionUpdate->setText(tr("appVersionUpgrade").arg(monitor->getUpdatedReleaseVersion(), monitor->getUpdatedReleaseUrl()));

        if (!monitor->needsSudo() || (monitor->needsSudo() && monitor->isSudoer()))
        {
            Installer *dlgInstaller = new Installer(monitor, ui);
            dlgInstaller->setAttribute(Qt::WA_DeleteOnClose);
            dlgInstaller->setModal(true);
            dlgInstaller->show();
            dlgInstaller->activateWindow();
            dlgInstaller->raise();

            ui->activateWindow();
            ui->raise();
        }
    });

    // start timer and trigger once
    monitor->start(tr("appVersion"));
    monitor->evaluate();
}

/**
 * dis-/connects theVPN
 * @brief Kompass::toggleVpn
 * @param commands
 */
void Kompass::toggleVpn(QStringList commands, bool connect, int trigger)
{

    updateUi(Kompass::STATUS_DISABLED, trigger);
    svgSpinner->setVisible(true);

    ConnectionResult *cnResult = new ConnectionResult();
    if (connect)
    {
        updateUi(Kompass::STATUS_CONNECTING, trigger);
        QThread *thread = QThread::create([this, commands, cnResult] {
            ConnectionResult *cnnct = connectVpn(commands);
            cnResult->setSuccessful(cnnct->isSuccessful());
            cnResult->setResult(cnnct->getResult());
        });
        QObject::connect(thread, &QThread::finished, this, [this, cnResult, trigger] {
            if (!cnResult->isSuccessful() && cnResult->getResult().trimmed().length() > 0)
            {
                updateUi(Kompass::STATUS_DISCONNECTED, trigger);
                menu->select(trigger);

                QString err = cnResult->getResult().trimmed();
                err = err.replace(QRegularExpression("[\\]|[\|]|[/]|[-]"), QString());

                if (trigger == CONNECTION_TRIGGER_SERVER)
                {
                    err = err.prepend(tr("msgErrorConnectingByServer"));
                }
                else
                {
                    err = err.prepend(tr("msgErrorConnecting"));
                }

                QMessageBox msg = QMessageBox(ui);
                msg.setWindowIcon(QIcon(":/img/kompass.png"));
                msg.setWindowTitle(tr("dlgErrorTitle"));
                msg.setText(err);
                msg.setIcon(QMessageBox::Warning);
                msg.setDefaultButton(QMessageBox::Ok);
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setWindowModality(Qt::WindowModality::ApplicationModal);
                msg.activateWindow();
                msg.show();
                msg.exec();
            }
            else if (!cnResult->isSuccessful())
            {
                // should never happen
                updateUi(Kompass::STATUS_DISCONNECTED, trigger);
            }
            else
            {
                updateUi(Kompass::STATUS_CONNECTED, trigger);
            }
        });
        thread->start();
    }
    else
    {
        updateUi(Kompass::STATUS_DISCONNECTING, trigger);
        QThread *thread = QThread::create([this, commands, cnResult]{
            ConnectionResult *dscnnct = disconnectVpn();
            cnResult->setSuccessful(dscnnct->isSuccessful());
            cnResult->setResult(dscnnct->getResult());
        });
        QObject::connect(thread, &QThread::finished, this, [this, cnResult, trigger] {
            if (!cnResult->isSuccessful() && cnResult->getResult().trimmed().length() > 0)
            {
                updateUi(Kompass::STATUS_CONNECTED, trigger);

                QMessageBox msg = QMessageBox(ui);
                msg.setWindowIcon(QIcon(":/img/kompass.png"));
                msg.setWindowTitle(tr("dlgErrorTitle"));
                msg.setText(tr("msgErrorDisconnecting") + cnResult->getResult().trimmed());
                msg.setIcon(QMessageBox::Warning);
                msg.setDefaultButton(QMessageBox::Ok);
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setWindowModality(Qt::WindowModality::ApplicationModal);
                msg.activateWindow();
                msg.show();
                msg.exec();
            }
            else if (!cnResult->isSuccessful())
            {
                // should never happen
                updateUi(Kompass::STATUS_CONNECTED, trigger);
            }
            else
            {
                updateUi(Kompass::STATUS_DISCONNECTED, trigger);
            }
        });
        thread->start();
    }
}

/**
 * Calls NordVPN binary to connect using the given list
 * if commands. If no commands are given, Kompass will
 * connect to (what NordVPN considers) the fastest server.
 * @brief Kompass::connectVpn
 * @param commands
 * @return result of the connection attempt
 */
ConnectionResult* Kompass::connectVpn(QStringList commands)
{
    // Update UI
    while (updatingStatus) {
        // wait
    }

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

    ConnectionResult *connectionResult = new ConnectionResult();
    QString output = command->readAllStandardOutput();
    QString error = command->readAllStandardError();

    if (error == nullptr || (error != nullptr && error.trimmed().length() == 0))
    {
        if (output != nullptr && !output.toLower().contains("connected"))
        {
            if (output.contains("new feature", Qt::CaseInsensitive))
            {
                output = output.mid(output.indexOf("feature"));
                output = output.mid(output.indexOf("\r")).trimmed();
            }
            connectionResult->setSuccessful(false);
            connectionResult->setResult(output.trimmed());
        }
        else
        {
            connectionResult->setSuccessful(true);
        }
    }
    else
    {
        if (error.contains("new feature", Qt::CaseInsensitive))
        {
            error = error.mid(error.indexOf("feature"));
            error = error.mid(error.indexOf("\r")).trimmed();
        }

        connectionResult->setSuccessful(false);
        connectionResult->setResult(error);
    }

    return connectionResult;
}

/**
 * Disconnect the current VPN Connection
 * @brief Kompass::disconnectVpn
 * @return result of the disconnect attempt
 */
ConnectionResult* Kompass::disconnectVpn()
{
    // Update UI
    while (updatingStatus) {
        // wait
    }

    // execute command
    QProcess *command = new QProcess();
    command->start("nordvpn", QStringList() << "disconnect");
    command->waitForFinished();

    ConnectionResult *connectionResult = new ConnectionResult();
    QString output = command->readAllStandardOutput();
    QString error = command->readAllStandardError();

    if (error == nullptr || (error != nullptr && error.trimmed().length() == 0))
    {
        connectionResult->setSuccessful(true);
    }
    else
    {
        connectionResult->setSuccessful(false);
        connectionResult->setResult(error);
    }

    return connectionResult;
}

/**
 * Updates the Connection Status Display
 * Enables/disables Buttons based on Connection Status
 * @brief Kompass::setStatus
 * @param status
 * @param vpnDetails
 */
void Kompass::updateUi(int status, int trigger, QString vpnDetails)
{
    updatingStatus = true;

    // these buttons are always active
    pbQuit->setEnabled(true);
    pbMinimize->setEnabled(true);

    // get list selection
    QModelIndexList selectedTypesIndex = lstServersByType->selectionModel()->selectedIndexes();
    QModelIndexList selectedCountriesIndex = lstServersByCountry->selectionModel()->selectedIndexes();
    QModelIndexList selectedServersIndex = vwServers->selectionModel()->selectedIndexes();

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

                if (uptime.contains("day", Qt::CaseInsensitive))
                {
                    QString d = uptime.left(uptime.toLower().indexOf("day") - 1);
                    d = d.mid(d.lastIndexOf(" "));
                    d = d.replace(QRegularExpression("\\D+"), QString());
                    hrs += 24 * d.toInt();
                }

                if (uptime.contains("hour", Qt::CaseInsensitive))
                {
                     QString h = uptime.left(uptime.toLower().indexOf("hour") - 1);
                     h = h.mid(h.lastIndexOf(" "));
                     h = h.replace(QRegularExpression("\\D+"), QString());
                     hrs += h.toInt();
                }

                if (uptime.contains("minute", Qt::CaseInsensitive))
                {
                    QString m = uptime.left(uptime.toLower().indexOf("minute") - 1);
                    m = m.mid(m.lastIndexOf(" "));
                    m = m.replace(QRegularExpression("\\D+"), QString());
                    min += m.toInt();
                }

                if (uptime.contains("second", Qt::CaseInsensitive))
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
        case STATUS_DISCONNECTING:
            svgSpinner->setVisible(true);
            systemTrayIcon->setIcon(trayIcon->getIcon(false));
            mnStatusUploaded->setVisible(false);
            mnStatusDownloaded->setVisible(false);
            mnStatusUptime->setVisible(false);
            mnStatusConnection->setIcon(trayIcon->getIcon(false));
            mnStatusConnection->setText(tr("statusConnecting"));
            mnStatusUploaded->setText("--");
            mnStatusDownloaded->setText("--");
            tbStatus->setChecked(false);
            tbStatus->setEnabled(false);
            tbConnectType->setChecked(false);
            tbConnectType->setEnabled(false);
            tbConnectCountry->setChecked(false);
            tbConnectCountry->setEnabled(false);
            tbConnectServer->setChecked(false);
            tbConnectServer->setEnabled(false);
            txtStatusServer->setText("--");
            txtStatusCity->setText("--");
            txtStatusCountry->setText("--");
            txtStatusProtocol->setText("--");
            txtStatusUploaded->setText("--");
            txtStatusDownloaded->setText("--");
            txtStatusUptime->setText("--");
            actionConnect->setEnabled(false);
            actionDisconnect->setEnabled(false);
            lstServersByType->setEnabled(false);
            lstServersByCountry->setEnabled(false);
            vwServers->setEnabled(false);
            txtFilterType->setEnabled(false);
            txtFilterCountry->setEnabled(false);
            txtFilterServer->setEnabled(false);
            pbSettings->setEnabled(false);
            break;
        case STATUS_DISCONNECTED:
            systemTrayIcon->setIcon(trayIcon->getIcon(false));
            mnStatusUploaded->setVisible(false);
            mnStatusDownloaded->setVisible(false);
            mnStatusUptime->setVisible(false);
            mnStatusConnection->setIcon(trayIcon->getIcon(false));
            mnStatusConnection->setText(tr("statusDisconnected"));
            mnStatusUploaded->setText("--");
            mnStatusDownloaded->setText("--");
            tbStatus->setChecked(false);
            tbStatus->setEnabled(true);
            txtStatusServer->setText("--");
            txtStatusCity->setText("--");
            txtStatusCountry->setText("--");
            txtStatusProtocol->setText("--");
            txtStatusUploaded->setText("--");
            txtStatusDownloaded->setText("--");
            txtStatusUptime->setText("--");
            txtFilterType->setEnabled(true);
            txtFilterCountry->setEnabled(true);
            txtFilterServer->setEnabled(true);
            lstServersByType->setEnabled(true);
            lstServersByCountry->setEnabled(true);
            vwServers->setEnabled(true);
            tbConnectType->setChecked(false);
            tbConnectType->setEnabled(selectedTypesIndex.length() > 0);
            tbConnectCountry->setChecked(false);
            tbConnectCountry->setEnabled(selectedCountriesIndex.length() > 0);
            tbConnectServer->setChecked(false);
            tbConnectServer->setEnabled(selectedServersIndex.length() > 0);
            actionConnect->setEnabled(true);
            actionDisconnect->setEnabled(false);
            pbSettings->setEnabled(true);
            svgSpinner->setVisible(false);
            break;
        case STATUS_CONNECTED:
            systemTrayIcon->setIcon(trayIcon->getIcon(true));
            mnStatusConnection->setIcon(trayIcon->getIcon(true));
            mnStatusConnection->setText(tr("statusConnected"));
            tbStatus->setChecked(true);
            tbStatus->setEnabled(true);
            if (trigger != -1)
            {
                tbConnectType->setEnabled(trigger == CONNECTION_TRIGGER_TYPE);
                tbConnectType->setChecked(trigger == CONNECTION_TRIGGER_TYPE);
                tbConnectCountry->setEnabled(trigger == CONNECTION_TRIGGER_COUNTRY);
                tbConnectCountry->setChecked(trigger == CONNECTION_TRIGGER_COUNTRY);
                tbConnectServer->setEnabled(trigger == CONNECTION_TRIGGER_SERVER);
                tbConnectServer->setChecked(trigger == CONNECTION_TRIGGER_SERVER);
            }
            if (vpnStatus->value("server").length() > 0) {
                txtStatusServer->setText(vpnStatus->value("server"));
            }
            if (vpnStatus->value("city").length() > 0) {
                txtStatusCity->setText(vpnStatus->value("city"));
            }
            if (vpnStatus->value("country").length() > 0) {
                txtStatusCountry->setText(vpnStatus->value("country"));
                mnStatusConnection->setText(tr("statusConnected") + ": " + vpnStatus->value("country"));
            }
            if (vpnStatus->value("protocol").length() > 0 && vpnStatus->value("technology").length() > 0) {
                txtStatusProtocol->setText(vpnStatus->value("protocol") + " [" + vpnStatus->value("technology") + "]");
            }
            if (vpnStatus->value("uploaded").length() > 0) {
                txtStatusUploaded->setText(vpnStatus->value("uploaded"));
                mnStatusUploaded->setText(vpnStatus->value("uploaded"));
                mnStatusUploaded->setVisible(true);
            }
            if (vpnStatus->value("downloaded").length() > 0) {
                txtStatusDownloaded->setText(vpnStatus->value("downloaded"));
                mnStatusDownloaded->setText(vpnStatus->value("downloaded"));
                mnStatusDownloaded->setVisible(true);
            }
            if (vpnStatus->value("uptime").length() > 0) {
                txtStatusUptime->setText(vpnStatus->value("uptime"));
                mnStatusUptime->setText(vpnStatus->value("uptime"));
                mnStatusUptime->setVisible(true);
            }
            actionDisconnect->setEnabled(true);
            actionConnect->setEnabled(false);
            lstServersByType->setEnabled(false);
            lstServersByCountry->setEnabled(false);
            txtFilterType->setEnabled(false);
            txtFilterCountry->setEnabled(false);
            txtFilterServer->setEnabled(false);
            pbSettings->setEnabled(true);
            svgSpinner->setVisible(false);
            break;
        case STATUS_DISABLED:
            tbStatus->setEnabled(false);
            actionConnect->setEnabled(false);
            actionDisconnect->setEnabled(false);
            tbConnectType->setEnabled(false);
            tbConnectCountry->setEnabled(false);
            tbConnectServer->setEnabled(false);
            lstServersByType->setEnabled(false);
            lstServersByCountry->setEnabled(false);
            vwServers->setEnabled(false);
            txtFilterType->setEnabled(false);
            txtFilterCountry->setEnabled(false);
            txtFilterServer->setEnabled(false);
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
    if (ui->isVisible())
    {
        ui->setVisible(false);
    }

    ui->setVisible(true);
    ui->show();
    ui->setWindowState(Qt::WindowActive);
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
