#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QCryptographicHash>
#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class SingleApplication
{

public:
    SingleApplication( const QString& key );
    ~SingleApplication();

    bool isRunning();
    bool test();
    void release();

private:
    const QString key;
    const QString memoryLockKey;
    const QString sharedMemoryKey;

    QSharedMemory sharedMemory;
    QSystemSemaphore memoryLock;

    QString hash(const QString& key, const QString& salt);

    Q_DISABLE_COPY( SingleApplication )
};
#endif // SINGLEAPPLICATION_H
