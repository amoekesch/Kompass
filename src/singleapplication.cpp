#include "singleapplication.h"

SingleApplication::SingleApplication(const QString& key)
    : key(key)
    , memoryLockKey(hash(key, "_memoryLockKey"))
    , sharedMemoryKey(hash(key, "_sharedMemoryKey"))
    , sharedMemory(sharedMemoryKey)
    , memoryLock(memoryLockKey, 1)
{
    memoryLock.acquire();
    {
        QSharedMemory fix(sharedMemoryKey);
        fix.attach();
    }
    memoryLock.release();
}

SingleApplication::~SingleApplication()
{
    release();
}

bool SingleApplication::isRunning()
{
    if (sharedMemory.isAttached())
    {
        return false;
    }

    memoryLock.acquire();
    const bool running = sharedMemory.attach();
    if (running)
    {
        sharedMemory.detach();
    }
    memoryLock.release();

    return running;
}

bool SingleApplication::test()
{
    if (isRunning())
    {
        return false;
    }

    memoryLock.acquire();
    const bool result = sharedMemory.create(sizeof(quint64));
    memoryLock.release();

    if (!result)
    {
        release();
        return false;
    }

    return true;
}

void SingleApplication::release()
{
    memoryLock.acquire();
    if (sharedMemory.isAttached())
    {
        sharedMemory.detach();
    }
    memoryLock.release();
}

QString SingleApplication::hash(const QString& key, const QString& salt)
{
    QByteArray data;

    data.append(key.toUtf8());
    data.append(salt.toUtf8());
    data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

    return data;
}
