#include "gui/kompass.h"

#include <QApplication>
#include <QFontDatabase>
#include <QLocale>
#include <QTranslator>
#include <launchverification.h>

/**
 * THe main method to launch Kompass
 * @brief main
 * @param argc
 * @param argv
 * @return the exit code
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // add translation support
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Kompass_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // add application font
    QFontDatabase::addApplicationFont(":/font/FontAwesome.otf");

    // start launch verification sequence
    auto *launcher = new LaunchVerification();
\
    // verify NordVPN binary
    launcher->retry = true;
    while (launcher->retry && !launcher->validBinary)
    {
        launcher->verifyBinary();
    }

    // vrify NordVPN version
    if (launcher->validBinary)
    {
        launcher->retry = true;
        while (launcher->retry && !launcher->validVersion)
        {
            launcher->verifyVersion();
        }
    }

    // verify NordVPN account
    if (launcher->validBinary && launcher->validVersion)
    {
        launcher->retry = true;
        while (launcher->retry && !launcher->validAccount)
        {
            launcher->verifyAccount();
        }
    }

    // launch the UI
    if (launcher->validBinary && launcher->validVersion && launcher->validAccount)
    {
        new Kompass();
        return a.exec();
    }
    return -1;
}
