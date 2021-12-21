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
#include "../ClientCalDAV/ClientCalDAV.h"
#include "./Calendar/Calendar.h"
#include "./NewCalendar/NewCalendarDialog.h"
#include "./NewEvent/NewEventDialog.h"
#include "../GuiComponents/ToDoList/ToDoList.h"
class MainWindow : public QWidget {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void createNewCalendar(const QString &displayName, const QString &hostURL,
                         bool isBasicAuth, const QString &username,
                         const QString &password, const QString &clientSecret);
  void createNewEvent(QString uid, QString filename, QString summary,
                      QString location, QString description, QString rrule,
                      QDateTime startDateTime, QDateTime endDateTime,
                      QString calendar);
  void deleteEvent(CalendarEvent *t);
  void createNewCalendarDialog();
  void createNewEventDialogM(CalendarEvent *event = nullptr);
  void createNewEventDialog();

private:
  void createPreviewGroupBox();
  QString checkDisplayName(QList<QObject *> cals, QString displayName);

  QGroupBox *_previewGroupBox;
  QGridLayout *_previewLayout;
  NewCalendarDialog *_newCalendarDialog;
  NewEventDialog *_newEventDialog;
  MainCalendar *_calendar;
  ToDoList *_todo;
  CalendarManager *_cals;
};

#endif // CALENDAR_TODO_LIST_CPP_MAINWINDOW_H
