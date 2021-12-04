#include "src/GuiComponents/Calendar/Calendar.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  MainCalendar m;
  m.show();

  return a.exec();
}
