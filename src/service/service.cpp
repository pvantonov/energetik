#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#include "settings.hpp"
#include "process_inhibitor.hpp"
#include "fullscreen_inhibitor.hpp"

#include "service.hpp"


/*!
 * \class Service
 * A service that suppresses KDE power management if certain conditions are hold. The service
 * activates a number of inhibitors and controls KDE power management according to requests of
 * these inhibitors.
 */

Service::Service()
{
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
 * Launch the service. The service activates inhibitors according to configuration.
 */
void Service::run()
{
    if (Settings::instance().inspectProcesses) {
        QSharedPointer<BaseInhibitor> processInhibitor(
            new ProcessInhibitor(
                Settings::instance().wantedProcesses,
                Settings::instance().inspecProcessesInterval
            )
        );
        this->inhibitors.append(processInhibitor);
    }

    if (Settings::instance().inspectFullscreen) {
        QSharedPointer<BaseInhibitor> fullscreenInhibitor(
            new FullscreenInhibitor()
        );
        this->inhibitors.append(fullscreenInhibitor);
    }

    foreach(QSharedPointer<BaseInhibitor> inhibitor, this->inhibitors) {
        this->connect(
            inhibitor.data(), SIGNAL(inhibitionStarted(QString,QString)),
            this, SLOT(startSuppressPowerManagement(QString,QString))
        );
        this->connect(
            inhibitor.data(), SIGNAL(inhibitionFinished(QString)),
            this, SLOT(stopSuppressPowerManagement(QString))
        );
    }
}

/*!
 * \fn void Service::startSuppressPowerManagement(const QString &app, const QString &reason)
 * Send DBus message to KDE power management system to inhibit.
 *
 * \param[in] app name of application that cause inhibition
 * \param[in] reason reason of inhibition
 */
void Service::startSuppressPowerManagement(const QString &app, const QString &reason)
{
    if (this->cookies.contains(app))
        return;
    QDBusReply<uint> cookie = this->busInterface->call("Inhibit", app, reason);
    this->cookies[app] = cookie;
}

/*!
 * \fn void Service::stopSuppressPowerManagement(const QString &app)
 * Send DBus message to KDE power management system to uninhibit.
 *
 * \param[in] app name of application that isn't cause of inhibition anymore
 */
void Service::stopSuppressPowerManagement(const QString &app)
{
    this->busInterface->call("UnInhibit", this->cookies[app]);
    this->cookies.remove(app);
}

/*!
 * \var QList<QScopedPointer<BaseInhibitor>> Service::inhibitors
 * List of activated inhibitors.
 */

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
