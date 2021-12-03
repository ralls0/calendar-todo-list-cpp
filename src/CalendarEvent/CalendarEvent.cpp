/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    CalendarEvent.cpp
 * @brief
 *
 */

#include "CalendarEvent.h"

CalendarEvent::CalendarEvent(QObject *parent) : QObject(parent) {
  _calendarName = "unnamed";
  _name = "";
  _location = "";
  _description = "";
  _startDateTime = QDateTime();
  _endDateTime = QDateTime();
  _categories = "";
  _exdates = "";
  _rRULE = "";
  _color = QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF).name();
  _isCanceled = false;
  _isCalendar = true;
  _UID = "";
  _HREF = "";
  _calendarPointer = NULL;
}

CalendarEvent::CalendarEvent(const CalendarEvent &other) : QObject() {
  copyFrom(other);
}

CalendarEvent &CalendarEvent::operator=(const CalendarEvent &other) {
  copyFrom(other);
  return (*this);
}

bool CalendarEvent::operator<(const CalendarEvent &other) const {
  return (this->_startDateTime < other._startDateTime);
}

void CalendarEvent::copyFrom(const CalendarEvent &other) {
  setColor(other._color);
  setCalendarName(other._calendarName);
  setName(other._name);
  setLocation(other._location);
  setDescription(other._description);
  setStartDateTime(other._startDateTime);
  setEndDateTime(other._endDateTime);
  setCategories(other._categories);
  setExdates(other._exdates);
  setRRULE(other._rRULE);
  setIsCanceled(other._isCanceled);
  setUID(other._UID);
  setHREF(other._HREF);
  setCalendarPointer(other._calendarPointer);
  setParent(other.parent());
}

QString CalendarEvent::getColor(void) const { return _color; }

void CalendarEvent::setColor(const QString &color) {
  if (color != _color) {
    _color = color;
    emit colorChanged(_color);
  }
}

QString CalendarEvent::calendarName() const { return _calendarName; }

void CalendarEvent::setCalendarName(const QString &calendarName) {
  if (calendarName != _calendarName) {
    _calendarName = calendarName;
    emit calendarNameChanged(_calendarName);
  }
}

QString CalendarEvent::name() const { return _name; }

void CalendarEvent::setName(const QString &name) {
  if (name != _name) {
    _name = name;
    emit nameChanged(_name);
  }
}

void CalendarEvent::setCalendarPointer(QObject *ptr) { _calendarPointer = ptr; }

QObject *CalendarEvent::getCalendarPointer(void) const {
  return _calendarPointer;
}

QString CalendarEvent::location() const { return _location; }

void CalendarEvent::setLocation(const QString &location) {
  if (location != _location) {
    _location = location;
    emit locationChanged(_location);
  }
}

QString CalendarEvent::description() const { return _description; }

void CalendarEvent::setDescription(const QString &description) {
  if (description != _description) {
    _description = description;
    emit descriptionChanged(_description);
  }
}

QDateTime CalendarEvent::getStartDateTime(void) const { return _startDateTime; }

void CalendarEvent::setStartDateTime(const QDateTime &startDateTime) {
  if (startDateTime != _startDateTime) {
    _startDateTime = startDateTime;
    emit startDateTimeChanged(_startDateTime);
  }
}

QDateTime CalendarEvent::getEndDateTime(void) const { return _endDateTime; }

void CalendarEvent::setEndDateTime(const QDateTime &endDateTime) {
  if (endDateTime != _endDateTime) {
    _endDateTime = endDateTime;
    emit endDateTimeChanged(_endDateTime);
  }
}

QString CalendarEvent::getRRULE() const { return _rRULE; }

void CalendarEvent::setRRULE(const QString &rrule) {
  if (_rRULE != rrule) {
    _rRULE = rrule;
    emit rruleChanged(_rRULE);
  }
}

QString CalendarEvent::getExdates() const { return _exdates; }

void CalendarEvent::setExdates(const QString &exdates) {
  if (_exdates != exdates) {
    _exdates = exdates;
    emit exdatesChanged(_exdates);
  }
}

QString CalendarEvent::getCategories() const { return _categories; }

void CalendarEvent::setCategories(const QString &categories) {
  if (_categories != categories) {
    _categories = categories;
    emit categoriesChanged(_categories);
  }
}

bool CalendarEvent::getIsCanceled() const { return _isCanceled; }

void CalendarEvent::setIsCanceled(const bool &isCanceled) {
  if (_isCanceled != isCanceled) {
    _isCanceled = isCanceled;
    emit isCanceledChanged(_isCanceled);
  }
}

bool CalendarEvent::getIsCalendar() const { return _isCalendar; }

void CalendarEvent::setIsCalendar(const bool &isCalendar) {
  if (_isCalendar != isCalendar) {
    _isCalendar = isCalendar;
    emit isCanceledChanged(_isCalendar);
  }
}

QString CalendarEvent::getUID(void) const { return _UID; }

QString CalendarEvent::getHREF(void) const { return _HREF; }

void CalendarEvent::setUID(const QString &uid) {
  if (uid != _UID) {
    _UID = uid;
    emit uidChanged(_UID);
  }
}

void CalendarEvent::setHREF(const QString &href) {
  if (href != _HREF) {
    _HREF = href;
    emit hrefChanged(_HREF);
  }
}
