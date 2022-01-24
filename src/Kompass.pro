QT += core gui svg svgwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    data/connectionresult.cpp \
    data/vpnserver.cpp \
    data/vpnservermodel.cpp \
    gui/clickablewidget.cpp \
    gui/installer.cpp \
    gui/kompass.cpp \
    gui/kompassmenu.cpp \
    gui/kompasswindow.cpp \
    gui/sectiontitle.cpp \
    gui/settings.cpp \
    gui/togglebutton.cpp \
    gui/trayicon.cpp \
    launchverification.cpp \
    main.cpp \
    releasemonitor.cpp \
    singleinstance.cpp

HEADERS += \
    data/connectionresult.h \
    data/vpnserver.h \
    data/vpnservermodel.h \
    gui/clickablewidget.h \
    gui/installer.h \
    gui/kompass.h \
    gui/kompassmenu.h \
    gui/kompasswindow.h \
    gui/sectiontitle.h \
    gui/settings.h \
    gui/togglebutton.h \
    gui/trayicon.h \
    launchverification.h \
    releasemonitor.h \
    singleinstance.h

FORMS +=

TRANSLATIONS += \
    Kompass_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Kompass.qrc
