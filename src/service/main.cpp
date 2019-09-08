#include <QtGui/QGuiApplication>

#include "service.hpp"


int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("energetiK");
    app.setOrganizationName("pantonov");
    app.setOrganizationDomain("pvantonov.ru");
    app.setApplicationVersion("0.2.1");

    Service service;
    service.run();

    return app.exec();
}
