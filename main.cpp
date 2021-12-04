#include "src/GuiComponents/Calendar/maincalendar.h"

#include <QApplication>


#include<QWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainCalendar m;
    m.show();
    return a.exec();
}
