//
// Created by Marco Manco on 29/10/21.
//

/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_ICS.cpp 15 2017-01-26 16:59:37Z cypax $
* Revision of last commit:  $Rev: 15 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-26 17:59:37 +0100 (Do, 26 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_ICS.cpp
 * @author        Cypax (cypax.net)
 *
 * @brief         Implementation file of class CalendarClient_ICS.
 */
/************************************************************************//*@}*/

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QDebug>

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarClient_ICS.h"
#include "CalendarEvent.h"

/******************************************************************************/
/* Namespace                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/

#define DEBUG_CALENDARCLIENT_ICS 0
#if DEBUG_CALENDARCLIENT_ICS
#define QDEBUG qDebug()
#else
#define QDEBUG if (0) qDebug()
#endif

/***** End of: define Constants *****************************************//*@}*/

/******************************************************************************/
/* Constructors                                                               */
/**********************************//*!@addtogroup constrc Constructors *//*@{*/
CalendarClient_ICS::CalendarClient_ICS(QObject* parent) : CalendarClient(parent)
{
    m_CalendarType = E_CALENDAR_ICS;
    m_DataStream = NULL;
    m_DisplayName = "";
    m_bFileReadRequested = false;

    setupStateMachine();
}

/***** End of: constrc Constructors *************************************//*@}*/

/******************************************************************************/
/* Deconstructor                                                              */
/******************************//*!@addtogroup deconstrc Deconstructors *//*@{*/
CalendarClient_ICS::~CalendarClient_ICS()
{
    m_SynchronizationTimer.stop();
}

/***** End of: deconstrc Constructors ***********************************//*@}*/

/******************************************************************************/
/* Public functions                                                           */
/*****************************//*!@addtogroup pubfunct Public functions *//*@{*/
/***** End of: pubfunct Public functions ********************************//*@}*/

/******************************************************************************/
/* Protected functions                                                        */
/*************************//*!@addtogroup protfunct Protected functions *//*@{*/
/***** End of: protfunct Protected functions ****************************//*@}*/

/******************************************************************************/
/* Private functions                                                          */
/*************************//*!@addtogroup privfunct Private functions   *//*@{*/
/***** End of: privfunct Private functions ******************************//*@}*/

/******************************************************************************/
/* Public slots                                                               */
/*************************//*!@addtogroup pubslots Public slots         *//*@{*/


void CalendarClient_ICS::startSynchronization(void)
{
    QDEBUG << m_DisplayName << ": " << "!!!forcing synchronization!!!";
    m_bFileReadRequested = true;
    emit forceSynchronization();
}

void CalendarClient_ICS::stopSynchronization(void)
{
}

void CalendarClient_ICS::recover(void)
{
    QDEBUG << m_DisplayName << ": " << "trying to recover from EEROR state";
    m_bFileReadRequested = true;
    emit recoverSignal();
}

bool CalendarClient_ICS::setHostURL(const QUrl hostURL)
{
    bool retVal = CalendarClient::setHostURL(hostURL);
    m_bFileReadRequested |= retVal;
    return retVal;
}
