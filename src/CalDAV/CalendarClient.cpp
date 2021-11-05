/**
 * @author      Created by Marco Manco on 16/10/21.
 *
 * @brief       File di implementazione della classe Calendarclient.
 */

#include <QColor>
#include <QDebug>

#include "../DateUtils.h"
#include "CalendarClient.h"
#include "CalendarEvent.h"

/**
 * Constants
 */

#define DEBUG_CALENDARCLIENT 0
#if DEBUG_CALENDARCLIENT
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

CalendarClient::CalendarClient(QObject *parent) : QObject(parent) {
  m_pReply = NULL;
  m_DataStream = NULL;
  m_HostURL = "";
  m_DisplayName = "";

  m_RequestTimeoutMS = 2000; // 2ms -> 32ms
  m_RequestTimeoutTimer.setSingleShot(true);

  // Il timer è impostato su singolo scatto,
  // quindi non è necessario interromperlo
  // nel caso in cui la sincronizzazione non sia riuscita
  m_SynchronizationTimer.setSingleShot(true);
  m_SynchronizationTimer.setInterval(60000);

  // Creazione di un colore generato in modo randomico
  m_Color = QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF).name();
}

CalendarClient::~CalendarClient() {
  m_EventList.clear();
  m_SynchronizationTimer.stop();
}

/**
 * Public function
 */

QList<QObject *> CalendarClient::eventsForDate(const QDate &date) {
  QList<QObject *> events;

  QDateTime startOfQuestionedDate(date, QTime(0, 0, 1));
  QDateTime endOfQuestionedDate(date, QTime(23, 59, 59));

  foreach (CalendarEvent evt, m_EventList) {
    // skip se l'evento inizia nel futuro
    if (evt.getStartDateTime() > endOfQuestionedDate)
      continue;

    if (evt.getRRULE() == "") {
      // è un evento singolo

      // skip se l'evento è terminato
      if (evt.getEndDateTime() < startOfQuestionedDate)
        continue;

      events.append(
          handleSingleEvent(evt, startOfQuestionedDate, endOfQuestionedDate));
    }

    else {
      // è un evento che si ripete

      // retrieve an array of rules
      QStringList rruleList =
          evt.getRRULE().trimmed().split(";", Qt::SkipEmptyParts);

      QString strFREQ = "";
      QString strINTERVAL = "";
      QString strCOUNT = "";
      QString strUNTIL = "";
      QString strWKST = "";
      QString strBYDAY = "";
      QString strBYMONTHDAY = "";
      QString strBYMONTH = "";
      QString strBYYEAR = "";
      QString strBYWEEKNO = "";
      QString strBYSETPOS = "";

      // dividi in nome e parametro (o parametri) delle regole
      foreach (QString ruleString, rruleList) {
        QStringList rruleElements =
            ruleString.trimmed().split("=", Qt::SkipEmptyParts);
        // il risultato deve avere 2 elementi
        if (rruleElements.length() != 2) {
          QDEBUG << m_DisplayName << ": "
                 << "ERROR: invalid rule element count"
                 << rruleElements.length() << "in rule" << ruleString;
          continue;
        }
        if (rruleElements.at(0).toUpper() == "FREQ") {
          strFREQ = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "INTERVAL") {
          strINTERVAL = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "COUNT") {
          strCOUNT = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "UNTIL") {
          strUNTIL = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "WKST") {
          strWKST = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "BYDAY") {
          strBYDAY = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "BYMONTHDAY") {
          strBYMONTHDAY = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "BYMONTH") {
          strBYMONTH = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "BYYEAR") {
          strBYYEAR = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "BYWEEKNO") {
          strBYWEEKNO = rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "BYSETPOS") {
          strBYSETPOS = rruleElements.at(1);
        } else {
          QDEBUG << m_DisplayName << ": "
                 << "WARNING: unsupported rrule element" << rruleElements.at(0);
        }
      }

      // fla frequenza deve essere impostata,
      // tutte le altre regole sono facoltative
      if (strFREQ.isEmpty()) {
        QDEBUG << m_DisplayName << ": "
               << "ERROR: FREQ rule not set in RRULE" << evt.getRRULE();
        continue;
      }

      // recupera INTERVAL come intero se impostato
      int iInterval = 1;
      if (!strINTERVAL.isEmpty()) {
        bool convResult = false;
        int convValue = strINTERVAL.toInt(&convResult, 10);
        if (convResult) {
          iInterval = convValue;
        } else {
          QDEBUG << m_DisplayName << ": "
                 << "ERROR: invalid INTERVAL value in" << evt.getRRULE();
          continue;
        }
      }

      // recupera COUNT come intero se impostato
      int iCount = -1;
      if (!strCOUNT.isEmpty()) {
        bool convResult = false;
        int convValue = strCOUNT.toInt(&convResult, 10);
        if (convResult)
          iCount = convValue;
        else {
          QDEBUG << m_DisplayName << ": "
                 << "ERROR: invalid COUNT value in" << evt.getRRULE();
          continue;
        }
      }

      // recuperare UNTIL come QDateTime se disponibile
      QDateTime dtUntil = QDateTime(); // Creazione di un QDateTime invalido
      if (!strUNTIL.isEmpty()) {
        dtUntil = QDateTime::fromString(strUNTIL, Qt::ISODate);
        if (!dtUntil.isValid()) {
          dtUntil = QDateTime::fromString(strUNTIL, "yyyyMMddTHHmmssZ");
        }
        if (!dtUntil.isValid()) {
          dtUntil = QDateTime::fromString(strUNTIL, "yyyyMMddTHHmmss");
        }
        if (!dtUntil.isValid()) {
          dtUntil = QDateTime::fromString(strUNTIL, "yyyyMMddHHmmss");
        }
        if (!dtUntil.isValid()) {
          QDEBUG << m_DisplayName << ": "
                 << "ERROR: could not interpret UNTIL date in"
                 << evt.getRRULE();
          continue;
        }
      }

      QDateTime testDateTime = evt.getStartDateTime();

      if (strFREQ == "WEEKLY") {
        // QDEBUG << m_DisplayName << ": " << evt.name() << " repeats weekly" <<
        // evt.getRRULE();

        if (strBYDAY.isEmpty()) {
          // ripete ogni ennesima settimana dalla data di inizio

          int occurrences = 0;
          while (testDateTime <= endOfQuestionedDate) {
            // verificare se l'evento non è escluso da EXDATEs
            bool bOccurrenceIsCanceled =
                isDateExcluded(evt.getExdates(), testDateTime.date());
            if (!bOccurrenceIsCanceled) {
              occurrences++;
            }

            // verifica se la data corrisponde all'intervallo delle date
            if ((testDateTime >= startOfQuestionedDate) &&
                (testDateTime <= endOfQuestionedDate)) {
              // QDEBUG << m_DisplayName << ": " << "found match on" <<
              // testDateTime.toString("yyyy-MM-dd HH:mm:ss");

              CalendarEvent *event = new CalendarEvent(this);
              event->copyFrom(evt);
              event->setIsCanceled(bOccurrenceIsCanceled);
              // event->setStartDateTime(testDateTime);

              // QDEBUG << m_DisplayName << ": " << "appended" << event->name();
              events.append(event);
            }

            // abortire se si applica la regola COUNT
            if (!strCOUNT.isEmpty()) {
              if (occurrences >= iCount) {
                QDEBUG << m_DisplayName << ": "
                       << "weekly, count reached" << occurrences;
                break;
              }
            }

            // abortire se si applica la regola UNTIL
            if (!strUNTIL.isEmpty()) {
              if (testDateTime > dtUntil) {
                QDEBUG << m_DisplayName << ": "
                       << "weekly, until reached";
                break;
              }
            }

            testDateTime = testDateTime.addDays(7 * iInterval);
          }
        } else {
          // ripete ogni ennesima settimana dalla data di inizio in determinati
          // giorni della settimana

          // ottenere una lista dei giorni feriali
          QStringList listOfWeekdays =
              strBYDAY.toUpper().trimmed().split(",", Qt::SkipEmptyParts);

          foreach (QString weekday, listOfWeekdays) {
            testDateTime = evt.getStartDateTime();

            int iWeekday = DateUtils::getWeekdayIndexFromString(weekday);

            if (0 == iWeekday) {
              QDEBUG << m_DisplayName << ": "
                     << "ERROR: BYDAY" << weekday << "in rrule"
                     << evt.getRRULE() << "is invalid";
              continue;
            }

            // se la data di prova non inizia nel giorno feriale in questione,
            // passa alla successiva occorrenza nel giorno feriale
            if (testDateTime.date().dayOfWeek() != iWeekday) {
              int weekdayDiff = iWeekday - testDateTime.date().dayOfWeek();
              if (weekdayDiff < 0) {
                weekdayDiff += 7;
              }
              testDateTime = testDateTime.addDays(weekdayDiff);
            }

            int occurrences = 0;

            while (testDateTime <= endOfQuestionedDate) {
              // verificare se l'evento non è escluso da EXDATEs
              bool bOccurrenceIsCanceled =
                  isDateExcluded(evt.getExdates(), testDateTime.date());
              if (!bOccurrenceIsCanceled) {
                occurrences++;
              }

              // verifica se la data corrisponde all'intervallo delle date
              if ((testDateTime >= startOfQuestionedDate) &&
                  (testDateTime <= endOfQuestionedDate)) {
                // QDEBUG << m_DisplayName << ": " << "found match on" <<
                // testDateTime.toString("yyyy-MM-dd HH:mm:ss") << weekday;

                CalendarEvent *event = new CalendarEvent(this);
                event->copyFrom(evt);
                // event->setStartDateTime(testDateTime);
                event->setIsCanceled(bOccurrenceIsCanceled);

                // QDEBUG << m_DisplayName << ": " << "appended" <<
                // event->name();
                events.append(event);
              }

              // abortire se si applica la regola COUNT
              if (!strCOUNT.isEmpty()) {
                if (occurrences >= iCount) {
                  QDEBUG << m_DisplayName << ": "
                         << "monthly, count reached" << occurrences;
                  break;
                }
              }

              // abortire se si applica la regola UNTIL
              if (!strUNTIL.isEmpty()) {
                if (testDateTime > dtUntil) {
                  QDEBUG << m_DisplayName << ": "
                         << "monthly, until reached";
                  break;
                }
              }

              testDateTime = testDateTime.addDays(7 * iInterval);
            }
          }
        }
      } else if (strFREQ == "MONTHLY") {
        if (strBYDAY.isEmpty()) {
          // ripete ogni ennesimo mese dalla data di inizio

          int occurrences = 0;
          while (testDateTime <= endOfQuestionedDate) {
            // verificare se l'evento non è escluso da EXDATEs
            bool bOccurrenceIsCanceled =
                isDateExcluded(evt.getExdates(), testDateTime.date());
            if (!bOccurrenceIsCanceled)
              occurrences++;

            // verifica se la data corrisponde all'intervallo delle date
            if ((testDateTime >= startOfQuestionedDate) &&
                (testDateTime <= endOfQuestionedDate)) {
              // QDEBUG << m_DisplayName << ": " << "found match on" <<
              // testDateTime.toString("yyyy-MM-dd HH:mm:ss");

              CalendarEvent *event = new CalendarEvent(this);
              event->copyFrom(evt);
              // event->setStartDateTime(testDateTime);
              event->setIsCanceled(bOccurrenceIsCanceled);

              QDEBUG << m_DisplayName << ": "
                     << "appended" << event->name();
              events.append(event);
            }

            // abortire se si applica la regola COUNT
            if (!strCOUNT.isEmpty()) {
              if (occurrences >= iCount) {
                // QDEBUG << m_DisplayName << ": " << "monthly, count reached"
                // << occurrences;
                break;
              }
            }

            // abortire se si applica la regola UNTIL
            if (!strUNTIL.isEmpty()) {
              if (testDateTime > dtUntil) {
                QDEBUG << m_DisplayName << ": "
                       << "monthly, until reached";
                break;
              }
            }

            testDateTime = testDateTime.addDays(7 * iInterval);
          }

        }
        // quando strBYDAY non è vuoto
        else {
          // ripete ogni ennesimo mese dalla data di inizio in determinati
          // giorni della settimana

          // ottenere una lista dei giorni feriali
          QStringList listOfWeekdays =
              strBYDAY.toUpper().trimmed().split(",", Qt::SkipEmptyParts);

          foreach (QString weekday, listOfWeekdays) {
            testDateTime = evt.getStartDateTime();

            int iWeekday = DateUtils::getWeekdayIndexFromString(weekday);
            if (0 == iWeekday) {
              QDEBUG << m_DisplayName << ": "
                     << "ERROR: unsupported weekday" << weekday << "in RRULE"
                     << evt.getRRULE();
              break;
            }

            int iWeekdaySelector =
                DateUtils::getWeekdaySelectorFromString(weekday);
            if (DATEUTILS_INVALID_DAY == iWeekdaySelector) {
              QDEBUG << m_DisplayName << ": "
                     << "ERROR: unsupported weekday selector" << weekday
                     << "in RRULE" << evt.getRRULE();
              break;
            }

            if (0 == iWeekdaySelector) {
              // Il selettore nei giorni feriali non è impostato
              // -> si applica ogni occorrenza del giorno feriale in un mese

              // se l'evento non inizia nel giorno feriale in questione,
              // passa alla successiva occorrenza nei giorni feriali
              if (testDateTime.date().dayOfWeek() != iWeekday) {
                int weekdayDiff = iWeekday - testDateTime.date().dayOfWeek();
                if (weekdayDiff < 0) {
                  weekdayDiff += 7;
                }
                testDateTime = testDateTime.addDays(weekdayDiff);
              }

              int occurrences = 0;

              while (testDateTime <= endOfQuestionedDate) {
                // verificare se l'evento non è escluso da EXDATEs
                bool bOccurrenceIsCanceled =
                    isDateExcluded(evt.getExdates(), testDateTime.date());
                if (!bOccurrenceIsCanceled)
                  occurrences++;

                // 2: Se (testDateTime nel periodo in questione),
                // aggiungi l'evento alla lista degli eventi
                if ((testDateTime >= startOfQuestionedDate) &&
                    (testDateTime <= endOfQuestionedDate)) {
                  // QDEBUG << m_DisplayName << ": " << "found match on" <<
                  // testDateTime.toString("yyyy-MM-dd HH:mm:ss") << weekday;

                  CalendarEvent *event = new CalendarEvent(this);
                  event->copyFrom(evt);
                  // event->setStartDateTime(testDateTime);
                  event->setIsCanceled(bOccurrenceIsCanceled);

                  // QDEBUG << m_DisplayName << ": " << "appended" <<
                  // event->name();
                  events.append(event);
                }

                // abortire se si applica la regola COUNT
                if (!strCOUNT.isEmpty()) {
                  if (occurrences >= iCount) {
                    QDEBUG << m_DisplayName << ": "
                           << "monthly, count reached" << occurrences;
                    break;
                  }
                }

                // abortire se si applica la regola UNTIL
                if (!strUNTIL.isEmpty()) {
                  if (testDateTime > dtUntil) {
                    QDEBUG << m_DisplayName << ": "
                           << "monthly, until reached";
                    break;
                  }
                }

                int thisMonth = testDateTime.date().month();
                int thisYear = testDateTime.date().year();

                // 3: aggiungere una settimana a testDateTime
                testDateTime.setDate(testDateTime.date().addDays(7));

                // 4: se 3. fa uscire il mese in corso, allora
                if (thisMonth != testDateTime.date().month()) {
                  // 4.1: torna al primo giorno del mese in corso
                  testDateTime.setDate(QDate(thisYear, thisMonth, 1));

                  // 4.2: add iInterval times months
                  testDateTime = testDateTime.addMonths(iInterval);

                  // 4.3: if this date is not the strBYDAY-weekday, then seek
                  // forward to the first strBYDAY-weekday
                  if (testDateTime.date().dayOfWeek() != iWeekday) {
                    int weekdayDiff =
                        iWeekday - testDateTime.date().dayOfWeek();
                    if (weekdayDiff < 0) {
                      weekdayDiff += 7;
                    }
                    testDateTime = testDateTime.addDays(weekdayDiff);
                  }
                }

              } // END: while (testDateTime <= endOfQuestionedDate)

            } else {
              // weekday selector is set -> only specific weekdays in a month do
              // apply

              int occurrences = 0;

              while (testDateTime <= endOfQuestionedDate) {
                QDate weekDayDate = DateUtils::getWeekdayOfMonth(
                    iWeekday, iWeekdaySelector, testDateTime.date());
                if (weekDayDate.isValid()) {
                  // check if event is not excluded by EXDATEs
                  bool bOccurrenceIsCanceled =
                      isDateExcluded(evt.getExdates(), testDateTime.date());
                  if (!bOccurrenceIsCanceled) {
                    occurrences++;
                  }

                  testDateTime.setDate(weekDayDate);

                  // check if date matches questioned date range
                  if ((testDateTime >= startOfQuestionedDate) &&
                      (testDateTime <= endOfQuestionedDate)) {
                    // QDEBUG << m_DisplayName << ": " << "found match" <<
                    // occurrences << "on" << testDateTime.toString("yyyy-MM-dd
                    // HH:mm:ss") << weekday;

                    CalendarEvent *event = new CalendarEvent(this);
                    event->copyFrom(evt);
                    // event->setStartDateTime(testDateTime);
                    event->setIsCanceled(bOccurrenceIsCanceled);

                    // QDEBUG << m_DisplayName << ": " << "appended" <<
                    // event->name();
                    events.append(event);
                  }

                }
                // abort if testDateTime exceeds end of questioned date
                else if (testDateTime > endOfQuestionedDate) {
                  // QDEBUG << m_DisplayName << ": " << "end of questioned range
                  // reached" << testDateTime.toString("yyyy-MM-dd HH:mm:ss");
                  break;
                }

                // abort if COUNT rule applies
                if (!strCOUNT.isEmpty()) {
                  if (occurrences >= iCount) {
                    // QDEBUG << m_DisplayName << ": " << "monthly, count
                    // reached" << occurrences;
                    break;
                  }
                }

                // abort if UNTIL rule applies
                if (!strUNTIL.isEmpty()) {
                  if (testDateTime > dtUntil) {
                    QDEBUG << m_DisplayName << ": "
                           << "monthly, until reached"
                           << testDateTime.toString("yyyy-MM-dd HH:mm:ss");
                    break;
                  }
                }

                testDateTime = testDateTime.addMonths(iInterval);
                testDateTime.setDate(QDate(testDateTime.date().year(),
                                           testDateTime.date().month(), 1));
                // if the event doesn't start on the questioned weekday, jump
                // forward to the next weekday occurrence
                if (testDateTime.date().dayOfWeek() != iWeekday) {
                  int weekdayDiff = iWeekday - testDateTime.date().dayOfWeek();
                  if (weekdayDiff < 0) {
                    weekdayDiff += 7;
                  }
                  testDateTime = testDateTime.addDays(weekdayDiff);
                }
              }
            }

          } // END: foreach weekday
        }

      } else if (strFREQ == "YEARLY") {
        int occurrences = 1; // start date counts as first occurrence

        while (testDateTime <= endOfQuestionedDate) {
          // check if event is not excluded by EXDATEs
          bool bOccurrenceIsCanceled =
              isDateExcluded(evt.getExdates(), testDateTime.date());
          if (!bOccurrenceIsCanceled) {
            occurrences++;
          }

          // check if date matches questioned date range
          if ((testDateTime >= startOfQuestionedDate) &&
              (testDateTime <= endOfQuestionedDate)) {
            // QDEBUG << m_DisplayName << ": " << "found yearly match" <<
            // occurrences << "on" << testDateTime.toString("yyyy-MM-dd
            // HH:mm:ss");

            CalendarEvent *event = new CalendarEvent(this);
            event->copyFrom(evt);
            // event->setStartDateTime(testDateTime);
            event->setIsCanceled(bOccurrenceIsCanceled);

            // QDEBUG << m_DisplayName << ": " << "appended" << event->name();
            events.append(event);
          }

          testDateTime = testDateTime.addYears(iInterval);

          // abort if testDateTime exceeds end of questioned date
          if (testDateTime > endOfQuestionedDate) {
            // QDEBUG << m_DisplayName << ": " << "end of questioned range
            // reached" << testDateTime.toString("yyyy-MM-dd HH:mm:ss");
            break;
          }

          // abort if COUNT rule applies
          if (!strCOUNT.isEmpty()) {
            if (occurrences >= iCount) {
              // QDEBUG << m_DisplayName << ": " << "yearly, count reached" <<
              // occurrences;
              break;
            }
          }

          // abort if UNTIL rule applies
          if (!strUNTIL.isEmpty()) {
            if (testDateTime > dtUntil) {
              // QDEBUG << m_DisplayName << ": " << "yearly, until reached" <<
              // testDateTime.toString("yyyy-MM-dd HH:mm:ss");
              break;
            }
          }
        }

        continue;
      } else {
        QDEBUG << m_DisplayName << ": "
               << "WARNING: unsupported event frequency" << strFREQ;
      }

    } // END: else (is repeated event)
  }

  return events;
}

QList<QObject *> CalendarClient::allEvents(void) {
  QList<QObject *> events;

  foreach (CalendarEvent evt, m_EventList) {
    CalendarEvent *event = new CalendarEvent(this);
    event->copyFrom(evt);
    events.append(event);
  }

  return events;
}

int CalendarClient::getRequestTimeoutMS(void) const {
  return m_RequestTimeoutMS;
}

void CalendarClient::setRequestTimeoutMS(const int requestTimeoutMS) {
  if (requestTimeoutMS > 0) {
    m_RequestTimeoutMS = requestTimeoutMS;
    emit requestTimeoutMSChanged(m_RequestTimeoutMS);
  }
}

/***** End of: pubfunct Public functions ********************************/ /*@}*/

/******************************************************************************/
/* Protected functions                                                        */
/*************************/
/*!@addtogroup protfunct Protected functions */ /*@{*/

QList<QObject *>
CalendarClient::handleSingleEvent(CalendarEvent &evt,
                                  const QDateTime &startOfQuestionedDate,
                                  const QDateTime &endOfQuestionedDate) {
  QList<QObject *> events;

  // events must not end at 00:00:00
  if (0 == evt.getEndDateTime().time().msecsSinceStartOfDay()) {
    // removing one second from endDateTime
    QDateTime newEndDateTime = evt.getEndDateTime().addSecs(-1);
    evt.setEndDateTime(newEndDateTime);
  }

  // check if event occurs in questioned range
  if (((evt.getStartDateTime() >= startOfQuestionedDate) &&
       (evt.getStartDateTime() <=
        endOfQuestionedDate)) // event starts in questioned range
      || ((evt.getEndDateTime() >= startOfQuestionedDate) &&
          (evt.getEndDateTime() <=
           endOfQuestionedDate)) // event ends in questioned range
      || ((evt.getStartDateTime() <= startOfQuestionedDate) &&
          (evt.getEndDateTime() >=
           endOfQuestionedDate)) // event overlaps questioned range
  ) {
    CalendarEvent *event = new CalendarEvent(this);
    event->copyFrom(evt);

    // QDEBUG << m_DisplayName << ": " << "appended" << event->name();
    events.append(event);
  }

  return events;
}

QString CalendarClient::encodeBase64(QString string) {
  QByteArray ba;
  ba.append(string.toUtf8());
  return ba.toBase64();
}

QString CalendarClient::ascii2utf8(QString str) {
  return QString::fromUtf8(str.toLocal8Bit().data());
}

void CalendarClient::parseCALENDAR(QString href) {
  QString line = m_DataStream->readLine();

  while (false == line.isNull()) {
    if (false != line.contains("BEGIN:VEVENT")) {
      parseVEVENT(href);
    }
    line = m_DataStream->readLine();
  }
}

void CalendarClient::parseVEVENT(QString href) {
  CalendarEvent event(this);
  event.setColor(m_Color);
  event.setCalendarName(m_DisplayName);
  event.setCalendarPointer(this);
  event.setHREF(href);
  QString line;
  QDateTime utcTime;
  while (
      !(line = m_DataStream->readLine()).contains(QByteArray("END:VEVENT"))) {
    // QDEBUG << m_DisplayName << ": " << line;

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
        QDEBUG << m_DisplayName << ": "
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
        QDEBUG << m_DisplayName << ": "
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
    m_EventList.append(event);
  }
}

bool CalendarClient::isDateExcluded(const QString strExdates,
                                    const QDate &questionedDate) const {
  QStringList strlstExdates = strExdates.split(",", Qt::SkipEmptyParts);
  bool bRet = false;

  if (strlstExdates.isEmpty()) {
    return false;
  }

  foreach (const QString &strExdate, strlstExdates) {
    QDateTime excludeDate;
    excludeDate = QDateTime::fromString(strExdate, "yyyyMMdd'T'hhmmss'Z'");
    if (!excludeDate.isValid())
      excludeDate = QDateTime::fromString(strExdate, "yyyyMMdd'T'hhmmss");
    if (!excludeDate.isValid())
      excludeDate = QDateTime::fromString(strExdate, "yyyyMMddhhmmss");
    if (!excludeDate.isValid())
      excludeDate = QDateTime::fromString(strExdate, "yyyyMMdd");
    if (!excludeDate.isValid()) {
      QDEBUG << m_DisplayName << ": "
             << "could not parse EXDATE" << strExdate;
    } else {
      excludeDate.setDate(excludeDate.toLocalTime().date());
      if (excludeDate.date() == questionedDate) {
        // event occurence is excluded
        // QDEBUG << m_DisplayName << ": " << "event is excluded by EXDATE" <<
        // strExdate;
        bRet = true;
        break;
      }
    }
  }

  return bRet;
}

/***** End of: protfunct Protected functions ****************************/ /*@}*/

/******************************************************************************/
/* Private functions                                                          */
/*************************/
/*!@addtogroup privfunct Private functions   */ /*@{*/
/***** End of: privfunct Private functions ******************************/ /*@}*/

/******************************************************************************/
/* Public slots                                                               */
/*************************/
/*!@addtogroup pubslots Public slots         */ /*@{*/

CalendarClient::E_CalendarState CalendarClient::getSyncState(void) {
  return m_State;
}

QString CalendarClient::getColor(void) const { return m_Color; }

void CalendarClient::setColor(const QString &color) {
  if (color != m_Color) {
    m_Color = color;
    for (int i = 0; i < m_EventList.count(); i++) {
      CalendarEvent *pEvent = &(m_EventList[i]);
      pEvent->setColor(m_Color);
    }
    emit colorChanged(m_Color);
  }
}

CalendarClient::E_CalendarType CalendarClient::getCalendarType(void) {
  return m_CalendarType;
}

bool CalendarClient::setHostURL(const QUrl hostURL) {
  bool bRet = false;

  if (false != hostURL.isValid()) {
    m_HostURL = hostURL;
    bRet = true;
    emit hostURLChanged(m_HostURL.toString());
  }

  return bRet;
}

QString CalendarClient::getHostURL(void) const { return m_HostURL.toString(); }

QString CalendarClient::getDisplayName(void) const { return m_DisplayName; }

void CalendarClient::setDisplayName(QString name) {
  m_DisplayName = name;
  emit displayNameChanged(m_DisplayName);
}
