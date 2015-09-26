#pragma once
#include <QtCore/QStringList>

#include <KF5/KConfigCore/KCoreConfigSkeleton>

class Settings: public KCoreConfigSkeleton
{
public:
    static Settings& instance() {
        static Settings instance;
        return instance;
    }

    int interval;
    QStringList processes;

private:
    Settings();
    Settings(const Settings&) = delete;
    Settings(const Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(const Settings&&) = delete;
};
