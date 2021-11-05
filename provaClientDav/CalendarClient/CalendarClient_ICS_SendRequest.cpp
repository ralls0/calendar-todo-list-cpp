//
// Created by Marco Manco on 29/10/21.
//

/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_ICS_SendRequest.cpp 11 2017-01-16 20:27:13Z cypax $
* Revision of last commit:  $Rev: 11 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-16 21:27:13 +0100 (Mo, 16 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_ICS_StateMachine.cpp
 * @author        Cypax (cypax.net)
 *
 * @brief         Methods to send HTTP(S) requests to CalDAV server.
 */
/************************************************************************//*@}*/

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QDebug>
#include <QXmlStreamReader>
//#include <QXmlContentHandler>
//#include <QXmlQuery>
#include <QXmlStreamNamespaceDeclaration>

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

#define DEBUG_CALENDARCLIENT_ICS_SENDREQUEST 0
#if DEBUG_CALENDARCLIENT_ICS_SENDREQUEST
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


//void CalendarClient_ICS::sendRequest(void)
//{
//    if (m_HostURL.isLocalFile())
//    {
//        QString filename = m_HostURL.toString().remove("file:///");
//        if (QUrl(filename).isRelative())
//        {
//            QDEBUG << m_DisplayName << ": " << "is realtive path:" << filename;
//            filename = filename.prepend(QCoreApplication::applicationDirPath() + "/");
//        }
//        filename = QUrl(filename).toString(QUrl::NormalizePathSegments);
//        QDEBUG << m_DisplayName << ": " << "reading local file" << filename;
//
//        QFile file(filename);
//
//        if (file.exists())
//        {
//            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//            {
//                QDEBUG << m_DisplayName << ": " << "ERROR: could not read file" << filename << "|" << file.errorString();
//                emit error("Could not read file " + filename);
//                return;
//            }
//        }
//        else
//        {
//            QDEBUG << m_DisplayName << ": " << "ERROR: file" << filename << "does not exist";
//            emit error("File " + filename + " does not exist");
//            return;
//        }
//
//        if (m_DataStream)
//        {
//            delete m_DataStream;
//        }
//        m_DataStream = new QTextStream(&file);
//
//        m_EventList.clear();
//
//        do
//        {
//            QString line = m_DataStream->readLine();
//
//            if (!line.isNull())
//            {
//                //QDEBUG << line;
//                if (line.startsWith("X-WR-CALNAME:"))
//                {
//                    QString sDisplayName = line.trimmed();
//                    sDisplayName.remove("X-WR-CALNAME:", Qt::CaseInsensitive);
//                    QDEBUG << m_DisplayName << ": " << "DISPLAYNAME = " << sDisplayName;
//                    this->setDisplayName(sDisplayName);
//                }
//#if CALENDAR_OVERWRITE_COLOR
//                else if (line.startsWith("X-APPLE-CALENDAR-COLOR:"))
//        {
//          QString sColor = line.trimmed();
//          sColor.remove("X-APPLE-CALENDAR-COLOR:", Qt::CaseInsensitive);
//          QDEBUG << m_DisplayName << ": " << "COLOR = " << sColor;
//          this->setColor(sColor);
//        }
//#endif
//            }
//        } while (!m_DataStream->atEnd());
//        m_DataStream->seek(0);
//
//        parseCALENDAR("");
//
//        emit eventsUpdated();
//        emit readingDone();
//
//        m_SynchronizationTimer.start();
//
//    }
//    else
//    {
//        if (NULL != m_pReply)
//        {
//            QDEBUG << m_DisplayName << ": " << "cleaning up m_pReply";
//            m_pReply->abort();
//            m_pReply->disconnect();
//        }
//
//        QDEBUG << m_DisplayName << ": " << "downloading" << m_HostURL.toString();
//
//        QNetworkRequest request;
//        request.setUrl(m_HostURL);
//        request.setRawHeader("User-Agent", "CalendarClient_ICS");
//        request.setRawHeader("Depth", "0");
//        request.setRawHeader("Prefer", "return-minimal");
//        request.setRawHeader("Content-Type", "text/xml; charset=utf-8");
//        request.setRawHeader("Content-Length", "0");
//
//        QSslConfiguration conf = request.sslConfiguration();
//        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
//        request.setSslConfiguration(conf);
//
//        m_pReply = m_NetworkManager.get(request);
//
//        if (NULL != m_pReply)
//        {
//            connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)),
//                    this, SLOT(handleHTTPError()));
//
//            connect(m_pReply, SIGNAL(finished()),
//                    this, SLOT(handleRequestFinished()));
//
//            m_RequestTimeoutTimer.start(m_RequestTimeoutMS);
//        }
//        else
//        {
//            QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when requesting URL.";
//            emit error("Invalid reply pointer when requesting URL.");
//        }
//    }
//}
// FIXME