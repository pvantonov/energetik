#include <algorithm>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#include <proc/readproc.h>

#include "service.hpp"


/*!
 * \class Service
 * \brief A service that suppresses KDE power management if certain conditions are hold.
 *
 * The service inspects running processes at regular intervals and inhibit screensaver,
 * screen turning off, etc. if certain processes are invoked.
 */


Service::Service(QObject *parent) : QObject(parent)
{
    this->rules.insert("retroarch");
    QDBusConnection bus = QDBusConnection::sessionBus();
    this->busInterface.reset(
        new QDBusInterface(
            "org.freedesktop.PowerManagement.Inhibit",
            "/org/freedesktop/PowerManagement/Inhibit",
            "org.freedesktop.PowerManagement.Inhibit",
            bus,
            this
        )
    );
}

//! \brief Launch the service.
void Service::run()
{
    this->startTimer(this->interval);
}

//! \brief Inspect whether power managemet should be suppressed or not.
void Service::timerEvent(QTimerEvent *event)
{
    this->inspectRunningProcesses();
}


void Service::inspectRunningProcesses()
{
    QSet<QString> foundProcesses;

    PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLCOM);
    proc_t* proc_info = nullptr;
    while ((proc_info = readproc(proc, proc_info)) != nullptr) {
        if (this->rules.contains(proc_info->cmd))
            foundProcesses.insert(QString(proc_info->cmd));
    }
    closeproc(proc);

    Q_FOREACH(QString process, foundProcesses){
        this->startSuppressPowerManagement(process);
    }

    Q_FOREACH(QString process, this->cookies.keys())
    {
        if (!foundProcesses.contains(process))
            this->stopSuppressPowerManagement(process);
    }
}


void Service::startSuppressPowerManagement(const QString &process)
{
    if (this->cookies.contains(process))
        return;
    QDBusReply<uint> cookie = this->busInterface->call("Inhibit", process, "energetiK");
    this->cookies[process] = cookie;
}


void Service::stopSuppressPowerManagement(const QString &process)
{
    this->busInterface->call("UnInhibit", this->cookies[process]);
    this->cookies.remove(process);
}
