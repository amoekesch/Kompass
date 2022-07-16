#include "launchverification.h"

LaunchVerification::LaunchVerification() {}

void LaunchVerification::verifyBinary()
{
    QProcess *command = new QProcess();
    command->start("nordvpn", QStringList() << "-v");
    command->waitForFinished();

    QString output = command->readAllStandardOutput();
    if (output != nullptr && output.length() > 0 && output.toLower().contains("nordvpn version"))
    {
        this->validBinary = true;
        this->retry = false;
        return;
    }

    QMessageBox msg = QMessageBox();
    msg.setWindowIcon(QIcon(":/img/kompass.png"));
    msg.setWindowTitle(tr("dlgErrorTitle"));
    msg.setText(tr("msgErrorNoBinary"));
    msg.setIcon(QMessageBox::Information);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setWindowModality(Qt::WindowModality::ApplicationModal);
    msg.activateWindow();
    msg.show();
    this->validBinary = false;
    this->retry = msg.exec() == QMessageBox::Ok;
}

void LaunchVerification::verifyVersion()
{
    QProcess *command = new QProcess();
    command->start("nordvpn", QStringList() << "-v");
    command->waitForFinished();

    QString output = command->readAllStandardOutput();
    if (output != nullptr && output.length() > 0 && output.toLower().contains("nordvpn version"))
    {
        QString version = output.mid(output.lastIndexOf(" ") + 1);
        version = version.left(version.lastIndexOf(".")).trimmed();
        if (version.toDouble() >= 3.14)
        {
            this->validVersion = true;
            this->retry = false;
            return;
        }
    }

    QMessageBox msg = QMessageBox();
    msg.setWindowIcon(QIcon(":/img/kompass.png"));
    msg.setWindowTitle(tr("dlgErrorTitle"));
    msg.setText(tr("msgErrorWrongBinaryVersion"));
    msg.setIcon(QMessageBox::Information);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setWindowModality(Qt::WindowModality::ApplicationModal);
    msg.activateWindow();
    msg.show();
    this->validVersion = false;
    this->retry = msg.exec() == QMessageBox::Ok;
}

void LaunchVerification::verifyAccount()
{
    QProcess *command = new QProcess();
    command->start("nordvpn", QStringList() << "account");
    command->waitForFinished();

    QString output = command->readAllStandardOutput();
    if (output != nullptr && output.length() > 0 && output.toLower().contains("service") && output.contains("Active"))
    {
        this->validAccount = true;
        this->retry = false;
        return;
    }
    else if (output != nullptr && output.length() > 0 && output.toLower().contains("service") && !output.contains("Active"))
    {
        QMessageBox msg = QMessageBox();
        msg.setWindowIcon(QIcon(":/img/kompass.png"));
        msg.setWindowTitle(tr("dlgErrorTitle"));
        msg.setText(tr("msgErrorExpiredAccount"));
        msg.setIcon(QMessageBox::Information);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setWindowModality(Qt::WindowModality::ApplicationModal);
        msg.activateWindow();
        msg.show();
        this->validAccount = false;
        this->retry = msg.exec() == QMessageBox::Ok;
        return;
    }
    else if (output != nullptr && output.length() > 0 && output.toLower().contains("not logged in"))
    {
        QProcess *commandLogin = new QProcess();
        commandLogin->start("nordvpn", QStringList() << "login" << "--nordaccount");
        commandLogin->waitForFinished();

        QString outputLogin = commandLogin->readAllStandardOutput();
        if (outputLogin != nullptr && outputLogin.length() > 0 && outputLogin.toLower().contains("https"))
        {
            QString url = outputLogin.mid(outputLogin.toLower().indexOf("https")).trimmed();

            QMessageBox msg = QMessageBox();
            msg.setWindowIcon(QIcon(":/img/kompass.png"));
            msg.setWindowTitle(tr("dlgErrorTitle"));
            msg.setText(tr("msgErrorNotLoggedIn").arg(url));
            msg.setIcon(QMessageBox::Information);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msg.setWindowModality(Qt::WindowModality::ApplicationModal);
            msg.activateWindow();
            msg.show();
            this->validAccount = false;
            this->retry = msg.exec() == QMessageBox::Ok;
            return;
        }
    }

    // should never happen
    QMessageBox msg = QMessageBox();
    msg.setWindowIcon(QIcon(":/img/kompass.png"));
    msg.setWindowTitle(tr("dlgErrorTitle"));
    msg.setText(tr("msgErrorInvalidAccount"));
    msg.setIcon(QMessageBox::Critical);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setWindowModality(Qt::WindowModality::ApplicationModal);
    msg.activateWindow();
    msg.show();
    this->validVersion = false;
    this->retry = false;
}
