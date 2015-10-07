#pragma once
#include <QtCore/QObject>
#include <QtCore/QString>

class BaseInhibitor : public QObject
{
    Q_OBJECT

signals:
    void inhibitionStarted(const QString &app, const QString &reason);
    void inhibitionFinished(const QString &app);
};
