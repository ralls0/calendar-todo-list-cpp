#include <QApplication>
#include <QPushButton>
#include <iostream>

#include "src/CalDAV/CalendarClient_CalDAV.h"

int main (int argc, char *argv[]) {
    QString color = "E06C75";
    QString displayName = "Google Calendar";
    int year = 2021;
    int month = 11;
    QDateTime dateStart = QDateTime();
    QDateTime dateEnd = QDateTime();

    QDate sdate = QDate(2021, 12, 22);
    QDate edate = QDate(2022, 1, 6);

    dateStart.setDate(sdate);
    dateEnd.setDate(edate);

    QString format = "dd.MM.yyyy";
    std::cout<<"Start date: "<<dateStart.toString(format).toStdString()<<" End date: "<<dateEnd.toString(format).toStdString()<<"\n";

    QApplication a(argc, argv);

    // Create Client CalDAV
    CalendarClient_CalDAV *clientCalDav = new CalendarClient_CalDAV(nullptr);

    // Set credential
    clientCalDav->setUsername("pdsmariorossi@gmail.com");
    clientCalDav->setPassword("Pdsmr98.");

    // Set host URL
    QString surl = "https://apidata.googleusercontent.com/caldav/v2/pdsmariorossi@gmail.com/events/";
    QUrl url = surl;
    clientCalDav->setHostURL(url);

    // Set minor parameters
    clientCalDav->setColor(color);
    clientCalDav->setDisplayName(displayName);
    clientCalDav->setRequestTimeoutMS(32000);

    // Set date
    clientCalDav->setYear(year);
    clientCalDav->setMonth(month);

    clientCalDav->startSynchronization();
    std::cout<<"Client State:"<<clientCalDav->getSyncState()<<"\n";

    clientCalDav->saveEvent(
            "",
            "",
            "Prova",
            "Casa mia",
            "Questo e' un evento di prova",
            "",
            "",
            dateStart,
            dateEnd
            );
    clientCalDav->startSynchronization();
    std::cout<<"Client State:"<<clientCalDav->getSyncState()<<"\n";


    return QApplication::exec();
}
