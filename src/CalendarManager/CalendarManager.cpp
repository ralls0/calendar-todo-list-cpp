/**
 *
 * @author  Marco Manco Davide Manco
 * @date    28/11/21.
 * @file    CalendarManager.cpp
 * @brief   Implementation file of class CalendarManager
 *
 */

#include "CalendarManager.h"

CalendarManager::CalendarManager(QString iniFileName, QObject *parent)
    : QObject(parent) {
  _date = QDate::currentDate();
  _iniFileName = iniFileName;
}

CalendarManager::~CalendarManager() {
  saveSettings();
  clearCalendarList();
}

QList<QObject *> CalendarManager::eventsForDate(const QDate &date) {
  QList<QObject *> events;

  foreach (ClientCalDAV *pClientCalDAV, _calendarList) {
    events.append(pClientCalDAV->eventsForDate(date));
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
    const ClientCalDAV *pCalDAVClient =
        (const ClientCalDAV *)_calendarList[index];
    if (pCalDAVClient->getClientAuth() ==
        ClientCalDAV::E_CalendarAuth::E_AUTH_UPWD) {
      settings.setValue("Type", ClientCalDAV::E_CalendarAuth::E_AUTH_UPWD);
      settings.setValue("Username", pCalDAVClient->getUsername());
      settings.setValue("Password",
                        QString(pCalDAVClient->getPassword().toUtf8().toBase64()));
    } else {
      settings.setValue("Type", ClientCalDAV::E_CalendarAuth::E_AUTH_TOKEN);
      settings.setValue("FilePath", pCalDAVClient->getFilePath());
    }
    settings.endGroup();
    QDEBUG << "[i] Stored calendar: "
           << _calendarList.at(index)->getDisplayName();
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
          password = QString(QByteArray::fromBase64(QByteArray(password.toStdString().c_str(), password.length())));
        }

        addCalendarCalDAVUP(displayName, url, username, password);
      } else {
        key = group;
        key.append("FilePath");
        QString filepath = settings.value(key, "").toString();

        addCalendarCalDAVOA(displayName, url, filepath);
      }

      QDEBUG << "[i] Loaded calendar: " << displayName;
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
    foreach (ClientCalDAV *pListItem, _calendarList) {

      ClientCalDAV *pClientCalDAV = (ClientCalDAV *)pListItem;
      pClientCalDAV->setYear(newDate.year());
      pClientCalDAV->setMonth(newDate.month());
    }
    emit dateChanged();
  }
}

QList<QObject *> CalendarManager::getListOfCalendars(void) {
  QList<QObject *> returnList;
  foreach (ClientCalDAV *pListItem, _calendarList) {
    returnList.append(pListItem);
  }
  return returnList;
}

QList<QObject *> CalendarManager::getListOfEvents(void) {
  QList<QObject *> returnList;
  foreach (ClientCalDAV *pListItem, _calendarList) {
    returnList.append(pListItem->eventsInRange(
        QDate(_date.year(), _date.month(), 1),
        QDate(_date.year(), _date.month(), 1).addMonths(1).addDays(-1)));
  }

  return returnList;
}

void CalendarManager::addCalendarCalDAVUP(QString calendarName, QString url,
                                          QString username, QString password) {
  ClientCalDAV *pCalendar =
      new ClientCalDAV(username, password, url, calendarName, this);
  _calendarList.append(pCalendar);
  connect(pCalendar, &ClientCalDAV::eventsUpdated, this, &CalendarManager::handleEventUpdate);
  connect(pCalendar, &ClientCalDAV::displayNameChanged, this,
          &CalendarManager::handleDisplayNameChanged);
  connect(pCalendar, &ClientCalDAV::errorOccured, this,
          &CalendarManager::handleErrorCalendarOccured);
  QDEBUG << "[i] Emit listOfCalendarsChanged";
  emit listOfCalendarsChanged(this->getListOfCalendars());
}

void CalendarManager::addCalendarCalDAVOA(QString calendarName, QString url,
                                          QString filepath) {
  ClientCalDAV *pCalendar = new ClientCalDAV(filepath, url, calendarName, this);
  connect(pCalendar, &ClientCalDAV::accessTokenError, this,
          &CalendarManager::handleErrorOAuthCalendar);
  _calendarList.append(pCalendar);
  connect(pCalendar, SIGNAL(eventsUpdated()), this, SLOT(handleEventUpdate()));
  connect(pCalendar, &ClientCalDAV::displayNameChanged, this,
          &CalendarManager::handleDisplayNameChanged);
  connect(pCalendar, &ClientCalDAV::errorOccured, this,
          &CalendarManager::handleErrorCalendarOccured);
  QDEBUG << "[i] New calendar added";
  QDEBUG << "[i] Emit listOfCalendarsChanged";
  emit listOfCalendarsChanged(this->getListOfCalendars());
  connect(pCalendar, &ClientCalDAV::accessTokenChanged, this,
          &CalendarManager::setToDo);
}

void CalendarManager::setToDo(QString accT) { emit setToDoList(accT); }

ClientCalDAV *CalendarManager::getListItemAt(int index) {
  if ((index < 0) || (index >= _calendarList.count())) {
    QDEBUG << "[e] ERROR: index" << index << "is invalid in _calendarList("
           << _calendarList.count() << ")";
    return nullptr;
  }

  return _calendarList[index];
}

void CalendarManager::handleEventUpdate(void) {
  emit listOfEventsChanged(this->getListOfEvents());
}

void CalendarManager::handleErrorOAuthCalendar(QString displayName) {
  int i = 0;
  foreach (ClientCalDAV *pListItem, _calendarList) {
    if (pListItem->getDisplayName() == displayName) {
      QDEBUG << "[e] (CalendarManager) Error remove calendar: " << displayName;
      _calendarList.removeAt(i);
      QDEBUG << "[i] Emit listOfCalendarsChanged";
      emit listOfCalendarsChanged(this->getListOfCalendars());
    }
    i++;
  }
}

void CalendarManager::handleLoadSetting(void) { loadSettings(); }

void CalendarManager::handleDisplayNameChanged(void) {
  emit listOfCalendarsChanged(this->getListOfCalendars());
}

void CalendarManager::handleErrorCalendarOccured() {
  emit errorCalendarOccured();
}