#include "base_inhibitor.hpp"

/*!
 * \class BaseInhibitor
 * The BaseInhibitor is base calss of all inhibitors. An inhibitor is an object that continually
 * inspects the OS and suppresses KDE power management while certain conditions are hold.
 */

/*!
 * \fn void BaseInhibitor::inhibitionStarted(const QString &app, const QString &reason)
 * This signal is emitted when
 *
 * \param[in] app name of application that cause inhibition
 * \param[in] reason reason of inhibition
 */

/*!
 * \fn void BaseInhibitor::inhibitionFinished(const QString &app)
 * This signal is emitted when
 *
 * \param[in] app name of application that isn't cause of inhibition anymore
 */
