#include "settings.hpp"

Settings::Settings() : KCoreConfigSkeleton()
{
    this->setCurrentGroup("General");
    this->addItemInt("Interval", this->interval, 1000);

    this->setCurrentGroup("Inhibition");
    this->addItemStringList("Processes", this->processes);
}


/*!
 * \var int Settings::interval
 * Interval (in milliseconds) at which the service should actualize inhibition list.
 */
