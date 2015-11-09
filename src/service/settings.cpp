#include "settings.hpp"

/*!
 * \class Settings
 * A singletone object to provide access to service configuration.
 */

Settings::Settings() : KCoreConfigSkeleton("energetikrc")
{
    this->setCurrentGroup("General");
    this->addItemBool("InspectProcesses", this->inspectProcesses, true);
    this->addItemBool("InspectFullscreen", this->inspectFullscreen, true);

    this->setCurrentGroup("ProcessInhibition");
    this->addItemInt("Interval", this->inspecProcessesInterval, 1000);
    this->addItemStringList("Processes", this->wantedProcesses);
}

/*!
 * \var QStringList Settings::inspectProcesses
 * Show if the process inhibitor should be activated.
 */

/*!
 * \var QStringList Settings::inspectFullscreen
 * Show if the fullscreen inhibitor should be activated.
 */

/*!
 * \var QStringList Settings::wantedProcesses
 * List of processes that should cause inhibition of power management.
 */

/*!
 * \var int Settings::inspecProcessesInterval
 * Interval (in milliseconds) at which the process inhibitor should actualize inhibition list.
 */
