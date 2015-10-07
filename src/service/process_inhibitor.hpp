#pragma once
#include <QtCore/QSet>
#include <QtCore/QString>

#include "base_inhibitor.hpp"


class ProcessInhibitor : public BaseInhibitor
{
    Q_OBJECT

public:
    ProcessInhibitor(const QStringList& processes, int interval);

private slots:
    void inspectRunningProcesses();

private:
    QSet<QString> wantedProcesses;
    QSet<QString> foundProcesses;
};
