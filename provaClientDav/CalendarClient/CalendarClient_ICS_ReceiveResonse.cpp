//
// Created by Marco Manco on 29/10/21.
//

/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_ICS_ReceiveResonse.cpp 11 2017-01-16 20:27:13Z cypax $
* Revision of last commit:  $Rev: 11 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-16 21:27:13 +0100 (Mo, 16 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_ICS_StateMachine.cpp
 * @author        Cypax (cypax.net)
 *
 * @brief         Methods to receive HTTP(S) responses from CalDAV server.
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

/******************************************************************************/
/* Namespace                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/

#define DEBUG_CALENDARCLIENT_ICS_RECEIVERESPONSE 0
#if DEBUG_CALENDARCLIENT_ICS_RECEIVERESPONSE
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


void CalendarClient_ICS::handleHTTPError(void)
{
    m_State = E_STATE_ERROR;
    emit syncStateChanged(m_State);
    m_RequestTimeoutTimer.stop();
    if (NULL != m_pReply)
    {
        QDEBUG << m_DisplayName << ": " << "HTTP request error:" << m_pReply->errorString();
        emit error(m_pReply->errorString());
    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when handling HTTP error.";
        emit error("Invalid reply pointer when handling HTTP error.");
    }
}

void CalendarClient_ICS::handleRequestFinished(void)
{
    m_RequestTimeoutTimer.stop();

    if (E_STATE_ERROR == m_State)
    {
        QDEBUG << m_DisplayName << ": " << "Error state - aborting";
    }

    QDEBUG << m_DisplayName << ": " << "HTTP RequestSyncToken finished";

    if (NULL != m_pReply)
    {

        m_EventList.clear();

        QBuffer calBuffer(this); // we need a QBuffer with a copy of the m_pReply QByteArray because m_pReply doesn't support seeking
        calBuffer.setData(m_pReply->readAll());

        calBuffer.open(QIODevice::ReadOnly);
        do
        {
            QString line = calBuffer.readLine();

            if (!line.isNull())
            {
                //QDEBUG << line;
                if (line.startsWith("X-WR-CALNAME:"))
                {
                    QString sDisplayName = line.trimmed();
                    sDisplayName.remove("X-WR-CALNAME:", Qt::CaseInsensitive);
                    QDEBUG << m_DisplayName << ": " << "DISPLAYNAME = " << sDisplayName;
                    this->setDisplayName(sDisplayName);
                }
#if CALENDAR_OVERWRITE_COLOR
                else if (line.startsWith("X-APPLE-CALENDAR-COLOR:"))
        {
          QString sColor = line.trimmed();
          sColor.remove("X-APPLE-CALENDAR-COLOR:", Qt::CaseInsensitive);
          QDEBUG << m_DisplayName << ": " << "COLOR = " << sColor;
          this->setColor(sColor);
        }
#endif
            }
        } while (!calBuffer.atEnd());
        calBuffer.seek(0);
        calBuffer.close();

        if (m_DataStream)
        {
            delete m_DataStream;
        }
        m_DataStream = new QTextStream(calBuffer.data());

        parseCALENDAR("");

        emit eventsUpdated();
        emit readingDone();

    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when receiving URL.";
        emit error("Invalid reply pointer when receiving URL.");
    }

}
