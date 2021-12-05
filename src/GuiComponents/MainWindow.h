/**
 *
 * @author  Marco Manco
 * @date    05/12/21.
 * @file    MainWindow.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_MAINWINDOW_H
#define CALENDAR_TODO_LIST_CPP_MAINWINDOW_H

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

#include "../CalendarManager/CalendarManager.h"
#include "../ClientCalDAV/CalendarClient.h"
#include "../ClientCalDAV/CalendarClient_CalDAV.h"
#include "Calendar/Calendar.h"
#include "NewCalendar/NewCalendarDialog.h"
#include "NewEvent/NewEventDialog.h"

class MainWindow : public QWidget {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

public slots:
  void createNewCalendar(const QString &displayName, const QString &hostURL,
                         bool isBasicAuth, const QString &username,
                         const QString &password, const QString &clientSecret);
  void createNewEvent(QString uid, QString filename, QString summary,
                      QString location, QString description, QString rrule,
                      QDateTime startDateTime, QDateTime endDateTime,
                      QString calendar);

  void createNewCalendarDialog();
  void createNewEventDialog();

private:
  void createPreviewGroupBox();
  QString checkDisplayName(QList<CalendarClient_CalDAV *> cals,
                           QString displayName);

  QGroupBox *_previewGroupBox;
  QGridLayout *_previewLayout;
  NewCalendarDialog *_newCalendarDialog;
  NewEventDialog *_newEventDialog;
  MainCalendar *_calendar;

  CalendarManager *_cals;
};

#endif // CALENDAR_TODO_LIST_CPP_MAINWINDOW_H
