#ifndef CONNECTIONRESULT_H
#define CONNECTIONRESULT_H

#include <QObject>

QT_BEGIN_NAMESPACE
    namespace Ui { class ConnectionResult;}
QT_END_NAMESPACE

class ConnectionResult : public QObject
{
    Q_OBJECT

    public:
        ConnectionResult();
        bool isSuccessful();
        QString getResult();
        void setSuccessful(bool success);
        void setResult(QString result);

    private:
        bool success;
        QString result;
};
#endif // CONNECTIONRESULT_H
