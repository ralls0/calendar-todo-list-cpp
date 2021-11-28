/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    WindowNewCalendar.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_WINDOWNEWCALENDAR_H
#define CALENDAR_TODO_LIST_CPP_WINDOWNEWCALENDAR_H

#include <QWidget>
#include <iostream>

#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QTextCharFormat>

#include "NewCalendarDialog.h"

class WindowNewCalendar : public QWidget {
  Q_OBJECT

public:
  WindowNewCalendar(QWidget *parent = nullptr);

private:
  void createPreviewGroupBox();

  QGroupBox *_previewGroupBox;
  QGridLayout *_previewLayout;
  NewCalendarDialog *_newCalendarDialog;
};

#endif // CALENDAR_TODO_LIST_CPP_WINDOWNEWCALENDAR_H
