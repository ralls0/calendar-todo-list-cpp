//
// Created by Marco Manco on 29/10/21.
//

/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_ICS_StateMachine.cpp 11 2017-01-16 20:27:13Z cypax $
* Revision of last commit:  $Rev: 11 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-16 21:27:13 +0100 (Mo, 16 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_ICS_StateMachine.cpp
 * @author        Cypax (cypax.net)
 *
 * @brief         State machine implementation of class CalendarClient_ICS.
 */
/************************************************************************//*@}*/

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QDebug>
#include <QStateMachine>

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarClient_ICS.h"

/******************************************************************************/
/* Namespace                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/

#define DEBUG_CALENDARCLIENT_ICS_STATEMACHINE 0
#if DEBUG_CALENDARCLIENT_ICS_STATEMACHINE
#define QDEBUG qDebug()
#else
#define QDEBUG if (0) qDebug()
#endif

/***** End of: define Constants *****************************************//*@}*/

/******************************************************************************/
/* Constructors                                                               */
/**********************************//*!@addtogroup constrc Constructors *//*@{*/
/***** End of: constrc Constructors *************************************//*@}*/

/******************************************************************************/
/* Deconstructor                                                              */
/******************************//*!@addtogroup deconstrc Deconstructors *//*@{*/
/***** End of: deconstrc Constructors ***********************************//*@}*/

/******************************************************************************/
/* Public functions                                                           */
/*****************************//*!@addtogroup pubfunct Public functions *//*@{*/
/***** End of: pubfunct Public functions ********************************//*@}*/

/******************************************************************************/
/* Protected functions                                                        */
/*************************//*!@addtogroup protfunct Protected functions *//*@{*/
void CalendarClient_ICS::setupStateMachine(void)
{
    QStateMachine* pStateMachine = new QStateMachine(this);

    QState* pStateWaiting = new QState(pStateMachine);
    QState* pStateReading = new QState(pStateMachine);
    QState* pStateError = new QState(pStateMachine);

    connect(&m_SynchronizationTimer, SIGNAL(timeout()), this, SLOT(debug_handleTimerTimeout()));

    // pStateWaiting
    pStateWaiting->addTransition(&m_SynchronizationTimer, SIGNAL(timeout()), pStateReading);
    pStateWaiting->addTransition(this, SIGNAL(forceSynchronization()), pStateReading);
    connect(pStateWaiting, SIGNAL(entered()),
            this, SLOT(handleStateWaitingEntry()));
    connect(pStateWaiting, SIGNAL(exited()),
            this, SLOT(handleStateWaitingExit()));


    // pStateReading
    pStateReading->addTransition(this, SIGNAL(readingDone()), pStateWaiting);
    pStateReading->addTransition(this, SIGNAL(error(QString)), pStateError);
    connect(pStateReading, SIGNAL(entered()),
            this, SLOT(handleStateReadingEntry()));
    connect(pStateReading, SIGNAL(exited()),
            this, SLOT(handleStateReadingExit()));

    // pStateError
    pStateError->addTransition(this, SIGNAL(recoverSignal()), pStateWaiting);
    connect(pStateError, SIGNAL(entered()),
            this, SLOT(handleStateErrorEntry()));


    pStateMachine->setInitialState(pStateWaiting);

    pStateMachine->start();
}

/***** End of: protfunct Protected functions ****************************//*@}*/

/******************************************************************************/
/* Private functions                                                          */
/*************************//*!@addtogroup privfunct Private functions   *//*@{*/
/***** End of: privfunct Private functions ******************************//*@}*/

/******************************************************************************/
/* Public slots                                                               */
/*************************//*!@addtogroup pubslots Public slots         *//*@{*/
/***** End of: pubslots Public slots ************************************//*@}*/

/******************************************************************************/
/* Protected slots                                                            */
/*********************//*!@addtogroup protslots Protected slots         *//*@{*/


void CalendarClient_ICS::handleStateWaitingEntry(void)
{
    QDEBUG << m_DisplayName << ": " << "entering pStateWaiting";
    m_State = E_STATE_IDLE;
    emit syncStateChanged(m_State);

    if (false != m_bFileReadRequested)
    {
        m_bFileReadRequested = false;
        emit forceSynchronization();
    }
}

void CalendarClient_ICS::handleStateWaitingExit(void)
{
    m_State = E_STATE_BUSY;
    emit syncStateChanged(m_State);
    QDEBUG << m_DisplayName << ": " << "leaving pStateWaiting";
}

void CalendarClient_ICS::handleStateReadingEntry(void)
{
    QDEBUG << m_DisplayName << ": " << "entering pStateReading";
    sendRequest();
}

void CalendarClient_ICS::handleStateReadingExit(void)
{
    QDEBUG << m_DisplayName << ": " << "leaving pStateReading";
}


void CalendarClient_ICS::handleStateErrorEntry(void)
{
    m_State = E_STATE_ERROR;
    emit syncStateChanged(m_State);
    QDEBUG << m_DisplayName << ": " << "entering state error";
}


void CalendarClient_ICS::debug_handleTimerTimeout(void)
{
    QDEBUG << m_DisplayName << ": " << "~~~~~~sync timer timeout~~~~~~";
}

/***** End of: protslots Protected slots ********************************//*@}*/

/******************************************************************************/
/* Private slots                                                              */
/*************************//*!@addtogroup privslots Private slots       *//*@{*/
/***** End of:privslots Private slots ***********************************//*@}*/

/**** Last line of source code                                             ****/
