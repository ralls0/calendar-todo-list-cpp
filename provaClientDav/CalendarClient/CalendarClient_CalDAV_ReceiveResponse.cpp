//
// Created by Marco Manco on 29/10/21.
//

/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_CalDAV_ReceiveResonse.cpp 18 2017-01-26 17:33:06Z cypax $
* Revision of last commit:  $Rev: 18 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-26 18:33:06 +0100 (Do, 26 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_CalDAV_StateMachine.cpp
 * @author        Cypax (cypax.net)
 *
 * @brief         Methods to receive HTTP(S) responses from CalDAV server.
 */
/************************************************************************//*@}*/

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QDebug>
#include <QDomDocument>

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarClient_CalDAV.h"

/******************************************************************************/
/* Namespace                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/

#define DEBUG_CALENDARCLIENT_CALDAV_RECEIVERESPONSE 0
#if DEBUG_CALENDARCLIENT_CALDAV_RECEIVERESPONSE
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


void CalendarClient_CalDAV::handleHTTPError(void)
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

void CalendarClient_CalDAV::handleRequestSyncTokenFinished(void)
{
    m_RequestTimeoutTimer.stop();

    if (E_STATE_ERROR == m_State)
    {
        QDEBUG << m_DisplayName << ": " << "Error state - aborting";
    }

    QDEBUG << m_DisplayName << ": " << "HTTP RequestSyncToken finished";

    if (NULL != m_pReply)
    {
        QDomDocument doc;

        doc.setContent(m_pReply);

        QString sDisplayName = "";
        QString sCTag = "";
        QString sSyncToken = "";
        QString sStatus = "";


        QDomNodeList response = doc.elementsByTagName("d:prop");
        for (int i = 0; i < response.size(); i++)
        {
            QDomNode n = response.item(i);
            QDomElement displayname = n.firstChildElement("d:displayname");
            if (!displayname.isNull())
            {
                //QDEBUG << m_DisplayName << ": " << "DISPLAYNAME = " << displayname.text();
                sDisplayName = displayname.text();
            }
            QDomElement ctag = n.firstChildElement("cs:getctag");
            if (!ctag.isNull())
            {
                //QDEBUG << m_DisplayName << ": " << "CTAG = " << ctag.text();
                sCTag = ctag.text();
            }
            QDomElement syncToken = n.firstChildElement("d:sync-token");
            if (!syncToken.isNull())
            {
                //QDEBUG << m_DisplayName << ": " << "SYNC-TOKEN = " << syncToken.text();
                sSyncToken = syncToken.text();
            }
        }

        response = doc.elementsByTagName("d:propstat");
        for (int i = 0; i < response.size(); i++)
        {
            QDomNode n = response.item(i);
            QDomElement status = n.firstChildElement("d:status");
            if (!status.isNull())
            {
                //QDEBUG << m_DisplayName << ": " << "STATUS = " << status.text();
                sStatus = status.text();
            }
        }

        if ( (!sSyncToken.isEmpty()) && (sStatus.endsWith("200 OK")) )
        {
            bool bDisplayNameChanged = (m_DisplayName != sDisplayName);
            bool bSyncTokenChanged = (m_syncToken != sSyncToken);

            if (true == bSyncTokenChanged)
            {
                QDEBUG << m_DisplayName << ": " << "sync token has changed from"  << m_syncToken << "to" << sSyncToken;
            }

            m_DisplayName = sDisplayName;
            m_cTag = sCTag;
            m_syncToken = sSyncToken;

            if (false != bDisplayNameChanged)
            {
                emit displayNameChanged(m_DisplayName);
            }

            if (false == bSyncTokenChanged)
            {
                QDEBUG << m_DisplayName << ": " << "sync token is unchanged";
                emit syncTokenHasNotChanged();

                if ( (m_Year != lastSyncYear) || (m_Month != lastSyncMonth) )
                {
                    QDEBUG << m_DisplayName << ": " << "year/month has changed from" << lastSyncYear << lastSyncMonth << "to" << m_Year << m_Month << "=> update required";
                    lastSyncMonth = m_Month;
                    lastSyncYear = m_Year;
                    emit calendarUpdateRequired();
                }
                else
                {
                    QDEBUG << m_DisplayName << ": " << "calendar has not changed, no update required";
                    m_SynchronizationTimer.start();
                    emit calendarHasNotChanged();
                }
            }
            else
            {
                QDEBUG << m_DisplayName << ": " << "sync token has changed";
                emit syncTokenChanged();
                lastSyncMonth = m_Month;
                lastSyncYear = m_Year;
                emit calendarUpdateRequired();
            }
        }
        else
        {
            QDEBUG << m_DisplayName << ": " << "ERROR: Receiving sync token failed. Status:" << sStatus;
            emit error("Receiving sync token failed.");
        }

        /*
        QDEBUG << m_DisplayName << ": " << "\r\nHeaders:" << m_pReply->rawHeaderList() << "\r\n";

        if (m_pReply->hasRawHeader("DAV")) {
          QDEBUG << m_DisplayName << ": " << "DAV:" << m_pReply->rawHeader("DAV");
        }
        if (m_pReply->hasRawHeader("Server")) {
          QDEBUG << m_DisplayName << ": " << "Server:" << m_pReply->rawHeader("Server");
        }
        QDEBUG << m_DisplayName << ": " << "Status code:" << m_pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        */
    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when receiving sync token.";
        emit error("Invalid reply pointer when receiving sync token.");
    }

}


void CalendarClient_CalDAV::handleRequestChangesFinished(void)
{
    m_RequestTimeoutTimer.stop();

    if (E_STATE_ERROR == m_State)
    {
        QDEBUG << m_DisplayName << ": " << "Error state - aborting";
    }

    QDEBUG << m_DisplayName << ": " << "HTTP RequestChanges finished";

    if (NULL != m_pReply)
    {
        QDomDocument doc;

        //QDEBUG << m_DisplayName << ": " << "received:\r\n" << m_pReply->readAll();

        doc.setContent(m_pReply);

        m_EventList.clear();

        QDomNodeList list_response = doc.elementsByTagName("d:response");
        for (int i = 0; i < list_response.size(); i++)
        {
            QDomNode thisResponse = list_response.item(i);

            //QDEBUG << m_DisplayName << ": " << "Response" << i;

            QString sHref = "";
            QString sETag = "";
            QString sPropStatus = "";
            QString strCalendarData = "";

            QDomElement elHref = thisResponse.firstChildElement("d:href");
            if (!elHref.isNull())
            {
                //QDEBUG << m_DisplayName << ": " << "  HREF = " << elHref.text();
                sHref = elHref.text();
            }
            else
            {
                QDEBUG << m_DisplayName << ": " << "  HREF = ";
            }


            QDomNode thisPropStat = thisResponse.firstChildElement("d:propstat");
            if (!thisPropStat.isNull())
            {

                QDomElement elPropStatus = thisPropStat.firstChildElement("d:status");
                if (!elPropStatus.isNull())
                {
                    //QDEBUG << m_DisplayName << ": " << "    PROPSTATUS = " << elPropStatus.text();
                    sPropStatus = elPropStatus.text();
                }
                else
                {
                    QDEBUG << m_DisplayName << ": " << "    PROPSTATUS = ";
                }


                QDomNode thisProp = thisPropStat.firstChildElement("d:prop");
                if (!thisProp.isNull())
                {
                    QDomElement elETag = thisProp.firstChildElement("d:getetag");
                    if (!elETag.isNull())
                    {
                        //QDEBUG << m_DisplayName << ": " << "    ETAG = " << elETag.text();
                        sETag = elETag.text();
                    }
                    else
                    {
                        QDEBUG << m_DisplayName << ": " << "    ETAG = ";
                    }

                    QDomElement elCalendarData = thisProp.firstChildElement("cal:calendar-data");
                    if (!elCalendarData.isNull())
                    {
                        //QDEBUG << m_DisplayName << ": " << "    CALENDARDATA = " << elCalendarData.text();

                        if (m_DataStream)
                        {
                            delete m_DataStream;
                        }
                        m_DataStream = new QTextStream(elCalendarData.text().toLatin1());

                        parseCALENDAR(sHref);

                        strCalendarData = elCalendarData.text();
                    }
                    else
                    {
                        QDEBUG << m_DisplayName << ": " << "    CALENDARDATA = ";
                    }

                }
            }
        }

        int iStatusCode = m_pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QDEBUG << m_DisplayName << ": " << "Status code:" << iStatusCode;

        if (207 != iStatusCode)
        {
            QDEBUG << m_DisplayName << ": " << "ERROR: Invalid HTTP return code:" << iStatusCode;
            emit error("Invalid HTTP return code.");
        }
        else
        {
            QDEBUG << m_DisplayName << ": " << "OK, restarting synchronization\r\n\r\n";
            m_SynchronizationTimer.start();
            emit eventsUpdated();
        }

    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when receiving changes.";
        emit error("Invalid reply pointer when receiving changes.");
    }

}
