/**
 * @author      Created by Marco Manco on 16/10/21.
 *
 *  @brief      CalDAV based calendar client.
 */

#ifndef PROVACLIENTDAV_CALENDARCLIENT_CALDAV_H
#define PROVACLIENTDAV_CALENDARCLIENT_CALDAV_H


/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QObject>

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarClient.h"
#include "CalendarEvent.h"

/******************************************************************************/
/* Namespace                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/
/***** End of: define Constants *****************************************//*@}*/

/******************************************************************************/
/* Class                                                                      */
/******************************************//*!@addtogroup class Classes*//*@{*/
/**
* @brief Calendar class which obtains CalendarEvent objects from a CalDAV server.
*
* The CalendarClient_CalDAV class works with owncloud, nextcloud and possibly
* with other calDAv based servers.
* The URL property can be in the style
* https://server.tld/owncloud/remote.php/dav/calendars/username/calendarname/
* It is highly recommended to use HTTPS to protect your personal data.
*
* @note CalendarClient_CalDAV uses the year and month properties to limit the
* list of events to a specific month.
**/
class CalendarClient_CalDAV : public CalendarClient
{
Q_OBJECT

/******************************************************************************/
/* Types (typedef)                                                            */
/******************************************//*!@addtogroup typedef Types*//*@{*/
public:

/***** End of: typedef Types ********************************************//*@}*/

/******************************************************************************/
/* Properties (Q_PROPERTY)                                                    */
/**********************************//*!@addtogroup Q_PROPERTY Properties*//*@{*/
    Q_PROPERTY(int year READ getYear WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(int month READ getMonth WRITE setMonth NOTIFY monthChanged)
    Q_PROPERTY(QString  username  READ  getUsername   WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString  password  READ  getPassword   WRITE setPassword NOTIFY passwordChanged)


/***** End of: Q_PROPERTY Properties ************************************//*@}*/

/******************************************************************************/
/* Constructors                                                               */
/******************************************************************************/
public:
    CalendarClient_CalDAV(QObject* parent = NULL);

/******************************************************************************/
/* Destructor                                                                 */
/******************************************************************************/
public:
    ~CalendarClient_CalDAV();

/******************************************************************************/
/* Public function prototypes                                                 */
/******************************************************************************/
public:

/******************************************************************************/
/* Protected function prototypes                                                */
/******************************************************************************/
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

/******************************************************************************/
/* Private function prototypes                                                */
/******************************************************************************/
private:

/******************************************************************************/
/* Signals                                                                    */
/******************************************************************************/
signals:
    void yearChanged(const int& year);
    void monthChanged(const int& month);
    void usernameChanged(QString username);
    void passwordChanged(QString password);

    void syncTokenChanged(void);        // emitted when the sync token has changed
    void syncTokenHasNotChanged(void);

    void calendarHasNotChanged(void);
    void calendarUpdateRequired(void); // emitted when the sync token has changed or the year/month since the last synchronization

/******************************************************************************/
/* Public slots                                                               */
/******************************************************************************/
public slots:

    int getYear() const;
    void setYear(const int& year);

    int getMonth() const;
    void setMonth(const int& month);

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
    void saveEvent(QString uid,
                   QString filename,
                   QString summary,
                   QString location,
                   QString description,
                   QString rrule,
                   QString exdate,
                   QDateTime startDateTime,
                   QDateTime endDateTime);

    /**
     * @brief Deletes a specific event from the calDAV server.
     */
    void deleteEvent(QString href);

/******************************************************************************/
/* Protected slots                                                            */
/******************************************************************************/
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


/******************************************************************************/
/* Private slots                                                               */
/******************************************************************************/
private slots:

/******************************************************************************/
/* Protected attributes                                                         */
/******************************************************************************/
protected:
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
    QNetworkReply* m_pUploadReply;
    QTimer m_UploadRequestTimeoutTimer;

/******************************************************************************/
/* Private attributes                                                         */
/******************************************************************************/
private:

};

#endif //PROVACLIENTDAV_CALENDARCLIENT_CALDAV_H
