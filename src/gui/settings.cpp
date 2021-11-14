#include "settings.h"

Settings::Settings()
{
    setupUi();
    setupData();
}

/**
 * Setup dialog elements
 * @brief Settings::setupUi
 */
void Settings::setupUi()
{
    /**
     * -------------------------------------------
     *  set dialog basics
     */

    this->setWindowTitle(tr("dialogTitle"));
    this->setWindowIcon(QIcon::fromTheme("compass"));
    this->setContentsMargins(0, 0, 0, 0);
    this->setMinimumSize(800, 494);
    this->setMaximumSize(800, 494);
    this->resize(800,494);
    this->setWindowModality(Qt::WindowModality::ApplicationModal);

    /**
     * -------------------------------------------
     *  checkbox and dropdown controls
     */

    cmbTechnology = new QComboBox();
    QObject::connect(cmbTechnology, &QComboBox::currentTextChanged, [this]() {
        QString result = saveSettings(QStringList() << "set" << "technology" << cmbTechnology->currentText());
        if (result.length() > 0)
        {
            cmbTechnology->blockSignals(true);
            cmbTechnology->setCurrentText(technology);
            cmbTechnology->blockSignals(false);
            displayError(result.prepend(tr("errorPrefixTechnology")));
        }
        else
        {
            technology = cmbTechnology->currentText();
            displaySuccess();
            setupData();
        }
    });
    cmbProtocol = new QComboBox();
    QObject::connect(cmbProtocol, &QComboBox::currentTextChanged, [this]() {
        QString result = saveSettings(QStringList() << "set" << "protocol" << cmbProtocol->currentText());
        if (result.length() > 0)
        {
            cmbProtocol->blockSignals(true);
            cmbProtocol->setCurrentText(protocol);
            cmbProtocol->blockSignals(false);
            displayError(result.prepend(tr("errorPrefixProtocol")));
        }
        else
        {
            protocol = cmbProtocol->currentText();
            displaySuccess();
            setupData();
        }
    });

    cbFirewall = new ToggleButton(8, 10);
    QObject::connect(cbFirewall, &ToggleButton::clicked, [this]() {
        QString result = saveSettings(QStringList() << "set" << "firewall" << ((cbFirewall->isChecked()) ? "enabled" : "disabled"));
        if (result.length() > 0)
        {
            cbFirewall->setChecked(!cbFirewall->isChecked());
            displayError(result.prepend(tr("errorPrefixFirewall")));
        }
        else
        {
            displaySuccess();
            setupData();
        }
    });

    cbCybersec = new ToggleButton(8, 10);
    QObject::connect(cbCybersec, &ToggleButton::clicked, [this]() {
        QString result = saveSettings(QStringList() << "set" << "cybersec" << ((cbCybersec->isChecked()) ? "enabled" : "disabled"));
        if (result.length() > 0)
        {
            cbCybersec->setChecked(!cbCybersec->isChecked());
            displayError(result.prepend(tr("errorPrefixCybersec")));
        }
        else
        {
            displaySuccess();
            setupData();
        }
    });

    cbObfuscate = new ToggleButton(8, 10);
    QObject::connect(cbObfuscate, &ToggleButton::clicked, [this]() {
        QString result = saveSettings(QStringList() << "set" << "obfuscate" << ((cbObfuscate->isChecked()) ? "enabled" : "disabled"));
        if (result.length() > 0)
        {
            cbObfuscate->setChecked(!cbObfuscate->isChecked());
            displayError(result.prepend(tr("errorPrefixObfuscate")));
        }
        else
        {
            displaySuccess();
            setupData();
        }
    });

    cbNotify = new ToggleButton(8, 10);
    QObject::connect(cbNotify, &ToggleButton::clicked, [this]() {
        QString result = saveSettings(QStringList() << "set" << "notify" << ((cbNotify->isChecked()) ? "enabled" : "disabled"));
        if (result.length() > 0)
        {
            cbNotify->setChecked(!cbNotify->isChecked());
            displayError(result.prepend(tr("errorPrefixNotify")));
        }
        else
        {
            displaySuccess();
            setupData();
        }
    });

    cbAutoconnect = new ToggleButton(8, 10);
    QObject::connect(cbAutoconnect, &ToggleButton::clicked, [this]() {
        QString result = saveSettings(QStringList() << "set" << "autoconnect" << ((cbAutoconnect->isChecked()) ? "enabled" : "disabled"));
        if (result.length() > 0)
        {
            cbAutoconnect->setChecked(!cbAutoconnect->isChecked());
            displayError(result.prepend(tr("errorPrefixAutoconnect")));
        }
        else
        {
            displaySuccess();
            setupData();
        }
    });

    cbIPv6 = new ToggleButton(8, 10);
    QObject::connect(cbIPv6, &ToggleButton::clicked, [this]() {
        QString result = saveSettings(QStringList() << "set" << "ipv6" << ((cbIPv6->isChecked()) ? "enabled" : "disabled"));
        if (result.length() > 0)
        {
            cbIPv6->setChecked(!cbIPv6->isChecked());
            displayError(result.prepend(tr("errorPrefixIPv6")));
        }
        else
        {
            displaySuccess();
            setupData();
        }
    });

    cbKillswitch = new ToggleButton(8, 10);
    QObject::connect(cbKillswitch, &ToggleButton::clicked, [this]() {
        QString result = saveSettings(QStringList() << "set" << "killswitch" << ((cbKillswitch->isChecked()) ? "enabled" : "disabled"));
        if (result.length() > 0)
        {
            cbKillswitch->setChecked(!cbKillswitch->isChecked());
            displayError(result.prepend(tr("errorPrefixKillswitch")));
        }
        else
        {
            displaySuccess();
            setupData();
        }
    });

    /**
     * -------------------------------------------
     *  Settings: Connection
     */

    QLabel *lblTechnology = new QLabel(tr("lblTechnology"));
    lblTechnology->setStyleSheet("font-weight: bold;");
    lblTechnology->setMinimumWidth(160);
    QLabel *lblProtocol = new QLabel(tr("lblProtocol"));
    lblProtocol->setStyleSheet("font-weight: bold;");
    lblProtocol->setMinimumWidth(160);

    QHBoxLayout *layoutTechnology = new QHBoxLayout();
    layoutTechnology->addWidget(lblTechnology);
    layoutTechnology->addWidget(cmbTechnology);
    layoutTechnology->setStretch(1, 1);

    QHBoxLayout *layoutProtocol = new QHBoxLayout();
    layoutProtocol->addWidget(lblProtocol);
    layoutProtocol->addWidget(cmbProtocol);
    layoutProtocol->setStretch(1, 1);

    QLabel *lblIPv6Toggle = new QLabel(tr("lblIPv6Toggle"));
    lblIPv6Toggle->setStyleSheet("font-weight: bold;");
    QLabel *lblIPv6Details = new QLabel(tr("lblIPv6Details"));
    lblIPv6Details->setWordWrap(true);

    QHBoxLayout *layoutIPv6Toggle = new QHBoxLayout();
    layoutIPv6Toggle->addWidget(cbIPv6);
    layoutIPv6Toggle->addSpacing(20);
    layoutIPv6Toggle->addWidget(lblIPv6Toggle);
    layoutIPv6Toggle->setStretch(2, 1);

    QLabel *lblObfuscateToggle = new QLabel(tr("lblObfuscateToggle"));
    lblObfuscateToggle->setStyleSheet("font-weight: bold;");
    QLabel *lblObfuscateDetails = new QLabel(tr("lblObfuscateDetails"));
    lblObfuscateDetails->setWordWrap(true);

    QHBoxLayout *layoutObfuscateToggle = new QHBoxLayout();
    layoutObfuscateToggle->addWidget(cbObfuscate);
    layoutObfuscateToggle->addSpacing(20);
    layoutObfuscateToggle->addWidget(lblObfuscateToggle);
    layoutObfuscateToggle->setStretch(2, 1);

    QLabel *lblAutoconnectToggle = new QLabel(tr("lblAutoconnectToggle"));
    lblAutoconnectToggle->setStyleSheet("font-weight: bold;");
    QLabel *lblAutoconnectDetails = new QLabel(tr("lblAutoconnectDetails"));
    lblAutoconnectDetails->setWordWrap(true);

    QHBoxLayout *layoutAutoconnectToggle = new QHBoxLayout();
    layoutAutoconnectToggle->addWidget(cbAutoconnect);
    layoutAutoconnectToggle->addSpacing(20);
    layoutAutoconnectToggle->addWidget(lblAutoconnectToggle);
    layoutAutoconnectToggle->setStretch(2, 1);

    QVBoxLayout *layoutConnection = new QVBoxLayout();
    layoutConnection->addItem(layoutTechnology);
    layoutConnection->addSpacing(20);
    layoutConnection->addItem(layoutProtocol);
    layoutConnection->addSpacing(20);
    layoutConnection->addItem(layoutIPv6Toggle);
    layoutConnection->addSpacing(10);
    layoutConnection->addWidget(lblIPv6Details);
    layoutConnection->addSpacing(20);
    layoutConnection->addItem(layoutObfuscateToggle);
    layoutConnection->addSpacing(10);
    layoutConnection->addWidget(lblObfuscateDetails);
    layoutConnection->addSpacing(20);
    layoutConnection->addItem(layoutAutoconnectToggle);
    layoutConnection->addSpacing(10);
    layoutConnection->addWidget(lblAutoconnectDetails);
    layoutConnection->addStretch(1);

    QWidget *wConnection = new QWidget();
    wConnection->setLayout(layoutConnection);

    mnConnection = new ClickableLabel();
    mnConnection->setText(tr("mnConnection"));
    mnConnection->setContentsMargins(20, 10, 0, 10);
    mnConnection->setAutoFillBackground(true);
    mnConnection->setStyleSheet("font-weight: normal; color: " + this->palette().light().color().name() + "; background: " + this->palette().highlight().color().name() + ";");
    QObject::connect(mnConnection, &ClickableLabel::clicked, [this]() {
        mnConnection->setStyleSheet("font-weight: normal; color: " + this->palette().light().color().name() + "; background: " + this->palette().highlight().color().name() + ";");
        mnKillswitch->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnCybersec->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnFirewall->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnNotify->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        stackSettings->setCurrentIndex(0);
    });

    /**
     * -------------------------------------------
     *  Settings: Kill Switch
     */

    QLabel *lblKillswitchToggle = new QLabel(tr("lblKillswitchToggle"));
    lblKillswitchToggle->setStyleSheet("font-weight: bold;");
    QLabel *lblKillswitchDetails = new QLabel(tr("lblKillswitchDetails"));
    lblKillswitchDetails->setWordWrap(true);

    QHBoxLayout *layoutKillswitchToggle = new QHBoxLayout();
    layoutKillswitchToggle->addWidget(cbKillswitch);
    layoutKillswitchToggle->addSpacing(20);
    layoutKillswitchToggle->addWidget(lblKillswitchToggle);
    layoutKillswitchToggle->setStretch(2, 1);

    QVBoxLayout *layoutKillswitch = new QVBoxLayout();
    layoutKillswitch->addItem(layoutKillswitchToggle);
    layoutKillswitch->addSpacing(10);
    layoutKillswitch->addWidget(lblKillswitchDetails);
    layoutKillswitch->addStretch(1);

    QWidget *wKillswitch = new QWidget();
    wKillswitch->setLayout(layoutKillswitch);

    mnKillswitch = new ClickableLabel();
    mnKillswitch->setText(tr("mnKillswitch"));
    mnKillswitch->setContentsMargins(20, 10, 0, 10);
    mnKillswitch->setAutoFillBackground(true);
    mnKillswitch->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
    QObject::connect(mnKillswitch, &ClickableLabel::clicked, [this]() {
        mnConnection->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnKillswitch->setStyleSheet("font-weight: normal; color: " + this->palette().light().color().name() + "; background: " + this->palette().highlight().color().name() + ";");
        mnCybersec->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnFirewall->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnNotify->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        stackSettings->setCurrentIndex(1);
    });

    /**
     * -------------------------------------------
     *  Settings: Cyber Securty
     */

    QLabel *lblCybersecToggle = new QLabel(tr("lblCybersecToggle"));
    lblCybersecToggle->setStyleSheet("font-weight: bold;");
    QLabel *lblCybersecDetails = new QLabel(tr("lblCybersecDetails"));
    lblCybersecDetails->setWordWrap(true);

    QHBoxLayout *layoutCybersecToggle = new QHBoxLayout();
    layoutCybersecToggle->addWidget(cbCybersec);
    layoutCybersecToggle->addSpacing(20);
    layoutCybersecToggle->addWidget(lblCybersecToggle);
    layoutCybersecToggle->setStretch(2, 1);

    QVBoxLayout *layoutCybersec = new QVBoxLayout();
    layoutCybersec->addItem(layoutCybersecToggle);
    layoutCybersec->addSpacing(10);
    layoutCybersec->addWidget(lblCybersecDetails);
    layoutCybersec->addStretch(1);

    QWidget *wCybersec = new QWidget();
    wCybersec->setLayout(layoutCybersec);

    mnCybersec = new ClickableLabel();
    mnCybersec->setText(tr("mnCybersec"));
    mnCybersec->setContentsMargins(20, 10, 0, 10);
    mnCybersec->setAutoFillBackground(true);
    mnCybersec->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
    QObject::connect(mnCybersec, &ClickableLabel::clicked, [this]() {
        mnConnection->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnKillswitch->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnCybersec->setStyleSheet("font-weight: normal; color: " + this->palette().light().color().name() + "; background: " + this->palette().highlight().color().name() + ";");
        mnFirewall->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnNotify->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        stackSettings->setCurrentIndex(2);
    });

    /**
     * -------------------------------------------
     *  Settings: Firewall
     */

    QLabel *lblFirewallToggle = new QLabel(tr("lblFirewallToggle"));
    lblFirewallToggle->setStyleSheet("font-weight: bold;");
    QLabel *lblFirewallDetails = new QLabel(tr("lblFirewallDetails"));
    lblFirewallDetails->setWordWrap(true);

    QHBoxLayout *layoutFirewallToggle = new QHBoxLayout();
    layoutFirewallToggle->addWidget(cbFirewall);
    layoutFirewallToggle->addSpacing(20);
    layoutFirewallToggle->addWidget(lblFirewallToggle);
    layoutFirewallToggle->setStretch(2, 1);

    QVBoxLayout *layoutFirewall = new QVBoxLayout();
    layoutFirewall->addItem(layoutFirewallToggle);
    layoutFirewall->addSpacing(10);
    layoutFirewall->addWidget(lblFirewallDetails);
    layoutFirewall->addStretch(1);

    QWidget *wFirewall = new QWidget();
    wFirewall->setLayout(layoutFirewall);

    mnFirewall = new ClickableLabel();
    mnFirewall->setText(tr("mnFirewall"));
    mnFirewall->setContentsMargins(20, 10, 0, 10);
    mnFirewall->setAutoFillBackground(true);
    mnFirewall->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
    QObject::connect(mnFirewall, &ClickableLabel::clicked, [this]() {
        mnConnection->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnKillswitch->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnCybersec->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnFirewall->setStyleSheet("font-weight: normal; color: " + this->palette().light().color().name() + "; background: " + this->palette().highlight().color().name() + ";");
        mnNotify->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        stackSettings->setCurrentIndex(3);
    });

    /**
     * -------------------------------------------
     *  Settings: Notifications
     */

    QLabel *lblNotifyToggle = new QLabel(tr("lblNotifyToggle"));
    lblNotifyToggle->setStyleSheet("font-weight: bold;");
    QLabel *lblNotifyDetails = new QLabel(tr("lblNotifyDetails"));
    lblNotifyDetails->setWordWrap(true);

    QHBoxLayout *layoutNotifyToggle = new QHBoxLayout();
    layoutNotifyToggle->addWidget(cbNotify);
    layoutNotifyToggle->addSpacing(20);
    layoutNotifyToggle->addWidget(lblNotifyToggle);
    layoutNotifyToggle->setStretch(2, 1);

    QVBoxLayout *layoutNotify = new QVBoxLayout();
    layoutNotify->addItem(layoutNotifyToggle);
    layoutNotify->addSpacing(10);
    layoutNotify->addWidget(lblNotifyDetails);
    layoutNotify->addStretch(1);

    QWidget *wNotify = new QWidget();
    wNotify->setLayout(layoutNotify);

    mnNotify = new ClickableLabel();
    mnNotify->setText(tr("mnNotify"));
    mnNotify->setContentsMargins(20, 10, 0, 10);
    mnNotify->setAutoFillBackground(true);
    mnNotify->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
    QObject::connect(mnNotify, &ClickableLabel::clicked, [this]() {
        mnConnection->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnKillswitch->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnCybersec->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnFirewall->setStyleSheet("font-weight: normal; color: " + this->palette().highlight().color().name() + "; background: " + this->palette().light().color().name() + ";");
        mnNotify->setStyleSheet("font-weight: normal; color: " + this->palette().light().color().name() + "; background: " + this->palette().highlight().color().name() + ";");
        stackSettings->setCurrentIndex(4);
    });

    /**
     * -------------------------------------------
     *  add setting detal panels into stack
     */

    stackSettings = new QStackedWidget();
    stackSettings->setContentsMargins(20, 25, 20, 20);
    stackSettings->addWidget(wConnection);
    stackSettings->addWidget(wKillswitch);
    stackSettings->addWidget(wCybersec);
    stackSettings->addWidget(wFirewall);
    stackSettings->addWidget(wNotify);

    /**
     * -------------------------------------------
     *  create the menu to switch active stack item
     */

    QVBoxLayout *layoutMenu = new QVBoxLayout();
    layoutMenu->setContentsMargins(0, 25, 0, 0);
    layoutMenu->addWidget(mnConnection);
    layoutMenu->addWidget(mnKillswitch);
    layoutMenu->addWidget(mnCybersec);
    layoutMenu->addWidget(mnFirewall);
    layoutMenu->addWidget(mnNotify);
    layoutMenu->addStretch(1);
    QWidget *wxMenu = new QWidget();
    wxMenu->setMinimumWidth(200);
    wxMenu->setMaximumWidth(200);
    wxMenu->setAutoFillBackground(true);
    wxMenu->setStyleSheet("background: " + this->palette().light().color().name() + ";");
    wxMenu->setLayout(layoutMenu);

    /**
     * -------------------------------------------
     *  create button controls
     */

    QPushButton *pbClose = new QPushButton();
    pbClose->setText(tr("pbClose"));
    pbClose->setIcon(QIcon::fromTheme("button_cancel"));
    pbClose->setMinimumWidth(140);
    QObject::connect(pbClose, &QPushButton::clicked, [this]()
    {
        this->close();
    });

    QGridLayout *layoutDialogControls = new QGridLayout();
    layoutDialogControls->setContentsMargins(20, 0, 20, 15);
    layoutDialogControls->addWidget(pbClose, 0, 1);
    layoutDialogControls->setColumnStretch(0, 1);

    /**
     * -------------------------------------------
     *  create dialog layout and add all components
     */

    QGridLayout *layoutMain = new QGridLayout();
    layoutMain->setContentsMargins(0, 0, 0, 0);
    layoutMain->addWidget(wxMenu, 0, 0, 3, 1);
    layoutMain->addWidget(stackSettings, 0, 1, 1, 1);
    layoutMain->addItem(layoutDialogControls, 2, 1, 1, 1);
    layoutMain->rowStretch(2);
    layoutMain->columnStretch(1);

    this->setLayout(layoutMain);
}

/**
 * Load and parse data into controls
 * @brief Settings::setupData
 */
void Settings::setupData()
{
    /**
     * -------------------------------------------
     *  available protocols
     */

    QProcess *commandTechnologies = new QProcess();
    commandTechnologies->start("nordvpn", QStringList() << "set" << "technology" << "--help");
    commandTechnologies->waitForFinished();
    QString outputTechnologies = commandTechnologies->readAllStandardOutput();

    // parse output
    QStringList technologies = outputTechnologies.trimmed().split(QString("\n"));
    for (QString line : technologies)
    {
        if (line.toLower().contains("supported values"))
        {
            line = line.mid(line.indexOf(":") + 1);
            QStringList technologyList = QStringList();
            if (line.contains(","))
            {
                technologyList.append(line.split(","));
            }
            else if (line.contains(" or "))
            {
                technologyList.append(line.split(" or "));
            }

            cmbTechnology->blockSignals(true);
            cmbTechnology->clear();
            for (QString technologyElement : technologyList)
            {
                technologyElement = technologyElement.replace(QRegularExpression("\\.+$"), "");
                cmbTechnology->addItem(technologyElement.trimmed());
            }
            cmbTechnology->blockSignals(false);
        }
    }

    /**
     * -------------------------------------------
     *  available protocols
     */

    QProcess *commandProtocols = new QProcess();
    commandProtocols->start("nordvpn", QStringList() << "set" << "protocol" << "--help");
    commandProtocols->waitForFinished();
    QString outputProtocols = commandProtocols->readAllStandardOutput();

    // parse output
    QStringList protocols = outputProtocols.trimmed().split(QString("\n"));
    for (QString line : protocols)
    {
        if (line.toLower().contains("supported values"))
        {
            line = line.mid(line.indexOf(":") + 1);
            QStringList protocolList = QStringList();
            if (line.contains(","))
            {
                protocolList.append(line.split(","));
            }
            else if (line.contains(" or "))
            {
                protocolList.append(line.split("or"));
            }

            cmbProtocol->blockSignals(true);
            cmbProtocol->clear();
            for (QString protocolElement : protocolList)
            {
                protocolElement = protocolElement.replace(QRegularExpression("\\.+$"), "");
                cmbProtocol->addItem(protocolElement.trimmed());
            }
            cmbProtocol->blockSignals(false);
        }
    }

    /**
     * -------------------------------------------
     *  load current settings
     */

    QProcess *command = new QProcess();
    command->start("nordvpn", QStringList() << "settings");
    command->waitForFinished();
    QString output = command->readAllStandardOutput();

    // parse output
    QStringList lines = output.trimmed().split(QString("\n"));
    for (QString line : lines)
    {
        if (line.trimmed().toLower().contains("technology"))
        {
            technology = line.mid(line.indexOf(":") + 1).trimmed();
            int index = cmbTechnology->findText(technology, Qt::MatchFlag::MatchFixedString);
            if (index != -1 )
            {
                cmbTechnology->blockSignals(true);
                cmbTechnology->setCurrentIndex(index);
                cmbTechnology->blockSignals(false);
            }
        }
        else if (line.trimmed().toLower().contains("protocol"))
        {
            protocol = line.mid(line.indexOf(":") + 1).trimmed();
            int index = cmbProtocol->findText(protocol, Qt::MatchFlag::MatchFixedString);
            if (index != -1 )
            {
                cmbProtocol->blockSignals(true);
                cmbProtocol->setCurrentIndex(index);
                cmbProtocol->blockSignals(false);
            }
        }
        else if (line.trimmed().toLower().contains("firewall"))
        {
            bool firewall = line.mid(line.indexOf(":") + 1).trimmed() == "enabled";
            cbFirewall->setChecked(firewall);
        }
        else if (line.trimmed().toLower().contains("kill"))
        {
            bool killswitch = line.mid(line.indexOf(":") + 1).trimmed() == "enabled";
            cbKillswitch->setChecked(killswitch);
        }
        else if (line.trimmed().toLower().contains("cybersec"))
        {
            bool cybersec = line.mid(line.indexOf(":") + 1).trimmed() == "enabled";
            cbCybersec->setChecked(cybersec);
        }
        else if (line.trimmed().toLower().contains("obfuscate"))
        {
            bool obfuscate = line.mid(line.indexOf(":") + 1).trimmed() == "enabled";
            cbObfuscate->setChecked(obfuscate);
        }
        else if (line.trimmed().toLower().contains("auto"))
        {
            bool autoconnect = line.mid(line.indexOf(":") + 1).trimmed() == "enabled";
            cbAutoconnect->setChecked(autoconnect);
        }
        else if (line.trimmed().toLower().contains("ipv6"))
        {
            bool ipv6 = line.mid(line.indexOf(":") + 1).trimmed() == "enabled";
            cbIPv6->setChecked(ipv6);
        }
    }
}

void Settings::displaySuccess()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle(tr("dlgSuccessTitle"));
    msg->setWindowModality(Qt::WindowModality::ApplicationModal);
    msg->setWindowIcon(QIcon::fromTheme("compass"));
    msg->setIcon(QMessageBox::Information);
    msg->setDefaultButton(QMessageBox::Ok);
    msg->setStandardButtons(QMessageBox::Ok);
    msg->setText(tr("dlgSuccessMessage"));
    msg->setMinimumSize(600, 357);
    msg->setMaximumSize(600, 357);
    msg->show();
    msg->activateWindow();
    msg->raise();
}

void Settings::displayError(QString message)
{
    QString html = message.trimmed().replace("\n", "<br>");
    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle(tr("dlgErrorTitle"));
    msg->setWindowModality(Qt::WindowModality::ApplicationModal);
    msg->setWindowIcon(QIcon::fromTheme("compass"));
    msg->setIcon(QMessageBox::Warning);
    msg->setDefaultButton(QMessageBox::Ok);
    msg->setStandardButtons(QMessageBox::Ok);
    msg->setText(tr("dlgErrorMessage"));
    msg->setInformativeText(html);
    msg->setMinimumSize(600, 357);
    msg->setMaximumSize(600, 357);
    msg->show();
    msg->activateWindow();
    msg->raise();
}

QString Settings::saveSettings(QStringList commands)
{
    QProcess *command = new QProcess();
    command->start("nordvpn", commands);
    command->waitForFinished();

    QString output = command->readAllStandardOutput();
    QString error = command->readAllStandardError();
    if ((error == nullptr || error.trimmed().length() == 0) &&
        !output.contains("successfully", Qt::CaseSensitivity::CaseInsensitive) &&
        !output.contains("already set", Qt::CaseSensitivity::CaseInsensitive)
        )
    {
        error = output.trimmed();
    }

    return (error == nullptr) ? "" : error.trimmed();
}
