#pragma once
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtDBus/QDBusInterface>

#include "base_inhibitor.hpp"


class Service : public QObject
{
    Q_OBJECT

public:
    Service();
    void run();

public slots:
    void startSuppressPowerManagement(const QString &app, const QString &reason);
    void stopSuppressPowerManagement(const QString &app);
    void simulateUserActivity();

private:
    QList<QSharedPointer<BaseInhibitor>> inhibitors;
    QScopedPointer<QDBusInterface> powerManagementInterface;
    QScopedPointer<QDBusInterface> screensaverInterface;
    QHash<QString, uint> cookies;
};
