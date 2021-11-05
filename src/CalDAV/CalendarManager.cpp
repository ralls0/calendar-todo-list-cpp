/**
 * @author      Created by Marco Manco on 16/10/21.
 * @file        CalendarManager.h
 *
 * @brief       Implementation file of class CalendarManager.
 *
 */

#include <QColor>
#include <QDate>
#include <QDebug>
#include <QSettings>

#include "CalendarClient_CalDAV.h"
#include "CalendarManager.h"
// #include "CalendarClient_ICS.h"
#include "../SimpleCrypt.h"

#define PWD_CRYPT Q_UINT64_C(0x3A3CF524EC443FB1)

#define DEBUG_CALENDARMANAGER 0
#if DEBUG_CALENDARMANAGER
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

CalendarManager::CalendarManager(QString iniFileName, QObject *parent)
    : QObject(parent) {
  m_Date = QDate::currentDate();
  m_IniFileName = iniFileName;
  loadSettings();
}

CalendarManager::~CalendarManager() {
  saveSettings();
  clearCalendarList();
}

QList<QObject *> CalendarManager::eventsForDate(const QDate &date) {
  QList<QObject *> events;

  foreach (CalendarClient *pCalendarClient, m_CalendarList) {
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
  foreach (QObject *pListItem, m_CalendarList) { pListItem->deleteLater(); }
  m_CalendarList.clear();
}

void CalendarManager::saveSettings(void) {
  SimpleCrypt crypto(PWD_CRYPT);
  QSettings settings(m_IniFileName, QSettings::IniFormat);
  settings.clear();

  for (int index = 0; index < m_CalendarList.count(); index++) {
    settings.beginGroup("Calendar" + QString::number(index));
    settings.setValue("DisplayName",
                      m_CalendarList.at(index)->getDisplayName());
    settings.setValue("Type", m_CalendarList.at(index)->getCalendarType());
    settings.setValue("URL", m_CalendarList.at(index)->getHostURL());
    settings.setValue("Color", m_CalendarList.at(index)->getColor());
    if (CalendarClient::E_CALENDAR_CALDAV ==
        m_CalendarList.at(index)->getCalendarType()) {
      const CalendarClient_CalDAV *pCalDAVClient =
          (const CalendarClient_CalDAV *)m_CalendarList[index];
      settings.setValue("Username", pCalDAVClient->getUsername());
      settings.setValue("Password",
                        crypto.encryptToString(pCalDAVClient->getPassword()));
    }
    settings.endGroup();
  }
}

void CalendarManager::loadSettings(void) {
  SimpleCrypt crypto(PWD_CRYPT);
  QSettings settings(m_IniFileName, QSettings::IniFormat);

  bool bAbort = false;
  int index = 0;

  clearCalendarList();

  while (false == bAbort) {

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
      switch ((CalendarClient::E_CalendarType)type) {
        //                case CalendarClient::E_CALENDAR_ICS:
        //                {
        //                    addICS_Calendar(color, displayName, QUrl(url));
        //                    break;
        //                } FIXME

      case CalendarClient::E_CALENDAR_CALDAV: {
        key = group;
        key.append("Username");
        QString username = settings.value(key, "").toString();

        key = group;
        key.append("Password");
        QString password = settings.value(key, "").toString();
        if (!password.isEmpty()) {
          password = crypto.decryptToString(password);
        }

        addCalDAV_Calendar(color, displayName, QUrl(url), username, password);

        break;
      }

      default: {
        QDEBUG << "ERROR:" << type
               << "is no valid CalendarClient::E_CalendarType";

        break;
      }
      }

    } else {
      bAbort = true;
    }
    index++;
  }
}

QDate CalendarManager::getDate() const { return m_Date; }

void CalendarManager::setDate(const QDate &newDate) {
  if (m_Date != newDate) {
    QDEBUG << "Date changed to " << newDate.toString();
    m_Date = newDate;
    foreach (CalendarClient *pListItem, m_CalendarList) {
      if (CalendarClient::E_CALENDAR_CALDAV == pListItem->getCalendarType()) {
        CalendarClient_CalDAV *pCalendarClient_CalDAV =
            (CalendarClient_CalDAV *)pListItem;
        pCalendarClient_CalDAV->setYear(newDate.year());
        pCalendarClient_CalDAV->setMonth(newDate.month());
      } else if (CalendarClient::E_CALENDAR_ICS ==
                 pListItem->getCalendarType()) {
        pListItem->startSynchronization();
      }
    }
    emit dateChanged();
  }
}

QList<QObject *> CalendarManager::getListOfCalendars(void) {
  QList<QObject *> returnList;
  foreach (CalendarClient *pListItem, m_CalendarList) {
    returnList.append(pListItem);
  }
  return returnList;
}

QList<QObject *> CalendarManager::getListOfEvents(void) {
  QList<QObject *> returnList;
  foreach (CalendarClient *pListItem, m_CalendarList) {
    returnList.append(pListItem->allEvents());
  }

  return returnList;
}

void CalendarManager::addCalDAV_Calendar(QString color, QString calendarName,
                                         QUrl url, QString username,
                                         QString password) {
  CalendarClient_CalDAV *pCalendar = new CalendarClient_CalDAV(this);
  pCalendar->setColor(color);
  pCalendar->setHostURL(url);
  pCalendar->setUsername(username);
  pCalendar->setPassword(password);
  pCalendar->setDisplayName(calendarName);

  m_CalendarList.append(pCalendar);

  connect(pCalendar, SIGNAL(eventsUpdated()), this, SLOT(handleEventUpdate()));

  emit listOfCalendarsChanged();
}

// void CalendarManager::addICS_Calendar(QString color, QString calendarName,
// QUrl url)
//{
//     CalendarClient_ICS* pCalendar = new CalendarClient_ICS(this);
//
//     pCalendar->setColor(color);
//     pCalendar->setDisplayName(calendarName);
//     pCalendar->setHostURL(url);
//
//     m_CalendarList.append(pCalendar);
//
//     connect(pCalendar, SIGNAL(eventsUpdated()), this,
//     SLOT(handleEventUpdate()));
//
//     pCalendar->startSynchronization();
//     emit listOfCalendarsChanged();
// } FIXME add when you introduce support for ICS

CalendarClient *CalendarManager::getListItemAt(int index) {
  if ((index < 0) || (index >= m_CalendarList.count())) {
    QDEBUG << "ERROR: index" << index << "is invalid in m_CalendarList("
           << m_CalendarList.count() << ")";
    return NULL;
  }

  return m_CalendarList[index];
}

bool CalendarManager::removeListItemAt(int index) {
  if ((-1 == index) && (m_CalendarList.count() > 0)) {
    m_CalendarList.removeLast();
    emit listOfCalendarsChanged();
    return true;
  }

  if ((index >= 0) && (index < m_CalendarList.count())) {
    m_CalendarList.removeAt(index);
    emit listOfCalendarsChanged();
    return true;
  }

  QDEBUG << "ERROR: index" << index << "is invalid in m_CalendarList("
         << m_CalendarList.count() << ")";
  return false;
}

int CalendarManager::getCalendarCount(void) const {
  return m_CalendarList.count();
}

void CalendarManager::handleEventUpdate(void) {
  emit eventsUpdated();
  emit listOfEventsChanged();
}
