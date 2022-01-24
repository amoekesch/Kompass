#include "trayicon.h"

TrayIcon::TrayIcon(QObject *parent) : QObject{parent}
{
    this->initialize();
}

void TrayIcon::initialize()
{
    readConfiguration();

    icons = new QMap<int, QIcon>();
    icons->insert(indexBlue, iconBlue);
    icons->insert(indexGreen, iconGreen);
    icons->insert(indexRed, iconRed);
    icons->insert(indexWhite, iconWhite);
    icons->insert(indexBlack, iconBlack);
    icons->insert(indexGrey25, iconGrey25);
    icons->insert(indexGrey50, iconGrey50);
    icons->insert(indexGrey75, iconGrey75);
}

QIcon TrayIcon::getIcon(bool connected)
{
    if (connected)
    {
        return icons->value(currentIndexConnected, iconBlue);
    }
    else
    {
        return icons->value(currentIndexDisconnected, iconGrey50);
    }
}

QMap<int, QIcon> TrayIcon::getIcons()
{
    return *icons;
}


int TrayIcon::getCurrentIndexConnected()
{
    return this->currentIndexConnected;
}

void TrayIcon::setCurrentIndexConnected(int index)
{
    this->currentIndexConnected = index;
    storeConfiguration();
}

int TrayIcon::getCurrentIndexDisconnected()
{
    return this->currentIndexDisconnected;
}

void TrayIcon::setCurrentIndexDisconnected(int index)
{
    this->currentIndexDisconnected = index;
    storeConfiguration();
}

void TrayIcon::readConfiguration()
{
    QSettings *cfg = new QSettings(CFG_FOLDER, CFG_FILE);
    currentIndexConnected = cfg->value(KEY_CONNECTED, defaultIndexConnected).toInt();
    currentIndexDisconnected = cfg->value(KEY_DISCONNECTED, defaultIndexDisconnected).toInt();
}

void TrayIcon::storeConfiguration()
{
    QSettings *cfg = new QSettings(CFG_FOLDER, CFG_FILE);
    cfg->setValue(KEY_CONNECTED, currentIndexConnected);
    cfg->setValue(KEY_DISCONNECTED, currentIndexDisconnected);
}
