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

#include "../CalendarManager/CalendarManager.h"
#include "../ClientCalDAV/ClientCalDAV.h"
#include "../GuiComponents/ToDoList/ToDoList.h"
#include "../Task/TasksManager.h"
#include "./Calendar/Calendar.h"
#include "./NewCalendar/NewCalendarDialog.h"
#include "./NewEvent/NewEventDialog.h"
#include "./ShareCalendar/ShareCalendarDialog.h"
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
#include <QVariant>
#include <QWidget>
#include <iostream>

#define DEBUG_ 1
#if DEBUG_
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

class MainWindow : public QWidget {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void createNewCalendar(const QString &displayName, const QString &hostURL,
                         bool isBasicAuth, const QString &username,
                         const QString &password, const QString &clientSecret);
  void createShareCalendar(QString displayName, QString email,
                           QString comments);
  void createNewEvent(QString uid, QString filename, QString summary,
                      QString location, QString description, QString rrule,
                      QDateTime startDateTime, QDateTime endDateTime,
                      QString calendar);
  void deleteEvent(CalendarEvent *t);
  void createNewCalendarDialog();
  void createNewEventDialogM(CalendarEvent *event = nullptr);
  void createNewEventDialog();
  void createShareCalendarDialog();
  void createToDo(QString acc);
  void createNewTaskDialog(TaskElement *te);
  void deleteTask(TaskElement *te);
  void modifyTask(QString name, QDateTime t, QString idT, TaskElement *te);
  void newTask(QString name, QDateTime t);

private:
  void createPreviewGroupBox();
  QString checkDisplayName(QList<QObject *> cals, QString displayName);

  QGroupBox *_previewGroupBox;
  QGridLayout *_previewLayout;
  NewCalendarDialog *_newCalendarDialog;
  NewEventDialog *_newEventDialog;
  ShareCalendarDialog *_shareCalendarDialog;
  MainCalendar *_calendar;
  ToDoList *_todo;
  CalendarManager *_cals;
  TasksManager *_taskm;
};

#endif // CALENDAR_TODO_LIST_CPP_MAINWINDOW_H
