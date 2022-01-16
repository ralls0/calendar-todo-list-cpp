/**
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    CalendarManager.h
 * @brief   Manages a list of CalendarClients
 *          A CalendarManager handles a list of CalendarClient objects and acts
 *          as a central instance to obtain events for specific dates.
 *
 *          Usage:  The CalendarManager maintains a list of calDAV server based
 *                  CalendarClients.
 *
 *          The calendar settings are loaded at creation from an
 *          INI file which is specified as constructor parameter.
 *          At destruction, the CalendarManager stores the calendar settings to
 *          this file.
 *          Additional calendars can be added by calling addCalDAV_Calendar().
 *
 *          The listOfCalendars property provides a list of all maintained
 *          calendars. To remove an calendar from this list call
 *          removeListItemAt() with the respective index.
 *          To receive a list of all events from all calendars use
 *          the eventList property.
 *          However, if only events for a specific date are of interest,
 *          first set the corresponding month and year via the date property
 *          and call the eventsForDate() method with the questioned date as
 *          parameter.
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_CALENDARMANAGER_H
#define CALENDAR_TODO_LIST_CPP_CALENDARMANAGER_H

#include <QColor>
#include <QDate>
#include <QDebug>
#include <QObject>
#include <QSettings>
#include <QUrl>

#include "../ClientCalDAV/ClientCalDAV.h"
#include "../Utils/SimpleCrypt.h"

#define DEBUG_ 1
#if DEBUG_
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

#define PWD_CRYPT Q_UINT64_C(0x3A3CF524EC443FB1)

class CalendarManager : public QObject {
  Q_OBJECT

public:
  Q_PROPERTY(QDate date READ getDate WRITE setDate NOTIFY dateChanged)

  Q_PROPERTY(QList<QObject *> listOfCalendars READ getListOfCalendars NOTIFY
                 listOfCalendarsChanged)

  Q_PROPERTY(QList<QObject *> eventList READ getListOfEvents NOTIFY
                 listOfEventsChanged)

  CalendarManager(QString iniFileName, QObject *parent = NULL);

  ~CalendarManager();

  /**
   * @brief Returns a list of events which occur on a specific date
   *
   * @note In case month or year have changed, it is necessary to set the date
   * property first
   * @see setDate()
   */
  Q_INVOKABLE QList<QObject *> eventsForDate(const QDate &date);

protected:
  /**
   * @brief Elimina la lista di ClientsCalDAV (_calendarList)
   */
  void clearCalendarList(void);

  /**
   * @brief Saves calendar settings to INI file
   *
   * Saved information for each CalendarClient instance: type, display name,
   * color, URL and in case of calDAV calendars also the username and password.
   * Passwords are save encrypted (using SimpleCrypt) so at least you won't
   * have any files containing your owncloud password in plaintext on your disk.
   */
  void saveSettings(void);

  /**
   * @brief Loads calendar settings from INI file
   *
   * @note _calendarList will be erased and rebuild during this procedure!
   *
   * @see saveSettings()
   */
  void loadSettings(void);

  QList<ClientCalDAV *> _calendarList; // list of CalDAV calendars
  QString _iniFileName;
  QDate _date;

signals:
  void dateChanged(void);
  void listOfCalendarsChanged(
      QList<QObject *> t); // emitted when _calendarList has changed

  void
  listOfEventsChanged(QList<QObject *> t); // emitted during handleEventUpdate()
  void eventsUpdated(void);                // emitted during handleEventUpdate()
  void setToDoList(QString acc);
  void errorCalendarOccured();

public slots:
  void handleDisplayNameChanged(void);
  void handleLoadSetting(void);
  QDate getDate() const;
  void setDate(const QDate &newDate);
  void handleErrorCalendarOccured(void);

  QList<QObject *> getListOfCalendars(void);
  QList<QObject *> getListOfEvents(void);
  // QList<QObject *>  listOfTaskChanged(void);
  void addCalendarCalDAVUP(QString calendarName, QString url, QString username,
                           QString password);

  void addCalendarCalDAVOA(QString calendarName, QString url, QString filepath);

  /**
   * @brief Returns a pointer to a specific calendar instance in the list of
   * calendars
   */
  ClientCalDAV *getListItemAt(int index); // FIXME: quando viene usata
  void setToDo(QString accT);
protected slots:

  /**
   * @brief Slot to be called when a CalendarClient instance reports an event
   * update
   */
  void handleEventUpdate(void);
  void handleErrorOAuthCalendar(QString displayName);
};

#endif // CALENDAR_TODO_LIST_CPP_CALENDARMANAGER_H
