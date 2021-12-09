/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    CalendarEvent.h
 * @brief   Class for Calendar events.
 *          CalendarEvent represents an event within an ICS or CalDAV calendar.
 *
 *          When owned by a CalendarClient object, the start date and end date
 * of the CalendarEvent object are stored as specified by the CalDAV server or
 * ICS file. Thus the event might apply to several dates if a RRULE value is
 * set.
 *
 */


#ifndef CALENDAR_TODO_LIST_CPP_CALENDAREVENT_H
#define CALENDAR_TODO_LIST_CPP_CALENDAREVENT_H

#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QString>

class CalendarEvent : public QObject {
  Q_OBJECT

public:
  // Display name of the calendar where this event originates from
  Q_PROPERTY(QString calendarName READ calendarName WRITE setCalendarName NOTIFY
                 calendarNameChanged)

  // Color of the calendar where this event originates from
  Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY colorChanged)

  // Summary or title of the event
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

  // Location, where the event occurs
  Q_PROPERTY(
      QString location READ location WRITE setLocation NOTIFY locationChanged)

  // Detailed description of the event
  Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY
                 descriptionChanged)

  // Start date and time; event applies since this point in time
  Q_PROPERTY(QDateTime startDateTime READ getStartDateTime WRITE
                 setStartDateTime NOTIFY startDateTimeChanged)

  // End date and time; event applies until this point in time
  Q_PROPERTY(QDateTime endDateTime READ getEndDateTime WRITE setEndDateTime
                 NOTIFY endDateTimeChanged)

  // iCalendar RRULE value as string
  Q_PROPERTY(QString rrule READ getRRULE WRITE setRRULE NOTIFY rruleChanged)

  // iCalendar EXDATE value as string
  Q_PROPERTY(
      QString exdates READ getExdates WRITE setExdates NOTIFY exdatesChanged)

  // Event categories, separated by ","
  Q_PROPERTY(QString categories READ getCategories WRITE setCategories NOTIFY
                 categoriesChanged)

  // Unique ID of event
  Q_PROPERTY(QString uid READ getUID WRITE setUID NOTIFY uidChanged)

  // URL of calendar file
  Q_PROPERTY(QString href READ getHREF WRITE setHREF NOTIFY hrefChanged)

  // Specify if is an event or acivity
  Q_PROPERTY(bool isCalendar READ getIsCalendar WRITE setIsCalendar NOTIFY
                 isCalendarChanged)

  // Read-only boolean property which is set, if the event represents a specific
  // occurrence and an EXDATE value applies
  Q_PROPERTY(bool isCanceled READ getIsCanceled NOTIFY isCanceledChanged)

public:
  explicit CalendarEvent(QObject *parent);
  CalendarEvent(const CalendarEvent &other);

public:
  CalendarEvent &operator=(const CalendarEvent &other);

  bool operator<(const CalendarEvent &other) const;

  void copyFrom(const CalendarEvent &other);

signals:
  void colorChanged(QString &color);
  void calendarNameChanged(const QString &calendarName);
  void nameChanged(const QString &name);
  void locationChanged(const QString &location);
  void descriptionChanged(const QString &description);
  void startDateTimeChanged(const QDateTime &startDateTime);
  void endDateTimeChanged(const QDateTime &endDateTime);
  void rruleChanged(const QString &rrule);
  void exdatesChanged(const QString &exdates);
  void categoriesChanged(const QString &categories);
  void uidChanged(const QString &uid);
  void hrefChanged(const QString &href);
  void isCanceledChanged(const bool &isCanceled);
  void isCalendarChanged(const bool &isCalendar);

public slots:
  QString getColor(void) const;
  void setColor(const QString &color);

  QString calendarName() const;
  void setCalendarName(const QString &calendarName);

  QString name() const;
  void setName(const QString &name);

  QString location() const;
  void setLocation(const QString &location);

  QString description() const;
  void setDescription(const QString &description);

  QDateTime getStartDateTime() const;
  void setStartDateTime(const QDateTime &startDateTime);

  QDateTime getEndDateTime(void) const;
  void setEndDateTime(const QDateTime &endDateTime);

  QString getRRULE() const;
  void setRRULE(const QString &rRule);

  QString getExdates() const;
  void setExdates(const QString &exdates);

  QString getCategories() const;
  void setCategories(const QString &categories);

  bool getIsCanceled() const;
  void setIsCanceled(const bool &isCanceled);

  bool getIsCalendar() const;
  void setIsCalendar(const bool &isCanceled);

  QString getUID(void) const;
  void setUID(const QString &uid);

  QString getHREF(void) const;
  void setHREF(const QString &href);

  // to edit an event and upload it back to the CalDAV server we need to know
  // the CalendarClient object where this event originates from
  QObject *getCalendarPointer(void) const;

  void setCalendarPointer(QObject *ptr);

protected:
  QString _color;
  QString _calendarName;
  QString _name;
  QString _location;
  QString _description;
  QDateTime _startDateTime;
  QDateTime _endDateTime;
  QString _rRULE;
  QString _exdates;
  QString _categories;
  QString _UID;
  QString _HREF;
  bool _isCanceled;
  bool _isCalendar;
  QObject *_calendarPointer;
};

#endif // CALENDAR_TODO_LIST_CPP_CALENDAREVENT_H
