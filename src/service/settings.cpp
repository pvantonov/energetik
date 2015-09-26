#include "settings.hpp"

/*!
 * \class Settings
 * A singletone object to provide access to service configuration.
 */

Settings::Settings() : KCoreConfigSkeleton("energetikrc")
{
    this->setCurrentGroup("General");
    this->addItemInt("Interval", this->interval, 1000);

    this->setCurrentGroup("Inhibition");
    this->addItemStringList("Processes", this->processes);
}

/*!
 * \var QStringList Settings::processes
 * List of processes that should cause inhibition of power management.
 */

/*!
 * \var int Settings::interval
 * Interval (in milliseconds) at which the service should actualize inhibition list.
 */
