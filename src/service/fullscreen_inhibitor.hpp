#pragma once
#include <QtCore/QSet>

#include <KF5/KWindowSystem/KWindowSystem>

#include "base_inhibitor.hpp"

class FullscreenInhibitor : public BaseInhibitor
{
    Q_OBJECT

public:
    FullscreenInhibitor();

private slots:
    void onWindowChanged(WId winId, NET::Properties properties);

private:
    QSet<WId> fullscreenWindows;
};
