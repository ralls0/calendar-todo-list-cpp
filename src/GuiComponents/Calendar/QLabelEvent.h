/**
 *
 * @author  Marco Manco Davide Manco
 * @date    04/12/21.
 * @file    QLabelEvent.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_QLABELEVENT_H
#define CALENDAR_TODO_LIST_CPP_QLABELEVENT_H

#include "../../CalendarEvent/CalendarEvent.h"
#include <QLabel>
#include <QMouseEvent>

class QLabelEvent : public QLabel {
  Q_OBJECT
private:
  CalendarEvent *event;

public:
  explicit QLabelEvent(QWidget *parent = 0);
  ~QLabelEvent();
  bool markSelection(bool selected);
  void setEvent(CalendarEvent *event);
  CalendarEvent *getEvent();
  bool drawUI();
  bool is_color_dark(QString colorName);

private:
  bool drawInvalidEvent();

protected:
  virtual void mousePressEvent(QMouseEvent *e);
  virtual void keyPressEvent(QKeyEvent *e);

signals:
  void clicked(QLabelEvent *event, Qt::MouseButton button);
  void keyPressed(int key);

public slots:
};

#endif // CALENDAR_TODO_LIST_CPP_QLABELEVENT_H
