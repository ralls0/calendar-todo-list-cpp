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
QList<QObject *>
ClientCalDAV::handleSingleEvent(CalendarEvent &evt,
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

    QDEBUG << "[i] (" << _displayName << ") Appended: " << event->name();
    events.append(event);
  }

  return events;
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

  foreach (CalendarEvent evt, _eventList) {
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

      QDateTime testDateTime = evt.getStartDateTime();

      if (strFREQ == "WEEKLY") {
         QDEBUG << "[i] (" << _displayName << ") " << evt.name() << " repeats weekly" << evt.getRRULE();

        if (strBYDAY.isEmpty()) {
          // ripete ogni ennesima settimana dalla data di inizio

          int occurrences = 0;
          while (testDateTime <= endOfQuestionedDate) {
              occurrences++;

            // verifica se la data corrisponde all'intervallo delle date
            if ((testDateTime >= startOfQuestionedDate) &&
                (testDateTime <= endOfQuestionedDate)) {
               QDEBUG << "[i] (" << _displayName << ") " << "found match on"
               << testDateTime.toString("yyyy-MM-dd HH:mm:ss");

              CalendarEvent *event = new CalendarEvent(this);
              event->copyFrom(evt);
              event->setIsCanceled(false);
              // event->setStartDateTime(testDateTime);

               QDEBUG << "[i] (" << _displayName << ") " << "appended" <<
               event->name();
              events.append(event);
            }

            // abortire se si applica la regola COUNT
            if (!strCOUNT.isEmpty()) {
              if (occurrences >= iCount) {
                QDEBUG << "[i] (" << _displayName << ") "
                       << "weekly, count reached" << occurrences;
                break;
              }
            }

            // abortire se si applica la regola UNTIL
            if (!strUNTIL.isEmpty()) {
              if (testDateTime > dtUntil) {
                QDEBUG << "[i] (" << _displayName << ") "
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
              QDEBUG << "[i] (" << _displayName << ") "
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
              occurrences++;

              // verifica se la data corrisponde all'intervallo delle date
              if ((testDateTime >= startOfQuestionedDate) &&
                  (testDateTime <= endOfQuestionedDate)) {
                // QDEBUG << "[i] (" << _displayName << ") " << "found match on"
                // << testDateTime.toString("yyyy-MM-dd HH:mm:ss") << weekday;

                CalendarEvent *event = new CalendarEvent(this);
                event->copyFrom(evt);
                // event->setStartDateTime(testDateTime);
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
                if (testDateTime > dtUntil) {
                  QDEBUG << "[i] (" << _displayName << ") "
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
          // ripete ogni ennesimo mese dalla data d'inizio

          int occurrences = 0;
          while (testDateTime <= endOfQuestionedDate) {
            occurrences++;

            // verifica se la data corrisponde al intervallo delle date
            if ((testDateTime >= startOfQuestionedDate) &&
                (testDateTime <= endOfQuestionedDate)) {
              // QDEBUG << "[i] (" << _displayName << ") " << "found match on"
              // << testDateTime.toString("yyyy-MM-dd HH:mm:ss");

              CalendarEvent *event = new CalendarEvent(this);
              event->copyFrom(evt);
              // event->setStartDateTime(testDateTime);
              event->setIsCanceled(false);

              QDEBUG << "[i] (" << _displayName << ") "
                     << "appended" << event->name();
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
              if (testDateTime > dtUntil) {
                QDEBUG << "[i] (" << _displayName << ") "
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
              if (testDateTime.date().dayOfWeek() != iWeekday) {
                int weekdayDiff = iWeekday - testDateTime.date().dayOfWeek();
                if (weekdayDiff < 0) {
                  weekdayDiff += 7;
                }
                testDateTime = testDateTime.addDays(weekdayDiff);
              }

              int occurrences = 0;

              while (testDateTime <= endOfQuestionedDate) {
                occurrences++;

                // 2: Se (testDateTime nel periodo in questione),
                // aggiungi l'evento alla lista degli eventi
                if ((testDateTime >= startOfQuestionedDate) &&
                    (testDateTime <= endOfQuestionedDate)) {
                   QDEBUG << "[i] (" << _displayName << ") " << "found match on" << testDateTime.toString("yyyy-MM-dd HH:mm:ss") <<
                   weekday;

                  CalendarEvent *event = new CalendarEvent(this);
                  event->copyFrom(evt);
                  // event->setStartDateTime(testDateTime);
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
                  if (testDateTime > dtUntil) {
                    QDEBUG << "[i] (" << _displayName << ") "
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
                  occurrences++;

                  testDateTime.setDate(weekDayDate);

                  // check if date matches questioned date range
                  if ((testDateTime >= startOfQuestionedDate) &&
                      (testDateTime <= endOfQuestionedDate)) {
                     QDEBUG << "[i] (" << _displayName << ") " << "found match" << occurrences << "on" <<
                     testDateTime.toString("yyyy-MM-dd HH:mm:ss") << weekday;

                    CalendarEvent *event = new CalendarEvent(this);
                    event->copyFrom(evt);
                    // event->setStartDateTime(testDateTime);
                    event->setIsCanceled(false);

                     QDEBUG << "[i] (" << _displayName << ") " << "appended"
                     << event->name();
                    events.append(event);
                  }

                }
                // abort if testDateTime exceeds end of questioned date
                else if (testDateTime > endOfQuestionedDate) {
                   QDEBUG << "[i] (" << _displayName << ") " << "end of questioned range reached" <<
                   testDateTime.toString("yyyy-MM-dd HH:mm:ss");
                  break;
                }

                // abort if COUNT rule applies
                if (!strCOUNT.isEmpty()) {
                  if (occurrences >= iCount) {
                     QDEBUG << "[i] (" << _displayName << ") " << "monthly, count reached" << occurrences;
                    break;
                  }
                }

                // abort if UNTIL rule applies
                if (!strUNTIL.isEmpty()) {
                  if (testDateTime > dtUntil) {
                    QDEBUG << "[i] (" << _displayName << ") "
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
          occurrences++;

          // check if date matches questioned date range
          if ((testDateTime >= startOfQuestionedDate) &&
              (testDateTime <= endOfQuestionedDate)) {
             QDEBUG << "[i] (" << _displayName << ") " << "found yearly match"
             << occurrences << "on" << testDateTime.toString("yyyy-MM-dd HH:mm:ss");

            CalendarEvent *event = new CalendarEvent(this);
            event->copyFrom(evt);
            // event->setStartDateTime(testDateTime);
            event->setIsCanceled(false);

             QDEBUG << "[i] (" << _displayName << ") " << "appended" <<
             event->name();
            events.append(event);
          }

          testDateTime = testDateTime.addYears(iInterval);

          // abort if testDateTime exceeds end of questioned date
          if (testDateTime > endOfQuestionedDate) {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "end of questioned range reached"
                   << testDateTime.toString("yyyy-MM-dd HH:mm:ss");
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
            if (testDateTime > dtUntil) {
              QDEBUG << "[i] (" << _displayName << ") "
                     << "yearly, until reached"
                     << testDateTime.toString("yyyy-MM-dd HH:mm:ss");
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