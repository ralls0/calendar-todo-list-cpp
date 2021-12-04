/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Utils.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_UTILS_H
#define CALENDAR_TODO_LIST_CPP_UTILS_H

#include <QColor>
#include <QWidget>

class WindowStyle : public QWidget {
  Q_OBJECT;

public:
  WindowStyle(QWidget *parent = nullptr);
  QString getMainWindowStyle();
  QString getLabelDateStyle();
  QString getCellStyle();

private:
  QColor _bg;
  QColor _fg;
  QString _holiday;
  QString _today;
  QString _selected;
  QString bg_rgb;
  QString fg_rgb;
};

#endif // CALENDAR_TODO_LIST_CPP_UTILS_H
