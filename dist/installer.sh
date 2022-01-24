#!/bin/bash

# Colors
CLR_GREEN="\033[1;32m"
CLR_RED="\033[1;31m"
CLR_NONE="\033[0m"

# Kompass settings
KOMPASS_DIR="/opt/Kompass"
KOMPASS_LINK="/usr/share/applications"
KOMPASS_INSTALLED="N"

# Start with a clean screen
clear

# Welcome user
echo -e "Hey there, thanks for waking me up. I'll be the one guiding ";
echo -e "you through the installation of Kompass.";
echo -e "";
echo -e "Before I start, some quick checks...";
echo -e "";

# Check if directory exists
if [ -d "${KOMPASS_DIR}" ]
then
    echo -e "${CLR_GREEN}[x]${CLR_NONE} Detected previous Kompass installation."
    KOMPASS_INSTALLED="Y"
else
    echo -e "${CLR_GREEN}[x]${CLR_NONE} No previous Kompass installation present."
fi

# Check if directory writable
if [ -w "/opt" ]
then
    echo -e "${CLR_GREEN}[x]${CLR_NONE} Installation directory is writable."
else
    echo -e "${CLR_RED}[-]${CLR_NONE} Installation directory is not writable.";
    echo -e "";
    echo -e "${CLR_RED}Canceled installation.${CLR_NONE}";
    echo -e "Sorry, it looks like the installation directory /opt is not writable.";
    echo -e "Please make sure to use sudo to run this installer.";
    echo -e "";
    exit;
fi

# Check if NordVPN installed
if command -v nordvpn &> /dev/null
then
    echo -e "${CLR_GREEN}[x]${CLR_NONE} NordVPN binary is available."
else
    echo -e "${CLR_RED}[-]${CLR_NONE} NordVPN binary is not available.";
    echo -e "";
    echo -e "${CLR_RED}Canceled installation.${CLR_NONE}";
    echo -e "Kompass depends on the NordVPN binary. Please make sure to install it first.";
    echo -e "You can download the NordVPN installer here:.https://nordvpn.com/download/linux/";
    echo -e "";
    exit;
fi

# Entertain user
echo -e "";
echo -e "Checks are done. Now here some questions...";
echo -e "";

# Ask user for permission to overwrite existing
if [ ${KOMPASS_INSTALLED} = "Y" ]
then
    echo -e "A previous version of Kompass has been found.";
    echo -e "This installer will replace the previous version.";
    while true
        do
            read -p "Is this OK? [Y/n] " KOMPASS_REPLACE
            case $KOMPASS_REPLACE in
                [yY])
                    # Just Continue
                    break;;
                [nN])
                    echo -e "";
                    echo -e "${CLR_RED}Canceled installation.${CLR_NONE}";
                    echo -e "No files have been overwritten. The installer completed without updating Kompass.${CLR_NONE}";
                    echo -e ""
                    exit;;
                *)
                    echo -e "";
                    echo -e "${CLR_RED}Please answer with Y or N.${CLR_NONE}";
                    echo -e "";
                    ;;
            esac
        done
fi

# Ask for desktop file location
echo -e ""
echo -e "A link to Kompass will be created for your launcher.";
echo -e "The link will be installed at this location: ${KOMPASS_LINK}";
while true
    do
        read -p "To accept the default path hit return. Otherwise specify a new path here: " KOMPASS_LINK_USER
        case $KOMPASS_LINK_USER in
            "")
                # Just Continue
                break;;
            *)
                if [ -d "${KOMPASS_LINK_USER}" ] || [ -w "${KOMPASS_LINK_USER}" ]
                then
                    KOMPASS_LINK = "${KOMPASS_LINK_USER}"
                    break;
                else
                    echo -e "";
                    echo -e "${CLR_RED}Invalid directory ${KOMPASS_LINK_USER}.${CLR_NONE}";
                    echo -e "The given directory does either not exist or is not writable.";
                    echo -e "";
                fi
                ;;
        esac
    done

# Remove previos Kompass version
rm -rf "${KOMPASS_DIR}"

# Place new files and make executable
cp -f "./kompass.desktop" "${KOMPASS_LINK}"
mkdir "${KOMPASS_DIR}"
cp "./Kompass" "${KOMPASS_DIR}"
chmod +x "${KOMPASS_DIR}/Kompass"
cp "./kompass.png" "${KOMPASS_DIR}"
chmod 644 "${KOMPASS_LINK}/kompass.desktop"

# Inform user
echo -e "";
echo -e "${CLR_GREEN}Kompass was installed successfully.${CLR_NONE}";
echo -e "The installer folder can now be deleted.";
echo -e "";
echo -e "Launch Kompass in a terminal using command: Kompass";
echo -e "Or browse through your launcher menu to launch Kompass from there.";
echo -e "";
exit;
