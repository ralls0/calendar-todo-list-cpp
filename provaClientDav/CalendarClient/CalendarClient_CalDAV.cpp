/**
 * @author      Created by Marco Manco on 16/10/21.
 *
 * @brief      Implementation file of class CalendarClient_CalDAV.
 */

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QDebug>

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarClient_CalDAV.h"
#include "CalendarEvent.h"
#include "DateUtils.h"

#define DEBUG_CALENDARCLIENT_CALDAV 0
#if DEBUG_CALENDARCLIENT_CALDAV
#define QDEBUG qDebug()
#else
#define QDEBUG if (0) qDebug()
#endif

CalendarClient_CalDAV::CalendarClient_CalDAV(QObject* parent) : CalendarClient(parent)
{
    m_CalendarType = E_CALENDAR_CALDAV;
    m_DataStream = NULL;
    m_pUploadReply = NULL;

    m_Username = "";
    m_Password = "";
    m_HostURL = "";
    m_syncToken = "";
    m_DisplayName = "";
    m_cTag = "";
    m_Year = 1;
    m_Month = 1;
    m_YearToBeRequested = QDate::currentDate().year();;
    m_MonthToBeRequested = QDate::currentDate().month();
    lastSyncYear = -1;
    lastSyncMonth = -1;
    m_bRecoveredFromError = false;

    setupStateMachine();
}

CalendarClient_CalDAV::~CalendarClient_CalDAV()
{
    m_SynchronizationTimer.stop();
}

int CalendarClient_CalDAV::getYear() const
{
    return m_Year;
}


int CalendarClient_CalDAV::getMonth() const
{
    return m_Month;
}

void CalendarClient_CalDAV::setUsername(const QString username)
{
    m_Username = username;
    emit usernameChanged(m_Username);
}

QString CalendarClient_CalDAV::getUsername(void) const
{
    return m_Username;
}

void CalendarClient_CalDAV::setPassword(const QString password)
{
    m_Password = password;
    emit passwordChanged(m_Password);
}

QString CalendarClient_CalDAV::getPassword(void) const
{
    return m_Password;
}

void CalendarClient_CalDAV::startSynchronization(void)
{
    QDEBUG << m_DisplayName << ": " << "!!!forcing synchronization!!!";
    emit forceSynchronization();
}


void CalendarClient_CalDAV::stopSynchronization(void)
{
}

void CalendarClient_CalDAV::recover(void)
{
    QDEBUG << m_DisplayName << ": " << "trying to recover from EEROR state";
    m_bRecoveredFromError = true;
    emit recoverSignal();
}

void CalendarClient_CalDAV::setYear(const int& year)
{
    if (E_STATE_IDLE == m_State)
    {
        if (m_Year != year)
        {
            QDEBUG << m_DisplayName << ": " << "Year changed from" << m_Year << "to" << year;
            m_Year = year;
            emit yearChanged(m_Year);
            m_YearToBeRequested = year;
            startSynchronization();
        }
    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "requested Year changed from" << m_YearToBeRequested << "to" << year;
        m_YearToBeRequested = year;
    }
}

void CalendarClient_CalDAV::setMonth(const int& month)
{
    if (E_STATE_IDLE == m_State)
    {
        if (m_Month != month)
        {
            QDEBUG << m_DisplayName << ": " << "Month changed from" << m_Month << "to" << month;
            m_Month = month;
            emit monthChanged(m_Month);
            m_MonthToBeRequested = month;
            startSynchronization();
        }
    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "requested Month changed from" << m_MonthToBeRequested << "to" << month;
        m_MonthToBeRequested = month;
    }
}


void CalendarClient_CalDAV::saveEvent(QString uid,
                                      QString filename,
                                      QString summary,
                                      QString location,
                                      QString description,
                                      QString rrule,
                                      QString exdate,
                                      QDateTime startDateTime,
                                      QDateTime endDateTime)
{
    QDEBUG << m_DisplayName << ": " << "saving event" << summary;


    if (NULL != m_pUploadReply)
    {
        QDEBUG << m_DisplayName << ": " << "cleaning up m_pUploadReply";
        m_pUploadReply->abort();
        m_pUploadReply->disconnect();
    }

    QString authorization = "Basic ";
    authorization.append(encodeBase64(m_Username + ":" + m_Password));

    QBuffer* buffer = new QBuffer();

    buffer->open(QIODevice::ReadWrite);


    if (uid.isEmpty())
    {
        uid = QDateTime::currentDateTime().toString("yyyyMMdd-HHMM-00ss") + "-0000-" + startDateTime.toString("yyyyMMddHHMM");
    }

    if (filename.isEmpty())
    {
        filename = uid + ".ics";
    }

    QString requestString = "BEGIN:VCALENDAR\r\n"
                            "BEGIN:VEVENT\r\n"
                            "UID:" + uid + "\r\n"
                                           "VERSION:2.0\r\n"
                                           "DTSTAMP:" + QDateTime::currentDateTime().toString("yyyyMMddTHHmmssZ") + "\r\n"
                                                                                                                    "SUMMARY:" + summary + "\r\n"
                                                                                                                                           "DTSTART:" + startDateTime.toString("yyyyMMddTHHmmss") + "\r\n"
                                                                                                                                                                                                    "DTEND:" + endDateTime.toString("yyyyMMddTHHmmss") + "\r\n"
                                                                                                                                                                                                                                                         "LOCATION:" + location + "\r\n"
                                                                                                                                                                                                                                                                                  "DESCRIPTION:" + description + "\r\n"
                                                                                                                                                                                                                                                                                                                 "TRANSP:OPAQUE\r\n";

    if (!rrule.isEmpty())
    {
        requestString.append("RRULE:" + rrule + "\r\n");
    }

    if (!exdate.isEmpty())
    {
        requestString.append("EXDATE:" + exdate + "\r\n");
    }

    requestString.append("END:VEVENT\r\nEND:VCALENDAR");

    int buffersize = buffer->write(requestString.toUtf8());
    buffer->seek(0);
    buffer->size();

    QByteArray contentlength;
    contentlength.append(QString::number(buffersize).toUtf8());

    QNetworkRequest request;
    request.setUrl(QUrl(m_HostURL.toString() + filename));
    request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
    request.setRawHeader("Authorization", authorization.toUtf8());
    request.setRawHeader("Depth", "0");
    request.setRawHeader("Prefer", "return-minimal");
    request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
    request.setRawHeader("Content-Length", contentlength);


    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    m_pUploadReply = m_UploadNetworkManager.put(request, buffer);

    if (NULL != m_pUploadReply)
    {
        connect(m_pUploadReply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(handleUploadHTTPError()));

        connect(m_pUploadReply, SIGNAL(finished()),
                this, SLOT(handleUploadFinished()));

        m_UploadRequestTimeoutTimer.start(m_RequestTimeoutMS);
    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when requesting URL.";
        emit error("Invalid reply pointer when requesting URL.");
    }

}


void CalendarClient_CalDAV::deleteEvent(QString href)
{
    if (href.isEmpty())
    {
        return;
    }

    QDEBUG << m_DisplayName << ": " << "deleting event with HREF" << href;

    if (NULL != m_pUploadReply)
    {
        QDEBUG << m_DisplayName << ": " << "cleaning up m_pUploadReply";
        m_pUploadReply->abort();
        m_pUploadReply->disconnect();
    }

    QString authorization = "Basic ";
    authorization.append(encodeBase64(m_Username + ":" + m_Password));

    QString filename = QUrl(href).fileName();

    QNetworkRequest request;
    request.setUrl(QUrl(m_HostURL.toString() + filename));
    request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
    request.setRawHeader("Authorization", authorization.toUtf8());
    request.setRawHeader("Depth", "0");
    request.setRawHeader("Prefer", "return-minimal");
    request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
    request.setRawHeader("Content-Length", 0);


    QDEBUG << m_DisplayName << ": " << "deleting" << request.url();


    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    m_pUploadReply = m_UploadNetworkManager.deleteResource(request);

    if (NULL != m_pUploadReply)
    {
        connect(m_pUploadReply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(handleUploadHTTPError()));

        connect(m_pUploadReply, SIGNAL(finished()),
                this, SLOT(handleUploadFinished()));

        m_UploadRequestTimeoutTimer.start(m_RequestTimeoutMS);
    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when requesting URL.";
        emit error("Invalid reply pointer when requesting URL.");
    }

}


void CalendarClient_CalDAV::handleUploadHTTPError(void)
{
    m_UploadRequestTimeoutTimer.stop();
    if (NULL != m_pUploadReply)
    {
        QDEBUG << m_DisplayName << ": " << "HTTP upload error:" << m_pUploadReply->errorString();
        emit error(m_pUploadReply->errorString());
    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when handling HTTP error.";
        emit error("Invalid reply pointer when handling HTTP error.");
    }

}

void CalendarClient_CalDAV::handleUploadFinished(void)
{
    m_UploadRequestTimeoutTimer.stop();

    QDEBUG << m_DisplayName << ": " << "HTTP upload finished";

    if (NULL != m_pUploadReply)
    {
        QDEBUG << m_DisplayName << ": " << "received:\r\n" << m_pUploadReply->readAll();
        emit forceSynchronization();
    }
}


void CalendarClient_CalDAV::sendRequestChanges(void)
{
    QString authorization = "Basic ";
    authorization.append(encodeBase64(m_Username + ":" + m_Password));

    QBuffer* buffer = new QBuffer();

    buffer->open(QIODevice::ReadWrite);

    QString monthString = QString::number(m_Month);
    if (monthString.length() < 2) monthString.prepend("0");

    QString lastDayString = QString::number(DateUtils::lastDayOfMonth(m_Year, m_Month));
    if (lastDayString.length() < 2) lastDayString.prepend("0");


    QString requestString = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n"
                            "<C:calendar-query xmlns:D=\"DAV:\" xmlns:C=\"urn:ietf:params:xml:ns:caldav\">\r\n"
                            " <D:prop>\r\n"
                            "   <D:getetag/>\r\n"
                            "   <C:calendar-data>\r\n"
                            "     <C:comp name=\"VCALENDAR\">\r\n"
                            "       <C:prop name=\"VERSION\"/>\r\n"
                            "       <C:comp name=\"VEVENT\">\r\n"
                            "         <C:prop name=\"SUMMARY\"/>\r\n"
                            "         <C:prop name=\"LOCATION\"/>\r\n"
                            "         <C:prop name=\"DESCRIPTION\"/>\r\n"
                            "         <C:prop name=\"UID\"/>\r\n"
                            "         <C:prop name=\"DTSTART\"/>\r\n"
                            "         <C:prop name=\"DTEND\"/>\r\n"
                            "       </C:comp>\r\n"
                            "     </C:comp>\r\n"
                            "   </C:calendar-data>\r\n"
                            " </D:prop>\r\n"
                            " <C:filter>\r\n"
                            "   <C:comp-filter name=\"VCALENDAR\">\r\n"
                            "     <C:comp-filter name=\"VEVENT\">\r\n"
                            "       <C:time-range start=\"" + QString::number(m_Year) + monthString + "01T000000Z\" end=\"" + QString::number(m_Year) + monthString + lastDayString + "T235959Z\"/>\r\n"
                                                                                                                                                                                      "     </C:comp-filter>\r\n"
                                                                                                                                                                                      "   </C:comp-filter>\r\n"
                                                                                                                                                                                      " </C:filter>\r\n"
                                                                                                                                                                                      "</C:calendar-query>\r\n";

    //QDEBUG << m_DisplayName << ": " << "requesting:";
    //QDEBUG << m_DisplayName << ": " << requestString;
    //QDEBUG << m_DisplayName << ": " << "<C:time-range start=\"" + QString::number(m_Year) + monthString + "01T000000Z\" end=\"" + QString::number(m_Year) + monthString + lastDayString + "T235959Z\"/>";

    int buffersize = buffer->write(requestString.toUtf8());
    buffer->seek(0);
    buffer->size();

    QByteArray contentlength;
    contentlength.append(QString::number(buffersize).toUtf8());

    QNetworkRequest request;
    request.setUrl(m_HostURL);
    request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
    request.setRawHeader("Authorization", authorization.toUtf8());
    request.setRawHeader("Depth", "1");
    request.setRawHeader("Content-Type", "application/xml; charset=utf-8");
    request.setRawHeader("Content-Length", contentlength);

    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    m_pReply = m_NetworkManager.sendCustomRequest(request, QByteArray("REPORT"), buffer);

    if (NULL != m_pReply)
    {
        connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(handleHTTPError()));

        connect(m_pReply, SIGNAL(finished()),
                this, SLOT(handleRequestChangesFinished()));

        m_RequestTimeoutTimer.start(m_RequestTimeoutMS);
    }
    else
    {
        QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when requesting sync token.";
        emit error("Invalid reply pointer when requesting sync token.");
    }
}


void CalendarClient_CalDAV::sendRequestSyncToken(void)
{
    return; // FIXME
}
