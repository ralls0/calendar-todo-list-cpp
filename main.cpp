#include <QApplication>
#include <iostream>

#include "./src/GuiComponents/NewCalendar/WindowNewCalendar.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  WindowNewCalendar window;
  window.show();
  return a.exec();
}
