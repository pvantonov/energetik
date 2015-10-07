#include <QtCore/QMutableSetIterator>
#include <QtCore/QTimer>

#include <proc/readproc.h>

#include "settings.hpp"

#include "process_inhibitor.hpp"


/*!
 * \class ProcessInhibitor
 * This inhibitor inspects running processes at regular intervals and inhibit KDE power management
 * if certain processes are invoked.
 */

ProcessInhibitor::ProcessInhibitor(const QStringList& processes, int interval)
{
    foreach(QString process, processes) {
        this->wantedProcesses.insert(process);
    }

    QTimer *timer = new QTimer(this);
    this->connect(timer, SIGNAL(timeout()), this, SLOT(inspectRunningProcesses()));
    timer->start(interval);
}

/*!
 * \fn void ProcessInhibitor::inspectRunningProcesses()
 * Inspect if specified processes are running. The inhibitor walks through \c \\proc filesystem
 * and check if processes specified in configuration are run. For each specified process an
 * inhibition is created if not exists. All previously created inhibitions are canceled if
 * corresponding processes have finished.
 */
void ProcessInhibitor::inspectRunningProcesses()
{
    QSet<QString> foundProcesses;

    PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLCOM);
    proc_t* proc_info = nullptr;
    while ((proc_info = readproc(proc, proc_info)) != nullptr) {
        if (this->wantedProcesses.contains(proc_info->cmd))
            foundProcesses.insert(QString(proc_info->cmd));
    }
    closeproc(proc);

    foreach(QString process, foundProcesses) {
        if (!this->foundProcesses.contains(process)) {
            emit this->inhibitionStarted(process, "running");
            this->foundProcesses.insert(process);
        }
    }

    QMutableSetIterator<QString> i(this->foundProcesses);
    while(i.hasNext()) {
        QString process = i.next();
        if (!foundProcesses.contains(process)) {
            emit this->inhibitionFinished(process);
            i.remove();
        }
    }
}

/*!
 * \var QSet<QString> ProcessInhibitor::wantedProcesses
 * List of processes that should cause inhibition of power management.
 */

/*!
 * \var QSet<QString> ProcessInhibitor::foundProcesses
 * List of processes that have caused inhibition of power management.
 */
