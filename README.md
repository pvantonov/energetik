# energetiK
energetiK (Russian word for energy drink, pronounced [enɛrˈɡʲetʲɪk]) is KDE service to inhibit power management if certain conditions such as launch of specified process, using fullscreen windows, etc. are hold.

### Configuration
To configure energetiK one should create `energeticrc` file in `.config` directory with following content
```
[General]
Interval=1000

[Inhibition]
Processes=retroarch,vlc,...
```
where `Interval` is interval at which the service should actualize inhibition list and `Processes` is list of processes that should cause inhibition of power management.
