/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    CalendarClient_CalDAV.h
 * @brief   Classe calendario che ottiene oggetti CalendarEvent da un server
 *          Caldav.
 *
 *          La classe CalendarClient_CalDAV funziona con i server basati su
 * CalDAV. La proprietà URL può essere nello stile
 *          https://server.tld/owncloud/remote.php/dav/calendars/username/calendarname/
 *          Si consiglia vivamente di utilizzare HTTPS per proteggere i dati
 *          personali.
 *
 * @note    CalendarClient_CalDAV utilizza le proprietà anno e mese per limitare
 *          il elenco degli eventi per un determinato mese.
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_CALENDARCLIENT_CALDAV_H
#define CALENDAR_TODO_LIST_CPP_CALENDARCLIENT_CALDAV_H

#include <QDebug>
#include <QDomDocument>
#include <QObject>
#include <QStateMachine>
#include <QXmlStreamReader>
#include <iostream>

#include "../CalendarEvent/CalendarEvent.h"
#include "../OAuth2/OAuth.h"
#include "CalendarClient.h"

class CalendarClient_CalDAV : public CalendarClient {
  Q_OBJECT

public:
  typedef enum {
    E_AUTH_UPWD, // Auth based on username and password
    E_AUTH_TOKEN // Auth based on OAuth2
  } E_CalendarAuth;

  Q_ENUM(E_CalendarAuth)

  Q_PROPERTY(
      E_CalendarAuth clientAuth READ getClientAuth NOTIFY clientAuthChanged)
  Q_PROPERTY(int year READ getYear WRITE setYear NOTIFY yearChanged)
  Q_PROPERTY(int month READ getMonth WRITE setMonth NOTIFY monthChanged)
  Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY
                 usernameChanged)
  Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY
                 passwordChanged)
  Q_PROPERTY(QString accessToken READ getAccessToken WRITE setAccessToken NOTIFY
                 accessTokenChanged)

  CalendarClient_CalDAV(QObject *parent = nullptr);
  CalendarClient_CalDAV(const QString &username, const QString &password,
                        const QString &hostURL, const QString &displayName,
                        QObject *parent = nullptr);
  CalendarClient_CalDAV(const QString &filepath, const QString &hostURL,
                        const QString &displayName, QObject *parent = nullptr);
  ~CalendarClient_CalDAV();

protected:
  /**
   * @brief     Ottiene le informazioni del calendario dall CalDAV Server.
   */
  void getChangedEvent(void);
  void getChangedActivity(void);
  void getCTag(void);
  void setupStateMachine(void);

  int lastSyncYear;
  int lastSyncMonth;

  E_CalendarAuth _auth;

  int _yearToBeRequested;
  int _monthToBeRequested;
  int _year;
  int _month;
  QString _username;
  QString _password;
  QString _accessToken;

  QString _cTag;

  bool _bRecoveredFromError;

  OAuth *_au;

  QNetworkAccessManager _uploadNetworkManager;
  QNetworkReply *_pUploadReply;
  QTimer _uploadRequestTimeoutTimer;

signals:
  void clientAuthChanged(E_CalendarAuth clientAuth);
  void yearChanged(const int &year);
  void monthChanged(const int &month);
  void accessTokenChanged(QString username);
  void usernameChanged(QString username);
  void passwordChanged(QString password);

  void calendarHasNotChanged(void);
  void calendarUpdateRequired(void); // Emesso quando year/month sono cambiati
                                     // rispetto alla scorsa sincronizzazione

public slots:

  E_CalendarAuth getClientAuth(void);

  int getYear() const;
  void setYear(const int &year);

  int getMonth() const;
  void setMonth(const int &month);

  void setUsername(const QString username);
  QString getUsername(void) const;

  void setPassword(const QString password);
  QString getPassword(void) const;

  void setAccessToken(QString accessToken);
  QString getAccessToken(void) const;

  void startSynchronization(void);
  void stopSynchronization(void);
  void recover(void);

  /**
   * @brief Salva un evento nel CalDAV Server.
   *
   * Se l'uid e' vuoto, un nuovo evento viene creato.
   */
  void saveEvent(QString uid, QString filename, QString summary,
                 QString location, QString description, QString rrule,
                 QDateTime startDateTime, QDateTime endDateTime);

  /**
   * @brief Salva un task nel CalDAV Server.
   *
   * Se l'uid e' vuoto, un nuovo evento viene creato.
   */
  void saveActivity(QString uid, QString filename, QString summary,
                    QString description, QDateTime startDateTime,
                    QDateTime endDateTime);

  /**
   * @brief Elimina uno specifico evento dal CalDAV Server.
   */
  void deleteEvent(QString href);

protected slots:
  void handleHTTPError(void);

  void handleRequestCTagFinished(void);
  void handleRequestChangesEventFinished(void);
  void handleRequestChangesActivityFinished(void);

  void handleStateWaitingEntry(void);
  void handleStateWaitingExit(void);

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
