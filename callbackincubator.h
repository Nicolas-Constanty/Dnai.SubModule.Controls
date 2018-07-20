#ifndef CALLBACKINCUBATOR_H
#define CALLBACKINCUBATOR_H

#include <QQmlIncubator>
#include <QObject>
#include <QAbstractItemModel>
#include <QQuickItem>

class CallBackIncubator : public QObject, public QQmlIncubator
{
    Q_OBJECT
public:
    explicit CallBackIncubator(QObject *parent = nullptr);

signals:
    void statusAsChanged(QQmlIncubator::Status);
    // QQmlIncubator interface
protected:
    void statusChanged(QQmlIncubator::Status);

public:
    QVariant name;
    QVariant item;
    QQuickItem *menuParent;
    QString fullpath;
    QAbstractItemModel* model;
    QModelIndex index;
};

#endif // CALLBACKINCUBATOR_H
