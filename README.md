# energetiK
energetiK (Russian word for energy drink, pronounced [enɛrˈɡʲetʲɪk]) is KDE service to inhibit power management if certain conditions such as launch of specified process, using fullscreen windows, etc. are hold.

### Configuration
To configure energetiK one should create `energetikrc` file in `.config` directory with following content
```
[General]
InspectProcesses=true
InspectFullscreen=true

[ProcessInhibition]
Interval=1000
Processes=retroarch,vlc,...
```
where `InspectFullscreen` enables inhibition on switching of  any window in fullscreen mode and `InspectProcesses` enables inhibition on launch of certain process with `Interval` to be interval at which the service should actualize inhibition list and `Processes` to be list of processes that should cause inhibition of power management.
