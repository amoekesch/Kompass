# Kompass
Kompass - Open-source graphical NordVPN client for Linux.

## Table of Contents
* [General Info](#general-info)
* [Installation](#installation)
* [Usage](#Usage)
* [Gallery](#Gallery)
* [Dependencies](#dependencies)

## General Info
**What is Kompass?** Kompass is an easy-to-use graphical user interface wrapping the functionality provided by NordVPN Linux command-line interface (CLI).
It relies directly on the NordVPN CLI and provides the most commonly used features in a user-friendly interface.

**What Kompass is not!** Kompass was never meant to implement all available NordVPN features. It's intention is to provide a quick and easy way to connect to a VPN, disconnect a VPN and show the current VPN status. All of this in a graphical user interface for the KDE Plasma desktop environment.

## Installation
**Kompass is easy to install.** It comes with its own installation script that will guide you through the installation process. Grab the **[latest release](https://github.com/amoekesch/Kompass/releases)**, unpack it, and run the installer.

```bash
$ tar -xvf Kompass-*.tar.gz
$ cd Kompass
$ chmod +x installer.sh
$ sudo ./installer.sh
```

## Usage
**Kompass is easy to use.** Select either a specific VPN server type, or a country to connect to, or put your fate in the hands of NordVPN and let it decude what is the fastest server to use. Toggle the *Connect* switch and that's it.
![Kompass Disconnected](https://github.com/amoekesch/Kompass/raw/main/doc/kompass_disconnected.png)
As soon as Kompass could establish a connection with a VPN server, the user interface will show the connection status and additional connection details:
![Kompass Connected](https://github.com/amoekesch/Kompass/raw/main/doc/kompass_connected.png)
To *Disconnect* toggle that switch once more and Kompass will disconnect your from the VPN. The *Minimize to Tray* button in the lower left corner sends Kompass to your system tray:

![Kompass Tray](https://github.com/amoekesch/Kompass/raw/main/doc/kompass_tray.png)

A left-click on the Kompass icon will open/show the application. A right-click on the icon will display the context menu shown in the screen copy above. The context menu includes:
* The current connection status
* A menu action to disconnect a VPN
* A menu action to connect to the *Fastest* server

The *Fastest* server is the server NordVPN considers the best for your current location.

To update your NordVPN settings, use the *Settings* button in the main application window. The opening dialog will present you your current NordVPN configuration and allow you to edit these settings:
![Kompass Settings](https://github.com/amoekesch/Kompass/raw/main/doc/kompass_settings.png)

## Gallery
<a target="_blank" href="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_disconnected.png"><img src="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_disconnected.png" width="15%"></img></a> <a target="_blank" href="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_connected.png"><img src="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_connected.png" width="15%"></img></a> <a target="_blank" href="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_servers_by_group.png"><img src="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_servers_by_group.png" width="15%"></img></a> <a target="_blank" href="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_servers_by_country.png"><img src="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_servers_by_country.png" width="15%"></img></a> <a target="_blank" href="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_servers_list.png"><img src="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_servers_list.png" width="15%"></img></a> <a target="_blank" href="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_settings.png"><img src="https://github.com/amoekesch/Kompass/raw/main/doc/kompass_settings.png" width="15%"></img></a>

## Dependencies
Kompass is written in C++/QT for the KDE Plasma desktop environment and relies heavily on the NordVPN CLI. Here is its full list of dependencies:
* [KDE](https://kde.org/)
* [QT](https://www.qt.io/) (v5.0+)
* [NordVPN CLI](https://nordvpn.com/download/linux/) (v3.12.0+)
