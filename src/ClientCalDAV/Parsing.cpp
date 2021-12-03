/**
 *
 * @author  Marco Manco
 * @date    03/12/21.
 * @file    Parsing.cpp
 * @brief
 *
 */

#include "CalendarClient.h"

#define DEBUG_CALENDARCLIENT 1
#if DEBUG_CALENDARCLIENT
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

void CalendarClient::parseCALENDAR(QString href) {
  QString line = _dataStream->readLine();

  while (false == line.isNull()) {
    if (false != line.contains("BEGIN:VEVENT")) {
      parseVEVENT(href);
    }
    line = _dataStream->readLine();
  }
}

void CalendarClient::parseVEVENT(QString href) {
  CalendarEvent event(this);
  event.setColor(_color);
  event.setCalendarName(_displayName);
  event.setCalendarPointer(this);
  event.setHREF(href);
  QString line;
  QDateTime utcTime;
  while (!(line = _dataStream->readLine()).contains(QByteArray("END:VEVENT"))) {
    // QDEBUG << _displayName << ": " << line;

    const int deliminatorPosition = line.indexOf(QLatin1Char(':'));
    const QString key = line.mid(0, deliminatorPosition);
    QString value = (line.mid(deliminatorPosition + 1, -1)
                         .replace("\\n", "\n")); //.toLatin1();
    QString testEncodingString = ascii2utf8(value);
    if (false == testEncodingString.contains("�")) {
      value = testEncodingString;
    }

    if (key.startsWith(QLatin1String("DTSTART"))) {
      utcTime = QDateTime::fromString(value, "yyyyMMdd'T'hhmmss'Z'");
      if (!utcTime.isValid())
        utcTime = QDateTime::fromString(value, "yyyyMMdd'T'hhmmss");
      if (!utcTime.isValid())
        utcTime = QDateTime::fromString(value, "yyyyMMddhhmmss");
      if (!utcTime.isValid())
        utcTime = QDateTime::fromString(value, "yyyyMMdd");
      if (!utcTime.isValid())
        QDEBUG << _displayName << ": "
               << "could not parse" << line;

      event.setStartDateTime(utcTime.toLocalTime());
    } else if (key.startsWith(QLatin1String("DTEND"))) {
      utcTime = QDateTime::fromString(value, "yyyyMMdd'T'hhmmss'Z'");
      if (!utcTime.isValid())
        utcTime = QDateTime::fromString(value, "yyyyMMdd'T'hhmmss");
      if (!utcTime.isValid())
        utcTime = QDateTime::fromString(value, "yyyyMMddhhmmss");
      if (!utcTime.isValid())
        utcTime = QDateTime::fromString(value, "yyyyMMdd");
      if (!utcTime.isValid())
        QDEBUG << _displayName << ": "
               << "could not parse" << line;

      event.setEndDateTime(utcTime.toLocalTime());
    } else if (key == QLatin1String("RRULE")) {
      event.setRRULE(value);
    } else if (key == QLatin1String("EXDATE")) {
      event.setExdates(value);
    } else if (key == QLatin1String("SUMMARY")) {
      event.setName(value);
    } else if (key == QLatin1String("LOCATION")) {
      event.setLocation(value);
    } else if (key == QLatin1String("UID")) {
      event.setUID(value);
    } else if (key == QLatin1String("CATEGORIES")) {
      event.setCategories(value);
    } else if (key == QLatin1String("DESCRIPTION")) {
      event.setDescription(value);
    }
  }
  if (event.name() != "") {
    _eventList.append(event);
  }
}
