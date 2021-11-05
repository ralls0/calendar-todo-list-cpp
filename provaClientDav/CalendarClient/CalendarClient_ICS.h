//
// Created by Marco Manco on 29/10/21.
//

#ifndef PROVACLIENTDAV_CALENDARCLIENT_ICS_H
#define PROVACLIENTDAV_CALENDARCLIENT_ICS_H
/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_ICS.h 17 2017-01-26 17:25:48Z cypax $
* Revision of last commit:  $Rev: 17 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-26 18:25:48 +0100 (Do, 26 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_ICS.h
 * @author        Cypax (cypax.net)
 *
 * @brief         File based calendar client.
 */
/************************************************************************//*@}*/
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
* @brief Calendar class which obtains CalendarEvent objects from a iCalendar file.
*
* The URL property can specify network or local files (also with relative paths)
* in the style
* https://server.tld/folder/filename.ics
* or
* file:///directory/subdirectory/filename.ics
* or
* file:///../../subdirectory/filename.ics
**/
class CalendarClient_ICS : public CalendarClient
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
/***** End of: Q_PROPERTY Properties ************************************//*@}*/

/******************************************************************************/
/* Constructors                                                               */
/******************************************************************************/
public:
    CalendarClient_ICS(QObject* parent = NULL);

/******************************************************************************/
/* Destructor                                                                 */
/******************************************************************************/
public:
    ~CalendarClient_ICS();

/******************************************************************************/
/* Public function prototypes                                                 */
/******************************************************************************/
public:

/******************************************************************************/
/* Protected function prototypes                                                */
/******************************************************************************/
protected:
    void setupStateMachine(void);
    void sendRequest(void);

/******************************************************************************/
/* Private function prototypes                                                */
/******************************************************************************/
private:

/******************************************************************************/
/* Signals                                                                    */
/******************************************************************************/
signals:

    // emitted, when reading the file has finished successful
    void readingDone(void);

/******************************************************************************/
/* Public slots                                                               */
/******************************************************************************/
public slots:

    void startSynchronization(void);
    void stopSynchronization(void);
    void recover(void);

    bool setHostURL(const QUrl hostURL);

/******************************************************************************/
/* Protected slots                                                            */
/******************************************************************************/
protected slots:

    void handleStateWaitingEntry(void);
    void handleStateWaitingExit(void);
    void handleStateReadingEntry(void);
    void handleStateReadingExit(void);
    void handleStateErrorEntry(void);
    void debug_handleTimerTimeout(void);

    void handleRequestFinished(void);
    void handleHTTPError(void);


/******************************************************************************/
/* Private slots                                                               */
/******************************************************************************/
private slots:

/******************************************************************************/
/* Protected attributes                                                         */
/******************************************************************************/
protected:

    bool m_bFileReadRequested;

/******************************************************************************/
/* Private attributes                                                         */
/******************************************************************************/
private:

};

#endif //PROVACLIENTDAV_CALENDARCLIENT_ICS_H
