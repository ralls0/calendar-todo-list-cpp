/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    QPushButtonExtended.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_QPUSHBUTTONEXTENDED_H
#define CALENDAR_TODO_LIST_CPP_QPUSHBUTTONEXTENDED_H

#include "Event.h"
#include <QPushButton>

class QPushButtonExtended : public QPushButton {
  Q_OBJECT
private:
  int index;
  Event *ev;

public:
  explicit QPushButtonExtended(const char *text, QWidget *parent = 0);
  QPushButtonExtended(QWidget *parent = 0);
  void setIndex(int index);
  void button_clicked();
  void button_edit_clicked();
  void setEvent(Event *event) { ev = event; };
  Event *getEvent() { return ev; };

signals:
  void on_click(int index);
  ;
  void on_click_edit(QPushButtonExtended *d);
public slots:
};

#endif // CALENDAR_TODO_LIST_CPP_QPUSHBUTTONEXTENDED_H