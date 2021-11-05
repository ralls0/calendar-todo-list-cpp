//
// Created by Marco Manco on 29/10/21.
//

#ifndef PROVACLIENTDAV_CALENDARMANAGER_H
#define PROVACLIENTDAV_CALENDARMANAGER_H

/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarManager.h 14 2017-01-26 16:52:40Z cypax $
* Revision of last commit:  $Rev: 14 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-26 17:52:40 +0100 (Do, 26 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarManager.h
 * @author        Cypax (cypax.net)
 *
 * @brief         Manages a list of CalendarClients.
 */
/************************************************************************//*@}*/


/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QObject>
#include <QUrl>
#include <QDate>

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarClient.h"

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
* @brief A CalendarManager handles a list of CalendarClient objects and acts as
* a central instance to obtain events for specific dates.
*
* Usage: The CalendarManager maintains a list of ICS file based or calDAV server
* based CalendarClients. The calendar settings are loaded at creation from an
* INI file which is specified as constructor parameter.
* At destruction, the CalendarManager stores the calendar settings to this file.
* Additional calendars can be added by calling addCalDAV_Calendar() or
* addICS_Calendar().
* The listOfCalendars property provides a list of all maintained calendars.
* To remove an calendar from this list call removeListItemAt() with the
* respective index.
* To receive a list of all events from all calendars use the eventList property.
* However, if only events for a specific date are of interest, first set the
* corresponding month and year via the date property and call the eventsForDate()
* method with the questioned date as parameter.
**/
class CalendarManager : public QObject
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
    Q_PROPERTY(QDate date READ getDate WRITE setDate NOTIFY dateChanged)

    Q_PROPERTY(QList<QObject*> listOfCalendars
                       READ   getListOfCalendars
                       NOTIFY listOfCalendarsChanged)

    Q_PROPERTY(QList<QObject*> eventList
                       READ   getListOfEvents
                       NOTIFY listOfEventsChanged)

/***** End of: Q_PROPERTY Properties ************************************//*@}*/

/******************************************************************************/
/* Constructors                                                               */
/******************************************************************************/
public:
    CalendarManager(QString iniFileName, QObject* parent = NULL);

/******************************************************************************/
/* Destructor                                                                 */
/******************************************************************************/
public:
    ~CalendarManager();

/******************************************************************************/
/* Public function prototypes                                                 */
/******************************************************************************/
public:

    /**
     * @brief Returns a list of events which occur on a specific date
     *
     * @note In case month or year have changed, it is necessary to set the date
     * property first
     * @see setDate()
     */
    Q_INVOKABLE QList<QObject*> eventsForDate(const QDate& date);

/******************************************************************************/
/* Protected function prototypes                                                */
/******************************************************************************/
protected:

    /**
     * @brief Erases the list of CalendarClients (m_CalendarList)
     */
    void clearCalendarList(void);

    /**
     * @brief Saves calendar settings to INI file
     *
     * Saved information for each CalendarClient instance: type, display name,
     * color, URL and in case of calDAV calendars also the username and password.
     * Passwords are save encrypted (using SimpleCrypt) so at least you won't
     * have any files containing your owncloud password in plaintext on your disk.
     */
    void saveSettings(void);

    /**
     * @brief Loads calendar settings from INI file
     *
     * @note m_CalendarList will be erased and rebuild during this procedure!
     *
     * @see saveSettings()
     */
    void loadSettings(void);

/******************************************************************************/
/* Private function prototypes                                                */
/******************************************************************************/
private:

/******************************************************************************/
/* Signals                                                                    */
/******************************************************************************/
signals:
    void dateChanged(void);
    void listOfCalendarsChanged(void); // emitted when m_CalendarList has changed

    void listOfEventsChanged(void); // emitted during handleEventUpdate()
    void eventsUpdated(void);       // emitted during handleEventUpdate()

/******************************************************************************/
/* Public slots                                                               */
/******************************************************************************/
public slots:

    QDate getDate() const;
    void setDate(const QDate& newDate);

    QList<QObject*> getListOfCalendars(void);
    QList<QObject*> getListOfEvents(void);

    void addCalDAV_Calendar(QString color, QString calendarName, QUrl url, QString username, QString password);
    void addICS_Calendar(QString color, QString calendarName, QUrl url);

    /**
     * @brief Returns a pointer to a specific calendar instance in the list of calendars
     */
    CalendarClient* getListItemAt(int index);

    /**
     * @brief Removes a CalendarClient instance from list of calendars
     *
     * @retval Returns false if the requested index does not exist
     * @note If called without parameter, the last calendar in the list will be removed
     */
    bool removeListItemAt(int index = -1);

    int getCalendarCount(void) const;

/******************************************************************************/
/* Protected slots                                                            */
/******************************************************************************/
protected slots:

    /**
     * @brief Slot to be called when a CalendarClient instance reports an event update
     */
    void handleEventUpdate(void);

/******************************************************************************/
/* Private slots                                                               */
/******************************************************************************/
private slots:

/******************************************************************************/
/* Protected attributes                                                         */
/******************************************************************************/
protected:
    QList<CalendarClient*> m_CalendarList; // list of CalDAV or ICS calendars
    QString m_IniFileName;
    QDate m_Date;

/******************************************************************************/
/* Private attributes                                                         */
/******************************************************************************/
private:
};

#endif //PROVACLIENTDAV_CALENDARMANAGER_H
