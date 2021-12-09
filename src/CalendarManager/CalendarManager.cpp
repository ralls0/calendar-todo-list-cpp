/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    CalendarManager.cpp
 * @brief   Implementation file of class CalendarManager
 *
 */

#include "CalendarManager.h"

#define DEBUG_ 1
#if DEBUG_
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

CalendarManager::CalendarManager(QString iniFileName, QObject *parent)
    : QObject(parent) {
  _date = QDate::currentDate();
  _iniFileName = iniFileName;
  loadSettings();
}

CalendarManager::~CalendarManager() {
  saveSettings();
  clearCalendarList();
}

QList<QObject *> CalendarManager::eventsForDate(const QDate &date) {
  QList<QObject *> events;

  foreach (CalendarClient *pCalendarClient, _calendarList) {
    events.append(pCalendarClient->eventsForDate(date));
  }

  // sort events by time
  for (int x = 1; x < events.count(); x++) {
    for (int i = x; i < events.count(); i++) {
      if (((CalendarEvent *)events[i])->getStartDateTime().time() <
          ((CalendarEvent *)events[i - 1])->getStartDateTime().time()) {
        std::vector<int> *l = new std::vector<int>();
        l->push_back(i);
        l->push_back(i - 1);
        events.swap(reinterpret_cast<QList<struct QObject *> &>(l));
      }
    }
  }

  return events;
}

void CalendarManager::clearCalendarList(void) {
  foreach (QObject *pListItem, _calendarList) { pListItem->deleteLater(); }
  _calendarList.clear();
}

void CalendarManager::saveSettings(void) {
  QDEBUG << "[i] Store Setting..";
  SimpleCrypt crypto(PWD_CRYPT);
  QSettings settings(_iniFileName, QSettings::IniFormat);
  settings.clear();

  for (int index = 0; index < _calendarList.count(); index++) {
    settings.beginGroup("Calendar" + QString::number(index));
    settings.setValue("DisplayName", _calendarList.at(index)->getDisplayName());
    settings.setValue("URL", _calendarList.at(index)->getHostURL());
    settings.setValue("Color", _calendarList.at(index)->getColor());
    const CalendarClient_CalDAV *pCalDAVClient =
        (const CalendarClient_CalDAV *)_calendarList[index];
    if (pCalDAVClient->getClientAuth() ==
        CalendarClient_CalDAV::E_CalendarAuth::E_AUTH_UPWD) {
      settings.setValue("Type",
                        CalendarClient_CalDAV::E_CalendarAuth::E_AUTH_UPWD);
      settings.setValue("Username", pCalDAVClient->getUsername());
      settings.setValue("Password",
                        crypto.encryptToString(pCalDAVClient->getPassword()));
    } else {
      settings.setValue("Type",
                        CalendarClient_CalDAV::E_CalendarAuth::E_AUTH_TOKEN);
      settings.setValue("AccessToken", crypto.encryptToString(
                                           pCalDAVClient->getAccessToken()));
      settings.setValue("FilePath", pCalDAVClient->getFilePath());
    }
    QDEBUG << "[i] Store Calendar: "
           << _calendarList.at(index)->getDisplayName();
    settings.endGroup();
  }
}

void CalendarManager::loadSettings(void) {
  QDEBUG << "[i] Load Setting...";
  SimpleCrypt crypto(PWD_CRYPT);
  QSettings settings(_iniFileName, QSettings::IniFormat);

  bool bAbort = false;
  int index = 0;

  clearCalendarList();

  while (!bAbort) {

    QString group = "Calendar";
    group.append(QString::number(index));
    group.append("/");

    QString key = group;
    key.append("URL");
    QString url = settings.value(key, "").toString();

    key = group;
    key.append("DisplayName");
    QString displayName = settings.value(key, "").toString();

    key = group;
    key.append("Color");
    QString color = settings.value(key, "").toString();

    key = group;
    key.append("Type");
    int type = settings.value(key, -1).toInt();

    if (-1 != type) {
      if (0 == type) {
        key = group;
        key.append("Username");
        QString username = settings.value(key, "").toString();

        key = group;
        key.append("Password");
        QString password = settings.value(key, "").toString();
        if (!password.isEmpty()) {
          password = crypto.decryptToString(password);
        }

        addCalDAV_Calendar(color, displayName, QUrl(url), username, password,
                           CalendarClient_CalDAV::E_CalendarAuth::E_AUTH_UPWD);
      } else {
        key = group;
        key.append("AccessToken");
        QString accessToken = settings.value(key, "").toString();
        if (!accessToken.isEmpty()) {
          accessToken = crypto.decryptToString(accessToken);
        }

        key = group;
        key.append("FilePath");
        QString filepath = settings.value(key, "").toString();

        addCalDAV_Calendar(color, displayName, QUrl(url), filepath, accessToken,
                           CalendarClient_CalDAV::E_CalendarAuth::E_AUTH_TOKEN);
      }

      QDEBUG << "[i] Load Calendar: " << displayName;
    } else {
      bAbort = true;
    }
    index++;
  }
}

QDate CalendarManager::getDate() const { return _date; }

void CalendarManager::setDate(const QDate &newDate) {
  if (_date != newDate) {
    QDEBUG << "Date changed to " << newDate.toString();
    _date = newDate;
    foreach (CalendarClient *pListItem, _calendarList) {

      CalendarClient_CalDAV *pCalendarClient_CalDAV =
          (CalendarClient_CalDAV *)pListItem;
      pCalendarClient_CalDAV->setYear(newDate.year());
      pCalendarClient_CalDAV->setMonth(newDate.month());
    }
    emit dateChanged();
  }
}

QList<QObject *> CalendarManager::getListOfCalendars(void) {
  QList<QObject *> returnList;
  foreach (CalendarClient *pListItem, _calendarList) {
    returnList.append(pListItem);
  }
  return returnList;
}
QList<CalendarClient_CalDAV *> CalendarManager::getListOfCalendarsClient(void) {
  QList<CalendarClient_CalDAV *> returnList;
  foreach (CalendarClient_CalDAV *pListItem, _calendarList) {
    returnList.append(pListItem);
  }
  return returnList;
}

QList<QString> CalendarManager::getListOfCalendarsName(void) {
  QList<QString> returnList;
  foreach (CalendarClient *pListItem, _calendarList) {
    returnList.push_back(pListItem->getDisplayName());
  }
  return returnList;
}

QList<QObject *> CalendarManager::getListOfEvents(void) {
  QList<QObject *> returnList;
  foreach (CalendarClient *pListItem, _calendarList) {
    returnList.append(pListItem->allEvents());


  }

  return returnList;
}

void CalendarManager::addCalDAV_Calendar(
    QString color, QString calendarName, QUrl url, QString username,
    QString password, CalendarClient_CalDAV::E_CalendarAuth type) {
  CalendarClient_CalDAV *pCalendar;
  if (type == CalendarClient_CalDAV::E_CalendarAuth::E_AUTH_UPWD) {
    pCalendar = new CalendarClient_CalDAV(username, password, url.toString(),
                                          calendarName, this);
  } else {
    pCalendar =
        new CalendarClient_CalDAV(username, url.toString(), calendarName, this);
  }

  pCalendar->setColor(color);

  _calendarList.append(pCalendar);

  connect(pCalendar, SIGNAL(eventsUpdated()), this, SLOT(handleEventUpdate()));

  emit listOfCalendarsChanged();
}

CalendarClient *CalendarManager::getListItemAt(int index) {
  if ((index < 0) || (index >= _calendarList.count())) {
    QDEBUG << "ERROR: index" << index << "is invalid in _calendarList("
           << _calendarList.count() << ")";
    return NULL;
  }

  return _calendarList[index];
}

bool CalendarManager::removeListItemAt(int index) {
  if ((-1 == index) && (_calendarList.count() > 0)) {
    _calendarList.removeLast();
    emit listOfCalendarsChanged();
    return true;
  }

  if ((index >= 0) && (index < _calendarList.count())) {
    _calendarList.removeAt(index);
    emit listOfCalendarsChanged();
    return true;
  }

  QDEBUG << "ERROR: index" << index << "is invalid in _calendarList("
         << _calendarList.count() << ")";
  return false;
}

int CalendarManager::getCalendarCount(void) const {
  return _calendarList.count();
}

void CalendarManager::handleEventUpdate(void) {
  emit eventsUpdated();
  emit listOfEventsChanged();
}
