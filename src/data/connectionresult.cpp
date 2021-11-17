#include "connectionresult.h"

ConnectionResult::ConnectionResult()
{
}

bool ConnectionResult::isSuccessful()
{
    return this->success;
}

void ConnectionResult::setSuccessful(bool success)
{
    this->success = success;
}

QString ConnectionResult::getResult()
{
    return this->result;
}

void ConnectionResult::setResult(QString result)
{
    this->result = result;
}

