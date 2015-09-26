#include <algorithm>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#include <proc/readproc.h>

#include "settings.hpp"

#include "service.hpp"


/*!
 * \class Service
 * A service that suppresses KDE power management if certain conditions are hold. The service
 * inspects running processes at regular intervals and inhibit screensaver, screen turning off,
 * etc. if certain processes are invoked.
 */


Service::Service(QObject *parent) : QObject(parent)
{
    Q_FOREACH(QString process, Settings::instance().processes) {
        this->rules.insert(process);
    }

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

/*!
 * \fn void Service::run()
 * Launch the service. Having been launched the service automatically actualizes inhibition
 * list at regular intervals.
 */
void Service::run()
{
    this->startTimer(Settings::instance().interval);
}

/*!
 * \fn void Service::timerEvent(QTimerEvent *event)
 * Actualize inhibition list. The service applies different rules such as
 * - whether specified process is running
 * - whether a window in fullscreen mode presents
 * - ...
 *
 * and decides if power management should be suppressed or not.
 */
void Service::timerEvent(QTimerEvent *event)
{
    this->inspectRunningProcesses();
}

/*!
 * \fn void Service::inspectRunningProcesses()
 * Inspect if specified processes are running. The service walks through \c \\proc filesystem
 * and check if processes specified in configuration are run. For each specified process an
 * inhibition is created if not exists. All previously created inhibitions are canceled if
 * corresponding processes have finished.
 */
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

    Q_FOREACH(QString process, foundProcesses) {
        this->startSuppressPowerManagement(process);
    }

    Q_FOREACH(QString process, this->cookies.keys()) {
        if (!foundProcesses.contains(process))
            this->stopSuppressPowerManagement(process);
    }
}

/*!
 * \fn void Service::startSuppressPowerManagement(const QString &process)
 * Send DBus message to KDE power management system to inhibit.
 *
 * \param[in] process name of process that cause inhibition
 */
void Service::startSuppressPowerManagement(const QString &process)
{
    if (this->cookies.contains(process))
        return;
    QDBusReply<uint> cookie = this->busInterface->call("Inhibit", process, "energetiK");
    this->cookies[process] = cookie;
}

/*!
 * \fn void Service::stopSuppressPowerManagement(const QString &process)
 * Send DBus message to KDE power management system to uninhibit.
 *
 * \param[in] process name of process that isn't cause of inhibition anymore
 */
void Service::stopSuppressPowerManagement(const QString &process)
{
    this->busInterface->call("UnInhibit", this->cookies[process]);
    this->cookies.remove(process);
}

/*!
 * \var QScopedPointer<QDBusInterface> Service::busInterface
 * DBus interface of KDE power management system.
 */

/*!
 * \var QHash<QString, uint> Service::cookies
 * Storage of inhibition cookies. It keeps pairs of process name and corresponding inhibition
 * cookie. Each inhibition is identified by unique positive integer number (cookie in KDE
 * terminlogy). The cookie is assigned to an inhibition after its creation and is necessary
 * to cancel the inhibition.
 */

/*!
 * \var QSet<QString> Service::rules
 * List of processes that should cause inhibition of power management.
 */
