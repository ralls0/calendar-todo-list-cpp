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
#include <QDebug>
#include <QWidget>

class WindowStyle : public QWidget {
  Q_OBJECT;

public:
  WindowStyle(QWidget *parent = nullptr);
  QString getMainWindowStyle();
  QString getLabelDateStyle();
  QString getCellStyle();
  QString getDialogStyle();

private:
  QColor _bg;
  QColor _fg;
  QString _dark;
  QString _red;
  QString _selected;
  QString bg_hex;
  QString fg_hex;
};

#endif // CALENDAR_TODO_LIST_CPP_UTILS_H
