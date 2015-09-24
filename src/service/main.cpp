#include <QtCore/QCoreApplication>

#include "service.hpp"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("energetiK");
    app.setOrganizationName("pantonov");
    app.setOrganizationDomain("pvantonov.ru");
    app.setApplicationVersion("0.0.1");

    Service service;
    service.run();

    return app.exec();
}
