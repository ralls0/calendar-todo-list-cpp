#include <QApplication>
#include <QPushButton>
#include "CalendarClient/CalendarClient_CalDAV.h"

int main (int argc, char *argv[]) {
    QApplication a(argc, argv);
    CalendarClient_CalDAV *cc = new CalendarClient_CalDAV();
    return QApplication::exec();
}
