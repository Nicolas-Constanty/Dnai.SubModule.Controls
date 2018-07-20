#include "callbackincubator.h"

CallBackIncubator::CallBackIncubator(QObject *parent) : QObject (parent)
{

}

void CallBackIncubator::statusChanged(QQmlIncubator::Status status)
{
    emit statusAsChanged(status);
}

