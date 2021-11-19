#include <QApplication>
#include <QPushButton>
#include <iostream>

#include "./OAuth/OAuth.h"
#include "./ClientCalDAV/ClientCalDAV.h"

int main (int argc, char *argv[]) {
    QApplication a(argc, argv);

    QDateTime dateStart = QDateTime();
    QDateTime dateEnd = QDateTime();

    QDate sdate = QDate(2021, 12, 22);
    QDate edate = QDate(2022, 1, 6);

    dateStart.setDate(sdate);
    dateEnd.setDate(edate);

    QString filename = QDir::currentPath()+"/client_setup.json";
    QString scope = "https://www.googleapis.com/auth/calendar";// https://www.googleapis.com/auth/userinfo.email";

    std::cout<<"Create obj\n";
    OAuth au(filename, scope);
    ClientCalDAV client(
            "pdsmariorossi@gmail.com",
            "Pdsmr98.",
            QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF).name(),
            QUrl("https://apidata.googleusercontent.com/caldav/v2/pdsmariorossi@gmail.com/events/"),
            "pds_project"
            );

    QObject::connect(&au, &OAuth::accessTokenChanged,
                     &client, &ClientCalDAV::setAccessToken);

    std::cout<<au.toString().toStdString()<<"\n";
    au.startAuth();

    /*client.saveEvent(
            "",
            "",
            "Prova",
            "Casa mia",
            "Messappunto Google OAuth",
            "",
            "",
            dateStart,
            dateEnd
    );*/

    return QApplication::exec();
}
