/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    ClientCalDAV.h
 * @brief   Classe comune per tutti i calendari.
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_CLIENTCALDAV_H
#define CALENDAR_TODO_LIST_CPP_CLIENTCALDAV_H

#include <QColor>
#include <QDate>
#include <QDebug>
#include <QDomDocument>
#include <QList>
#include <QNetworkAccessManager>
#include <QObject>
#include <QStateMachine>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include <QXmlStreamReader>
#include <QtNetwork>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

#include "../CalendarEvent/CalendarEvent.h"
#include "../OAuth2/OAuth.h"
#include "../Task/TaskElement.h"
#include "../Utils/DateUtils.h"

#define DEBUG_ 1
#if DEBUG_
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

class ClientCalDAV : public QObject {
  Q_OBJECT

public:
  typedef enum {
    E_STATE_IDLE, // Calendario in attesa della prossima sincronizzazione
    E_STATE_BUSY, // Calendario sincronizzato
    E_STATE_ERROR // Errore nel calendario
  } E_CalendarState;

  Q_ENUM(E_CalendarState)

  typedef enum {
    E_AUTH_UPWD, // Auth based on username and password
    E_AUTH_TOKEN // Auth based on OAuth2
  } E_CalendarAuth;

  Q_ENUM(E_CalendarAuth)

  /**
   *
   * @brief     for usage of Q_PROPERTY see upstream [Reading and Writing
   * Properties with the
   * MOS](https://doc.qt.io/qt-5/properties.html#reading-and-writing-properties-with-the-meta-object-system)
   */
  // Colore del CalendarClient per la GUI
  Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY colorChanged)

  // Sincronizzazione dello stato del CalendarClient
  Q_PROPERTY(
      E_CalendarState syncState READ getSyncState NOTIFY syncStateChanged)

  // URL della da cui vengono recuperati gli eventi del calendario
  // (percorso del file iCalendar, URL del server Caldav, ...)
  Q_PROPERTY(
      QUrl hostURL READ getHostURL WRITE setHostURL NOTIFY hostURLChanged)

  // Nome visualizzato dell'istanza Calendarclient;
  // potrebbe essere sovrascritto dalle informazioni del calendario ricevute
  Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY
                 displayNameChanged)

  // Richiesta di timeout in ms per l'accesso alle sorgenti del calendario
  // remoto (default: 32000ms)
  Q_PROPERTY(int requestTimeoutMS READ getRequestTimeoutMS WRITE
                 setRequestTimeoutMS NOTIFY requestTimeoutMSChanged)

  // Tipo di authenticazione eseguita con il server
  Q_PROPERTY(
      E_CalendarAuth clientAuth READ getClientAuth NOTIFY clientAuthChanged)

  // Anno visualizzato dal calendario
  Q_PROPERTY(int year READ getYear WRITE setYear NOTIFY yearChanged)

  // CTag dal calendario
  Q_PROPERTY(QString cTag READ getCTag NOTIFY cTagChanged)

  // Mese visualizzato dal calendario
  Q_PROPERTY(int month READ getMonth WRITE setMonth NOTIFY monthChanged)

  // Username per l'autenticazione base
  Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY
                 usernameChanged)

  // Username per l'autenticazione base
  Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY
                 passwordChanged)

  // Access Token ricevuto dal Server
  Q_PROPERTY(QString accessToken READ getAccessToken WRITE setAccessToken NOTIFY
                 accessTokenChanged)

  // File path dei secret del client in caso di autenticazione con OAuth
  Q_PROPERTY(QString filepath READ getFilePath WRITE setFilePath NOTIFY
                 filePathChanged)

  ClientCalDAV(const QString &username, const QString &password,
               const QString &hostURL, const QString &displayName,
               QObject *parent = nullptr);
  ClientCalDAV(const QString &filepath, const QString &hostURL,
               const QString &displayName, QObject *parent = nullptr);
  ~ClientCalDAV();

  /**
   * @brief     Restituisce un elenco di eventi che si verificano nella data
   *            inserita
   * @note      About Q_INVOKABLE se upstream
   * (Q_INVOKABLE)[https://doc.qt.io/qt-5/qobject.html#Q_INVOKABLE]
   */
  Q_INVOKABLE QList<QObject *> eventsForDate(const QDate &date);

  /**
   * @brief     Restituisce un elenco di eventi che si verificano nel range di
   * date inserite
   */
  Q_INVOKABLE QList<QObject *> eventsInRange(const QDate &startDate,
                                             const QDate &endDate);

  /**
   * @brief     Restituisce l'elenco completo degli eventi gestiti da
   *            CalendarClient
   */
  Q_INVOKABLE QList<QObject *> allEvents(void);

private:
  void handle(void);
  void submit(QString href, QTextStream *dataStream);

protected:
  void retrieveChangedEvent(void);
  void retrieveCTag(void);
  void setupStateMachine(void);

  // Tipo di auth verso il server
  E_CalendarAuth _auth;

  std::thread _t;
  std::mutex _m;
  std::condition_variable _cv;
  std::map<QString, QTextStream *> _eventsp;

  int _yearToBeRequested;
  int _monthToBeRequested;

  int _year;
  int _month;

  QString _username;
  QString _password;
  QString _accessToken;
  QString _filepath;
  bool _on;
  bool _firstSync;

  QString _color;
  E_CalendarState _state;
  QUrl _hostURL;
  QString _displayName;

  // Utilizzato per confrontare lo stato del calendario rispetto a quello che vi
  // e' sul server
  QString _cTag;

  bool _bRecoveredFromError;

  OAuth *_au;

  // Contengono le risposte delle richieste fatte ferso il server
  QNetworkAccessManager _uploadNetworkManager;
  QNetworkReply *_pUploadReply;
  QTimer _uploadRequestTimeoutTimer;

  QNetworkAccessManager _networkManager;
  QNetworkReply *_pReply;
  QTimer _requestTimeoutTimer;
  int _requestTimeoutMS;

  // Timer per risincronizzare automaticamente con la sorgente dell'evento
  QTimer _synchronizationTimer;

  /*// Flusso di prova per gestire il contenuto del file iCalendar
  QTextStream *_dataStream;*/

  // Elenco degli eventi gestiti CalendarEvent
  QList<CalendarEvent> _eventList;
  QList<TaskElement> _taskList;
  /**
   * @brief     Helper function per codificare le richieste di autorizzazione di
   *            rete
   */
  QString encodeBase64(QString string);

  /**
   * @brief     Helper function per decodificare le stringhe ricevute
   */
  QString ascii2utf8(QString str);

  /**
   * @brief   Parser per i campi del calendario da un iCalendar.
   */
  void parseVCALENDAR(QString href, QTextStream &dataStream);

  /**
   * @brief   Parser per i campi dei task da un iCalendar.
   */
  void parseVTODO(QString href, QTextStream &dataStream);

  /**
   * @brief   Parser per i campi VEVENT di un evento VCALENDAR.
   */
  CalendarEvent parseCalendarVEVENT(QString href, QTextStream &dataStream);

  /**
   * @brief   Parser per i campi VEVENT di un task VTODO.
   */
  void parseTodoVEVENT(QString href, QTextStream &dataStream);

  /**
   * @brief   Aggiunge un singolo evento a _eventList
   *          se l'evento si verifica entro un determinato Qdatetime.
   * @note    Evento singolo = evento senza valore RRULE.
   * @note    Gli eventi con un valore RRULE sono gestiti con parseVEVENT()
   */
  CalendarEvent *handleSingleEvent(CalendarEvent &evt,
                                   const QDateTime &startOfQuestionedDate,
                                   const QDateTime &endOfQuestionedDate);
  /**
   * @brief Returns true, if a questioned date is excluded by the event EXDATE
   * value.
   */
  bool isDateExcluded(const QString strExdates,
                      const QDate &questionedDate) const;

signals:

  void eventParsed(QList<CalendarEvent> eventList);
  void accessTokenError(QString displayName);
  void colorChanged(QString color);
  // emesso quando il calendario è entrato in un nuovo stato di sincronizzazione
  void syncStateChanged(E_CalendarState syncState);
  void hostURLChanged(QString hostURL);
  void displayNameChanged(QString displayName);

  // emesso quando l'evento in _eventList ha dei cambiamenti
  void eventsUpdated(void);
  void noEventsUpdated(void);

  // emesso quando vi è un errore
  void error(QString errorMsg);
  void errorOccured(void);

  // emesso quando lo stato di errore viene recuperato
  void recoverSignal(void);

  void requestTimeoutMSChanged(int);

  // emesso per forzare la sincronizzazione con la sorgente del calendario (file
  // o server)
  void forceSynchronization(void);

  void clientAuthChanged(E_CalendarAuth clientAuth);
  void yearChanged(const int &year);
  void monthChanged(const int &month);
  void cTagChanged(QString cTag);
  void accessTokenChanged(QString accessToken);
  void filePathChanged(QString filepath);
  void usernameChanged(QString username);
  void passwordChanged(QString password);

  void calendarHasNotChanged(void);
  void calendarCTagHasNotChanged(void);
  void calendarCTagChanged(void);
  void calendarUpdateRequired(void); // Emesso quando year/month sono cambiati
                                     // rispetto alla scorsa sincronizzazione
  void calendarCheckCTag(void);

public slots:
  void handleEventParsed(QList<CalendarEvent> eventList);
  QString getCTag(void) const;
  QString getColor(void) const;
  void setColor(const QString &color);
  void handleHTTPError(void);
  void handleRequestAuthentication(QNetworkReply *reply,
                                   QAuthenticator *authenticator);
  E_CalendarState getSyncState(void);

  virtual bool setHostURL(const QUrl hostURL);
  QString getHostURL(void) const;

  QString getDisplayName(void) const;
  virtual void setDisplayName(QString name);

  int getRequestTimeoutMS(void) const;
  void setRequestTimeoutMS(const int requestTimeoutMS);

  E_CalendarAuth getClientAuth(void) const;

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

  void notifyError(void);

  void setFilePath(QString filepath);
  QString getFilePath(void) const;

  /**
   * @brief Slot per avviare la sincronizzazione con la sorgente dell'evento
   */
  void startSynchronization(void);

  /**
   * @brief Slot per interrompere la sincronizzazione con la sorgente
   * dell'evento
   */
  void stopSynchronization(void);

  /**
   * @brief Slot per recuperare dallo stato di errore
   */
  void recover(void);

  /**
   * @brief Condivide un calendario.
   */
  void shareCalendar(QString email, QString comment);

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
  void saveTask(QString uid, QString filename, QString summary,
                QString description, QDateTime startDateTime,
                QDateTime endDateTime);

  /**
   * @brief Elimina uno specifico evento dal CalDAV Server.
   */
  void deleteEvent(QString href);

protected slots:

  void handleRequestCTagFinished(void);
  void handleRequestChangesEventFinished(void);

  void handleStateWaitingEntry(void);
  void handleStateWaitingExit(void);

  void handleStateCheckingChangesEntry(void);
  void handleStateCheckingChangesExit(void);

  void handleStateRequestingChangesEntry(void);
  void handleStateRequestingChangesExit(void);

  void handleStateProcessingChangesEntry(void);
  void handleStateProcessingChangesExit(void);

  void handleStateErrorEntry(void);

  void debug_handleTimerTimeout(void);

  void handleUploadHTTPError(void);
  void handleUploadFinished(void);
  void handleShareFinished(void);
};

#endif // CALENDAR_TODO_LIST_CPP_CLIENTCALDAV_H
