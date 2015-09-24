#pragma once
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtDBus/QDBusInterface>


class Service : public QObject
{
    Q_OBJECT

public:
    Service(QObject *parent=nullptr);
    void run();

protected:
    void timerEvent(QTimerEvent *event);

private:
    void inspectRunningProcesses();
    void startSuppressPowerManagement(const QString &process);
    void stopSuppressPowerManagement(const QString &process);

    QScopedPointer<QDBusInterface> busInterface;
    QHash<QString, uint> cookies;
    QSet<QString> rules;
    int interval = 1000;
};
