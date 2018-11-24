#include "callbackincubator.h"

CallBackIncubator::CallBackIncubator(QObject *parent) : QObject (parent)
{
    menuParent = nullptr;
    model = nullptr;
}

void CallBackIncubator::statusChanged(QQmlIncubator::Status status)
{
    emit statusAsChanged(status);
}

