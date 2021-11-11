#ifndef LAUNCHVERIFICATION_H
#define LAUNCHVERIFICATION_H

#include <QIcon>
#include <QMessageBox>
#include <QProcess>

QT_BEGIN_NAMESPACE
    namespace Ui { class LaunchVerification;}
QT_END_NAMESPACE

class LaunchVerification : public QObject
{
    Q_OBJECT

    public:
        bool validBinary = false;
        bool validVersion = false;
        bool validAccount = false;
        bool retry = true;

        LaunchVerification();
        void verifyBinary();
        void verifyVersion();
        void verifyAccount();
};

#endif // LAUNCHVERIFICATION_H
