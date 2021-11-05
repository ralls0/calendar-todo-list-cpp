//
// Created by Marco Manco on 05/11/21.
//

#ifndef CALENDAR_TODO_LIST_CPP_CALENDARCLIENT_CALDAV_H
#define CALENDAR_TODO_LIST_CPP_CALENDARCLIENT_CALDAV_H

#include <QObject>

#include "CalendarClient.h"
#include "CalendarEvent.h"

/**
 * @brief Classe calendario che ottiene oggetti CalendarEvent da un server
 *Caldav.
 *
 * La classe CalendarClient_CalDAV funziona con i server basati su CalDAV.
 * La proprietà URL può essere nello stile
 * https://server.tld/owncloud/remote.php/dav/calendars/username/calendarname/
 * Si consiglia vivamente di utilizzare HTTPS per proteggere i dati personali.
 *
 * @note Calendarclient_caldav utilizza le proprietà anno e mese per limitare il
 * elenco degli eventi per un determinato mese.
 **/
class CalendarClient_CalDAV : public CalendarClient {
  Q_OBJECT

public:
  Q_PROPERTY(int year READ getYear WRITE setYear NOTIFY yearChanged)
  Q_PROPERTY(int month READ getMonth WRITE setMonth NOTIFY monthChanged)
  Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY
                 usernameChanged)
  Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY
                 passwordChanged)

  CalendarClient_CalDAV(QObject *parent = NULL);
  ~CalendarClient_CalDAV();

protected:
  /**
   * @brief Retrieves a sync token from the calDAv server.
   *
   * If the token has changed, the changes shall be retrieved by calling
   * sendRequestChanges().
   */
  void sendRequestSyncToken(void);

  /**
   * @brief Obtains calendar information from the calDAV server.
   */
  void sendRequestChanges(void);
  void setupStateMachine(void);

  int lastSyncYear;
  int lastSyncMonth;

  int m_YearToBeRequested;
  int m_MonthToBeRequested;
  int m_Year;
  int m_Month;
  QString m_Username;
  QString m_Password;

  QString m_cTag;
  QString m_syncToken;

  bool m_bRecoveredFromError;

  QNetworkAccessManager m_UploadNetworkManager;
  QNetworkReply *m_pUploadReply;
  QTimer m_UploadRequestTimeoutTimer;

signals:
  void yearChanged(const int &year);
  void monthChanged(const int &month);
  void usernameChanged(QString username);
  void passwordChanged(QString password);

  void syncTokenChanged(void); // emitted when the sync token has changed
  void syncTokenHasNotChanged(void);

  void calendarHasNotChanged(void);
  void
  calendarUpdateRequired(void); // emitted when the sync token has changed or
                                // the year/month since the last synchronization

public slots:

  int getYear() const;
  void setYear(const int &year);

  int getMonth() const;
  void setMonth(const int &month);

  void setUsername(const QString username);
  QString getUsername(void) const;

  void setPassword(const QString password);
  QString getPassword(void) const;

  void startSynchronization(void);
  void stopSynchronization(void);
  void recover(void);

  /**
   * @brief Saves a event to the calDAV server.
   *
   * If the uid parameter is empty, a new event will be created.
   */
  void saveEvent(QString uid, QString filename, QString summary,
                 QString location, QString description, QString rrule,
                 QString exdate, QDateTime startDateTime,
                 QDateTime endDateTime);

  /**
   * @brief Deletes a specific event from the calDAV server.
   */
  void deleteEvent(QString href);

protected slots:
  void handleHTTPError(void);

  void handleRequestSyncTokenFinished(void);
  void handleRequestChangesFinished(void);

  void handleStateWaitingEntry(void);
  void handleStateWaitingExit(void);

  void handleStateRequestingSyncTokenEntry(void);
  void handleStateRequestingSyncTokenExit(void);

  void handleStateRequestingChangesEntry(void);
  void handleStateRequestingChangesExit(void);

  void handleStateProcessingChangesEntry(void);
  void handleStateProcessingChangesExit(void);

  void handleStateErrorEntry(void);

  void debug_handleTimerTimeout(void);

  void handleUploadHTTPError(void);
  void handleUploadFinished(void);
};

#endif // CALENDAR_TODO_LIST_CPP_CALENDARCLIENT_CALDAV_H
