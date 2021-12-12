/**
 *
 * @author  Marco Manco
 * @date    10/12/21.
 * @file    GetEvents.cpp
 * @brief
 *
 */

#include "ClientCalDAV.h"

/**
 * @brief   Aggiunge un singolo evento a _eventList
 *          se l'evento si verifica entro un determinato QDateTime.
 * @note    Evento singolo = evento senza valore RRULE.
 * @note    Gli eventi con un valore RRULE sono gestiti con parseVEVENT()
 */
CalendarEvent *
ClientCalDAV::handleSingleEvent(CalendarEvent &evt,
                                const QDateTime &startOfQuestionedDate,
                                const QDateTime &endOfQuestionedDate) {
  CalendarEvent *event = nullptr;
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
    event = new CalendarEvent(this);
    event->copyFrom(evt);

    QDEBUG << "[i] (" << _displayName << ") Appended: " << event->name();
  }

  return event;
}

QList<QObject *> ClientCalDAV::eventsForDate(const QDate &date) {
  QList<QObject *> events;

  events.append(eventsInRange(date, date));

  return events;
}

/**
 * @brief     Controlla che l'evento non sn si svolga nel futuro.
 *            Se l'evento ha delle RRULE li recupera (FREQ obbligatoria).
 *            RRULE supportate: FREQ, COUNT, INTERVAL, UNTIL, BYDAY, WKST.
 */
QList<QObject *> ClientCalDAV::eventsInRange(const QDate &startDate,
                                             const QDate &endDate) {

  QList<QObject *> events;

  QDateTime startOfQuestionedDate(startDate, QTime(0, 0, 1));
  QDateTime endOfQuestionedDate(endDate, QTime(23, 59, 59));
  QDEBUG << "[i] (" << _displayName
         << ") startOfQuestionedDate: " << startOfQuestionedDate.toString()
         << " endOfQuestionedDate: " << endOfQuestionedDate;

  foreach (CalendarEvent evt, _eventList) {
    // skip se l'evento inizia nel futuro
    if (evt.getStartDateTime() > endOfQuestionedDate)
      continue;

    if (evt.getRRULE() == "") {
      // è un evento singolo
      QDEBUG << "[i] (" << _displayName << ") Is a single event";

      // skip se l'evento è terminato
      if (evt.getEndDateTime() < startOfQuestionedDate)
        continue;
      CalendarEvent *e =
          handleSingleEvent(evt, startOfQuestionedDate, endOfQuestionedDate);
      if (e != nullptr) {
        QDEBUG << "[i] (" << _displayName << ") Appended"
               << e->property("name");
        events.append(e);
      } else {
        QDEBUG << "[i] (" << _displayName << ") Event not in range";
      }
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
      QString strBYDAY = "";

      // dividi in nome e parametro (o parametri) delle regole
      foreach (QString ruleString, rruleList) {
        QStringList rruleElements =
            ruleString.trimmed().split("=", Qt::SkipEmptyParts);
        // il risultato deve avere 2 elementi
        if (rruleElements.length() != 2) {
          QDEBUG << "[i] (" << _displayName << ") "
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
          QDEBUG << "[i] (" << _displayName
                 << ") WKST: " << rruleElements.at(1);
        } else if (rruleElements.at(0).toUpper() == "BYDAY") {
          strBYDAY = rruleElements.at(1);
        } else {
          QDEBUG << "[i] (" << _displayName << ") "
                 << "WARNING: unsupported rrule element" << rruleElements.at(0);
        }
      }

      // la frequenza deve essere impostata,
      // tutte le altre regole sono facoltative
      if (strFREQ.isEmpty()) {
        QDEBUG << "[i] (" << _displayName << ") "
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
          QDEBUG << "[i] (" << _displayName << ") "
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
          QDEBUG << "[i] (" << _displayName << ") "
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
          QDEBUG << "[i] (" << _displayName << ") "
                 << "ERROR: could not interpret UNTIL date in"
                 << evt.getRRULE();
          continue;
        }
      }

      QDateTime testStartDateTime = evt.getStartDateTime();
      QDateTime testEndDateTime = evt.getStartDateTime();

      if (strFREQ == "WEEKLY") {
        QDEBUG << "[i] (" << _displayName << ")" << evt.name()
               << " repeats weekly" << evt.getRRULE();

        if (strBYDAY.isEmpty()) {
          // ripete ogni ennesima settimana dalla data di inizio

          int occurrences = 0;
          while (testStartDateTime <= endOfQuestionedDate) {
            occurrences++;

            // verifica se la data corrisponde all'intervallo delle date
            if ((testStartDateTime >= startOfQuestionedDate) &&
                (testStartDateTime <= endOfQuestionedDate)) {
              QDEBUG << "[i] (" << _displayName << ") "
                     << "found match on"
                     << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss");

              CalendarEvent *event = new CalendarEvent(this);
              event->copyFrom(evt);
              event->setIsCanceled(false);
              event->setStartDateTime(testStartDateTime);
              event->setEndDateTime(testEndDateTime);

              QDEBUG << "[i] (" << _displayName << ") Appended"
                     << event->name();
              events.append(event);
            }

            // abortire se si applica la regola COUNT
            if (!strCOUNT.isEmpty()) {
              if (occurrences >= iCount) {
                QDEBUG << "[i] (" << _displayName << ") Weekly, count reached"
                       << occurrences;
                break;
              }
            }

            // abortire se si applica la regola UNTIL
            if (!strUNTIL.isEmpty()) {
              if (testStartDateTime > dtUntil) {
                QDEBUG << "[i] (" << _displayName << ") Weekly, until reached";
                break;
              }
            }

            testStartDateTime = testStartDateTime.addDays(7 * iInterval);
            testEndDateTime = testEndDateTime.addDays(7 * iInterval);
          }
        } else {
          // ripete ogni ennesima settimana dalla data di inizio in determinati
          // giorni della settimana

          // ottenere una lista dei giorni feriali
          QStringList listOfWeekdays =
              strBYDAY.toUpper().trimmed().split(",", Qt::SkipEmptyParts);

          foreach (QString weekday, listOfWeekdays) {
            testStartDateTime = evt.getStartDateTime();
            testEndDateTime = evt.getEndDateTime();

            int iWeekday = DateUtils::getWeekdayIndexFromString(weekday);

            if (0 == iWeekday) {
              QDEBUG << "[i] (" << _displayName << ") "
                     << "ERROR: BYDAY" << weekday << "in rrule"
                     << evt.getRRULE() << "is invalid";
              continue;
            }

            // se la data di prova non inizia nel giorno feriale in questione,
            // passa alla successiva occorrenza nel giorno feriale
            if (testStartDateTime.date().dayOfWeek() != iWeekday) {
              int weekdayDiff = iWeekday - testStartDateTime.date().dayOfWeek();
              if (weekdayDiff < 0) {
                weekdayDiff += 7;
              }
              testStartDateTime = testStartDateTime.addDays(weekdayDiff);
              testEndDateTime = testEndDateTime.addDays(weekdayDiff);
            }

            int occurrences = 0;

            while (testStartDateTime <= endOfQuestionedDate) {
              occurrences++;

              // verifica se la data corrisponde all'intervallo delle date
              if ((testStartDateTime >= startOfQuestionedDate) &&
                  (testStartDateTime <= endOfQuestionedDate)) {
                QDEBUG << "[i] (" << _displayName << ") Found match on"
                       << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss")
                       << weekday;

                CalendarEvent *event = new CalendarEvent(this);
                event->copyFrom(evt);
                event->setStartDateTime(testStartDateTime);
                event->setEndDateTime(testEndDateTime);
                event->setIsCanceled(false);

                QDEBUG << "[i] (" << _displayName << ") Appended"
                       << event->name();
                events.append(event);
              }

              // abortire se si applica la regola COUNT
              if (!strCOUNT.isEmpty()) {
                if (occurrences >= iCount) {
                  QDEBUG << "[i] (" << _displayName
                         << ") Monthly, count reached" << occurrences;
                  break;
                }
              }

              // abortire se si applica la regola UNTIL
              if (!strUNTIL.isEmpty()) {
                if (testStartDateTime > dtUntil) {
                  QDEBUG << "[i] (" << _displayName
                         << ") Monthly, until reached";
                  break;
                }
              }

              testStartDateTime = testStartDateTime.addDays(7 * iInterval);
              testEndDateTime = testEndDateTime.addDays(7 * iInterval);
            }
          }
        }
      } else if (strFREQ == "MONTHLY") {
        if (strBYDAY.isEmpty()) {
          // ripete ogni ennesimo mese dalla data d'inizio

          int occurrences = 0;
          while (testStartDateTime <= endOfQuestionedDate) {
            occurrences++;

            // verifica se la data corrisponde al intervallo delle date
            if ((testStartDateTime >= startOfQuestionedDate) &&
                (testStartDateTime <= endOfQuestionedDate)) {
              QDEBUG << "[i] (" << _displayName << ") Found match on"
                     << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss");

              CalendarEvent *event = new CalendarEvent(this);
              event->copyFrom(evt);
              event->setStartDateTime(testStartDateTime);
              event->setEndDateTime(testEndDateTime);
              event->setIsCanceled(false);

              QDEBUG << "[i] (" << _displayName << ") Appended"
                     << event->name();
              events.append(event);
            }

            // abortire se si applica la regola COUNT
            if (!strCOUNT.isEmpty()) {
              if (occurrences >= iCount) {
                // QDEBUG << "[i] (" << _displayName << ") " << "monthly, count
                // reached"
                // << occurrences;
                break;
              }
            }

            // abortire se si applica la regola UNTIL
            if (!strUNTIL.isEmpty()) {
              if (testStartDateTime > dtUntil) {
                QDEBUG << "[i] (" << _displayName << ") Monthly, until reached";
                break;
              }
            }

            testStartDateTime = testStartDateTime.addDays(7 * iInterval);
            testEndDateTime = testEndDateTime.addDays(7 * iInterval);
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
            testStartDateTime = evt.getStartDateTime();
            testEndDateTime = evt.getEndDateTime();

            int iWeekday = DateUtils::getWeekdayIndexFromString(weekday);
            if (0 == iWeekday) {
              QDEBUG << "[i] (" << _displayName << ") "
                     << "ERROR: unsupported weekday" << weekday << "in RRULE"
                     << evt.getRRULE();
              break;
            }

            int iWeekdaySelector =
                DateUtils::getWeekdaySelectorFromString(weekday);
            if (DATEUTILS_INVALID_DAY == iWeekdaySelector) {
              QDEBUG << "[i] (" << _displayName << ") "
                     << "ERROR: unsupported weekday selector" << weekday
                     << "in RRULE" << evt.getRRULE();
              break;
            }

            if (0 == iWeekdaySelector) {
              // Il selettore nei giorni feriali non è impostato
              // -> si applica ogni occorrenza del giorno feriale in un mese

              // se l'evento non inizia nel giorno feriale in questione,
              // passa alla successiva occorrenza nei giorni feriali
              if (testStartDateTime.date().dayOfWeek() != iWeekday) {
                int weekdayDiff =
                    iWeekday - testStartDateTime.date().dayOfWeek();
                if (weekdayDiff < 0) {
                  weekdayDiff += 7;
                }
                testStartDateTime = testStartDateTime.addDays(weekdayDiff);
                testEndDateTime = testEndDateTime.addDays(weekdayDiff);
              }

              int occurrences = 0;

              while (testStartDateTime <= endOfQuestionedDate) {
                occurrences++;

                // 2: Se (testStartDateTime nel periodo in questione),
                // aggiungi l'evento alla lista degli eventi
                if ((testStartDateTime >= startOfQuestionedDate) &&
                    (testStartDateTime <= endOfQuestionedDate)) {
                  QDEBUG << "[i] (" << _displayName << ") "
                         << "found match on"
                         << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss")
                         << weekday;

                  CalendarEvent *event = new CalendarEvent(this);
                  event->copyFrom(evt);
                  event->setStartDateTime(testStartDateTime);
                  event->setEndDateTime(testEndDateTime);
                  event->setIsCanceled(false);

                  // QDEBUG << "[i] (" << _displayName << ") " << "appended" <<
                  // event->name();
                  events.append(event);
                }

                // abortire se si applica la regola COUNT
                if (!strCOUNT.isEmpty()) {
                  if (occurrences >= iCount) {
                    QDEBUG << "[i] (" << _displayName << ") "
                           << "monthly, count reached" << occurrences;
                    break;
                  }
                }

                // abortire se si applica la regola UNTIL
                if (!strUNTIL.isEmpty()) {
                  if (testStartDateTime > dtUntil) {
                    QDEBUG << "[i] (" << _displayName << ") "
                           << "monthly, until reached";
                    break;
                  }
                }

                int thisMonth = testStartDateTime.date().month();
                int thisYear = testStartDateTime.date().year();

                // 3: aggiungere una settimana a testStartDateTime
                testStartDateTime.setDate(testStartDateTime.date().addDays(7));

                // 4: se 3. fa uscire il mese in corso, allora
                if (thisMonth != testStartDateTime.date().month()) {
                  // 4.1: torna al primo giorno del mese in corso
                  testStartDateTime.setDate(QDate(thisYear, thisMonth, 1));

                  // 4.2: add iInterval times months
                  testStartDateTime = testStartDateTime.addMonths(iInterval);
                  testEndDateTime = testEndDateTime.addMonths(iInterval);

                  // 4.3: if this date is not the strBYDAY-weekday, then seek
                  // forward to the first strBYDAY-weekday
                  if (testStartDateTime.date().dayOfWeek() != iWeekday) {
                    int weekdayDiff =
                        iWeekday - testStartDateTime.date().dayOfWeek();
                    if (weekdayDiff < 0) {
                      weekdayDiff += 7;
                    }
                    testStartDateTime = testStartDateTime.addDays(weekdayDiff);
                    testEndDateTime = testEndDateTime.addDays(weekdayDiff);
                  }
                }

              } // END: while (testStartDateTime <= endOfQuestionedDate)

            } else {
              // weekday selector is set -> only specific weekdays in a month do
              // apply

              int occurrences = 0;

              while (testStartDateTime <= endOfQuestionedDate) {
                QDate weekDayDate = DateUtils::getWeekdayOfMonth(
                    iWeekday, iWeekdaySelector, testStartDateTime.date());
                if (weekDayDate.isValid()) {
                  occurrences++;

                  testStartDateTime.setDate(weekDayDate);

                  // check if date matches questioned date range
                  if ((testStartDateTime >= startOfQuestionedDate) &&
                      (testStartDateTime <= endOfQuestionedDate)) {
                    QDEBUG << "[i] (" << _displayName << ") "
                           << "found match" << occurrences << "on"
                           << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss")
                           << weekday;

                    CalendarEvent *event = new CalendarEvent(this);
                    event->copyFrom(evt);
                    event->setStartDateTime(testStartDateTime);
                    event->setEndDateTime(testEndDateTime);
                    event->setIsCanceled(false);

                    QDEBUG << "[i] (" << _displayName << ") "
                           << "appended" << event->name();
                    events.append(event);
                  }

                }
                // abort if testStartDateTime exceeds end of questioned date
                else if (testStartDateTime > endOfQuestionedDate) {
                  QDEBUG << "[i] (" << _displayName << ") "
                         << "end of questioned range reached"
                         << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss");
                  break;
                }

                // abort if COUNT rule applies
                if (!strCOUNT.isEmpty()) {
                  if (occurrences >= iCount) {
                    QDEBUG << "[i] (" << _displayName << ") "
                           << "monthly, count reached" << occurrences;
                    break;
                  }
                }

                // abort if UNTIL rule applies
                if (!strUNTIL.isEmpty()) {
                  if (testStartDateTime > dtUntil) {
                    QDEBUG << "[i] (" << _displayName << ") "
                           << "monthly, until reached"
                           << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss");
                    break;
                  }
                }

                testStartDateTime = testStartDateTime.addMonths(iInterval);
                testEndDateTime = testEndDateTime.addMonths(iInterval);
                testStartDateTime.setDate(
                    QDate(testStartDateTime.date().year(),
                          testStartDateTime.date().month(), 1));
                // if the event doesn't start on the questioned weekday, jump
                // forward to the next weekday occurrence
                if (testStartDateTime.date().dayOfWeek() != iWeekday) {
                  int weekdayDiff =
                      iWeekday - testStartDateTime.date().dayOfWeek();
                  if (weekdayDiff < 0) {
                    weekdayDiff += 7;
                  }
                  testStartDateTime = testStartDateTime.addDays(weekdayDiff);
                  testEndDateTime = testEndDateTime.addDays(weekdayDiff);
                }
              }
            }

          } // END: foreach weekday
        }

      } else if (strFREQ == "YEARLY") {
        int occurrences = 1; // start date counts as first occurrence

        while (testStartDateTime <= endOfQuestionedDate) {
          occurrences++;

          // check if date matches questioned date range
          if ((testStartDateTime >= startOfQuestionedDate) &&
              (testStartDateTime <= endOfQuestionedDate)) {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "found yearly match" << occurrences << "on"
                   << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss");

            CalendarEvent *event = new CalendarEvent(this);
            event->copyFrom(evt);
            event->setStartDateTime(testStartDateTime);
            event->setEndDateTime(testEndDateTime);
            event->setIsCanceled(false);

            QDEBUG << "[i] (" << _displayName << ") "
                   << "appended" << event->name();
            events.append(event);
          }

          testStartDateTime = testStartDateTime.addYears(iInterval);
          testEndDateTime = testEndDateTime.addYears(iInterval);

          // abort if testStartDateTime exceeds end of questioned date
          if (testStartDateTime > endOfQuestionedDate) {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "end of questioned range reached"
                   << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss");
            break;
          }

          // abort if COUNT rule applies
          if (!strCOUNT.isEmpty()) {
            if (occurrences >= iCount) {
              QDEBUG << "[i] (" << _displayName << ") "
                     << "yearly, count reached" << occurrences;
              break;
            }
          }

          // abort if UNTIL rule applies
          if (!strUNTIL.isEmpty()) {
            if (testStartDateTime > dtUntil) {
              QDEBUG << "[i] (" << _displayName << ") "
                     << "yearly, until reached"
                     << testStartDateTime.toString("yyyy-MM-dd HH:mm:ss");
              break;
            }
          }
        }

        continue;
      } else {
        QDEBUG << "[i] (" << _displayName << ") "
               << "WARNING: unsupported event frequency" << strFREQ;
      }

    } // END: else (is repeated event)
  }

  return events;
}

QList<QObject *> ClientCalDAV::allEvents(void) {

  QList<QObject *> events;

  foreach (CalendarEvent evt, _eventList) {
    CalendarEvent *event = new CalendarEvent(this);
    event->copyFrom(evt);
    events.append(event);
  }

  return events;
}