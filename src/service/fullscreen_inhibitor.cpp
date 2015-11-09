#include <KF5/KWindowSystem/KWindowInfo>

#include "fullscreen_inhibitor.hpp"


/*!
 * \class FullscreenInhibitor
 * This inhibitor listens to windows manager events and inhibit KDE power management while any
 * window is in fulscreen mode.
 */

FullscreenInhibitor::FullscreenInhibitor()
{
    this->connect(
        KWindowSystem::self(), SIGNAL(windowChanged(WId, NET::Properties, NET::Properties2)),
        this, SLOT(onWindowChanged(WId, NET::Properties)));
}

/*!
 * \fn void FullscreenInhibitor::onWindowChanged(WId winId, NET::Properties properties)
 * Check if a window have change its state to fullscreen state or from fullscreen state to any
 * other state and change inhibition status according to it.
 * \param[in] winId window identifier
 * \param[in] properties changed properties of the window
 */
void FullscreenInhibitor::onWindowChanged(WId winId, NET::Properties properties)
{
    if (properties & NET::WMState) {
        KWindowInfo info(winId, NET::WMState);
        if (info.valid()) {
            if(info.hasState(NET::FullScreen) && !this->fullscreenWindows.contains(winId)) {
                emit this->inhibitionStarted("energetik", "fullscreen window");
                this->fullscreenWindows.insert(winId);
            }
            if(!info.hasState(NET::FullScreen) && this->fullscreenWindows.contains(winId)) {
                emit this->inhibitionFinished("energetik");
                this->fullscreenWindows.remove(winId);
            }
        }
    }
}


/*!
 * \var QSet<QString> FullscreenInhibitor::fullscreenWindows
 * List of fullscreen windows.
 */
