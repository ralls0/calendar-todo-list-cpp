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

#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QTextCharFormat>
#include <QWidget>
#include <iostream>

#include "../../ClientCalDAV/CalendarClient_CalDAV.h"
#include "../NewEvent/NewEventDialog.h"
#include "NewCalendarDialog.h"

class WindowNewCalendar : public QWidget {
  Q_OBJECT

public:
  WindowNewCalendar(QWidget *parent = nullptr);

public slots:
  void createNewCalendar(const QString &displayName, const QString &hostURL,
                         bool isBasicAuth, const QString &username,
                         const QString &password, const QString &clientSecret);

private:
  void createPreviewGroupBox();

  QGroupBox *_previewGroupBox;
  QGridLayout *_previewLayout;
  NewCalendarDialog *_newCalendarDialog;
  NewEventDialog *_newEventDialog;

  CalendarClient_CalDAV *_cals;
};

#endif // CALENDAR_TODO_LIST_CPP_WINDOWNEWCALENDAR_H
