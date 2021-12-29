//
// Created by Davide on 27/12/2021.
//

#ifndef CALENDAR_TODO_LIST_CPP_QPUSHBUTTONEXTENDEDTD_H
#define CALENDAR_TODO_LIST_CPP_QPUSHBUTTONEXTENDEDTD_H

#include "../../Task/TaskElement.h"
#include <QPushButton>

class QPushButtonExtendedTD : public QPushButton {
  Q_OBJECT
private:
  int index;
  TaskElement *te;

public:
  explicit QPushButtonExtendedTD(const char *text, QWidget *parent = 0);
  void button_edit_clicked();
  void button_clicked_delete();
  void setTask(TaskElement *ted) { te = ted; };
  TaskElement *getTask() { return te; };

signals:
  void on_click_delete(QPushButtonExtendedTD *d);
  void on_click_edit(QPushButtonExtendedTD *d);
};

#endif // CALENDAR_TODO_LIST_CPP_QPUSHBUTTONEXTENDEDTD_H
