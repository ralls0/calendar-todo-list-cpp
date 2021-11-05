/**
 * @author      Created by Marco Manco on 16/10/21.
 *
 * @brief      Implementation file of class CalendarClient_CalDAV.
 */

#include <QDebug>
#include <QDomDocument>
#include <QStateMachine>
#include <QXmlStreamReader>
// #include <QXmlStreamNamespaceDeclaration> FIXME se servisse

#include "../DateUtils.h"
#include "CalendarClient_CalDAV.h"
#include "CalendarEvent.h"

#define DEBUG_CALENDARCLIENT_CALDAV 0
#if DEBUG_CALENDARCLIENT_CALDAV
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

CalendarClient_CalDAV::CalendarClient_CalDAV(QObject *parent)
    : CalendarClient(parent) {
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
  m_YearToBeRequested = QDate::currentDate().year();
  ;
  m_MonthToBeRequested = QDate::currentDate().month();
  lastSyncYear = -1;
  lastSyncMonth = -1;
  m_bRecoveredFromError = false;

  setupStateMachine();
}

CalendarClient_CalDAV::~CalendarClient_CalDAV() {
  m_SynchronizationTimer.stop();
}

int CalendarClient_CalDAV::getYear() const { return m_Year; }

int CalendarClient_CalDAV::getMonth() const { return m_Month; }

void CalendarClient_CalDAV::setUsername(const QString username) {
  m_Username = username;
  emit usernameChanged(m_Username);
}

QString CalendarClient_CalDAV::getUsername(void) const { return m_Username; }

void CalendarClient_CalDAV::setPassword(const QString password) {
  m_Password = password;
  emit passwordChanged(m_Password);
}

QString CalendarClient_CalDAV::getPassword(void) const { return m_Password; }

void CalendarClient_CalDAV::startSynchronization(void) {
  QDEBUG << m_DisplayName << ": "
         << "!!!forcing synchronization!!!";
  emit forceSynchronization();
}

void CalendarClient_CalDAV::stopSynchronization(void) {}

void CalendarClient_CalDAV::recover(void) {
  QDEBUG << m_DisplayName << ": "
         << "trying to recover from EEROR state";
  m_bRecoveredFromError = true;
  emit recoverSignal();
}

void CalendarClient_CalDAV::setYear(const int &year) {
  if (E_STATE_IDLE == m_State) {
    if (m_Year != year) {
      QDEBUG << m_DisplayName << ": "
             << "Year changed from" << m_Year << "to" << year;
      m_Year = year;
      emit yearChanged(m_Year);
      m_YearToBeRequested = year;
      startSynchronization();
    }
  } else {
    QDEBUG << m_DisplayName << ": "
           << "requested Year changed from" << m_YearToBeRequested << "to"
           << year;
    m_YearToBeRequested = year;
  }
}

void CalendarClient_CalDAV::setMonth(const int &month) {
  if (E_STATE_IDLE == m_State) {
    if (m_Month != month) {
      QDEBUG << m_DisplayName << ": "
             << "Month changed from" << m_Month << "to" << month;
      m_Month = month;
      emit monthChanged(m_Month);
      m_MonthToBeRequested = month;
      startSynchronization();
    }
  } else {
    QDEBUG << m_DisplayName << ": "
           << "requested Month changed from" << m_MonthToBeRequested << "to"
           << month;
    m_MonthToBeRequested = month;
  }
}

void CalendarClient_CalDAV::saveEvent(QString uid, QString filename,
                                      QString summary, QString location,
                                      QString description, QString rrule,
                                      QString exdate, QDateTime startDateTime,
                                      QDateTime endDateTime) {
  QDEBUG << m_DisplayName << ": "
         << "saving event" << summary;

  if (NULL != m_pUploadReply) {
    QDEBUG << m_DisplayName << ": "
           << "cleaning up m_pUploadReply";
    m_pUploadReply->abort();
    m_pUploadReply->disconnect();
  }

  QString authorization = "Basic ";
  authorization.append(encodeBase64(m_Username + ":" + m_Password));

  QBuffer *buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);

  if (uid.isEmpty()) {
    uid = QDateTime::currentDateTime().toString("yyyyMMdd-HHMM-00ss") +
          "-0000-" + startDateTime.toString("yyyyMMddHHMM");
  }

  if (filename.isEmpty()) {
    filename = uid + ".ics";
  }

  QString requestString =
      "BEGIN:VCALENDAR\r\n"
      "BEGIN:VEVENT\r\n"
      "UID:" +
      uid +
      "\r\n"
      "VERSION:2.0\r\n"
      "DTSTAMP:" +
      QDateTime::currentDateTime().toString("yyyyMMddTHHmmssZ") +
      "\r\n"
      "SUMMARY:" +
      summary +
      "\r\n"
      "DTSTART:" +
      startDateTime.toString("yyyyMMddTHHmmss") +
      "\r\n"
      "DTEND:" +
      endDateTime.toString("yyyyMMddTHHmmss") +
      "\r\n"
      "LOCATION:" +
      location +
      "\r\n"
      "DESCRIPTION:" +
      description +
      "\r\n"
      "TRANSP:OPAQUE\r\n";

  if (!rrule.isEmpty()) {
    requestString.append("RRULE:" + rrule + "\r\n");
  }

  if (!exdate.isEmpty()) {
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

  if (NULL != m_pUploadReply) {
    connect(m_pUploadReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(handleUploadHTTPError()));

    connect(m_pUploadReply, SIGNAL(finished()), this,
            SLOT(handleUploadFinished()));

    m_UploadRequestTimeoutTimer.start(m_RequestTimeoutMS);
  } else {
    QDEBUG << m_DisplayName << ": "
           << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }
}

void CalendarClient_CalDAV::deleteEvent(QString href) {
  if (href.isEmpty()) {
    return;
  }

  QDEBUG << m_DisplayName << ": "
         << "deleting event with HREF" << href;

  if (NULL != m_pUploadReply) {
    QDEBUG << m_DisplayName << ": "
           << "cleaning up m_pUploadReply";
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

  QDEBUG << m_DisplayName << ": "
         << "deleting" << request.url();

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  m_pUploadReply = m_UploadNetworkManager.deleteResource(request);

  if (NULL != m_pUploadReply) {
    connect(m_pUploadReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(handleUploadHTTPError()));

    connect(m_pUploadReply, SIGNAL(finished()), this,
            SLOT(handleUploadFinished()));

    m_UploadRequestTimeoutTimer.start(m_RequestTimeoutMS);
  } else {
    QDEBUG << m_DisplayName << ": "
           << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }
}

void CalendarClient_CalDAV::handleUploadHTTPError(void) {
  m_UploadRequestTimeoutTimer.stop();
  if (NULL != m_pUploadReply) {
    QDEBUG << m_DisplayName << ": "
           << "HTTP upload error:" << m_pUploadReply->errorString();
    emit error(m_pUploadReply->errorString());
  } else {
    QDEBUG << m_DisplayName << ": "
           << "ERROR: Invalid reply pointer when handling HTTP error.";
    emit error("Invalid reply pointer when handling HTTP error.");
  }
}

void CalendarClient_CalDAV::handleUploadFinished(void) {
  m_UploadRequestTimeoutTimer.stop();

  QDEBUG << m_DisplayName << ": "
         << "HTTP upload finished";

  if (NULL != m_pUploadReply) {
    QDEBUG << m_DisplayName << ": "
           << "received:\r\n"
           << m_pUploadReply->readAll();
    emit forceSynchronization();
  }
}

/**
 * @brief         Methods to receive HTTP(S) responses from CalDAV server.
 */

void CalendarClient_CalDAV::handleHTTPError(void) {
  m_State = E_STATE_ERROR;
  emit syncStateChanged(m_State);
  m_RequestTimeoutTimer.stop();
  if (NULL != m_pReply) {
    QDEBUG << m_DisplayName << ": "
           << "HTTP request error:" << m_pReply->errorString();
    emit error(m_pReply->errorString());
  } else {
    QDEBUG << m_DisplayName << ": "
           << "ERROR: Invalid reply pointer when handling HTTP error.";
    emit error("Invalid reply pointer when handling HTTP error.");
  }
}

void CalendarClient_CalDAV::handleRequestSyncTokenFinished(void) {
  m_RequestTimeoutTimer.stop();

  if (E_STATE_ERROR == m_State) {
    QDEBUG << m_DisplayName << ": "
           << "Error state - aborting";
  }

  QDEBUG << m_DisplayName << ": "
         << "HTTP RequestSyncToken finished";

  if (NULL != m_pReply) {
    QDomDocument doc;

    doc.setContent(m_pReply);

    QString sDisplayName = "";
    QString sCTag = "";
    QString sSyncToken = "";
    QString sStatus = "";

    QDomNodeList response = doc.elementsByTagName("d:prop");
    for (int i = 0; i < response.size(); i++) {
      QDomNode n = response.item(i);
      QDomElement displayname = n.firstChildElement("d:displayname");
      if (!displayname.isNull()) {
        // QDEBUG << m_DisplayName << ": " << "DISPLAYNAME = " <<
        // displayname.text();
        sDisplayName = displayname.text();
      }
      QDomElement ctag = n.firstChildElement("cs:getctag");
      if (!ctag.isNull()) {
        // QDEBUG << m_DisplayName << ": " << "CTAG = " << ctag.text();
        sCTag = ctag.text();
      }
      QDomElement syncToken = n.firstChildElement("d:sync-token");
      if (!syncToken.isNull()) {
        // QDEBUG << m_DisplayName << ": " << "SYNC-TOKEN = " <<
        // syncToken.text();
        sSyncToken = syncToken.text();
      }
    }

    response = doc.elementsByTagName("d:propstat");
    for (int i = 0; i < response.size(); i++) {
      QDomNode n = response.item(i);
      QDomElement status = n.firstChildElement("d:status");
      if (!status.isNull()) {
        // QDEBUG << m_DisplayName << ": " << "STATUS = " << status.text();
        sStatus = status.text();
      }
    }

    if ((!sSyncToken.isEmpty()) && (sStatus.endsWith("200 OK"))) {
      bool bDisplayNameChanged = (m_DisplayName != sDisplayName);
      bool bSyncTokenChanged = (m_syncToken != sSyncToken);

      if (true == bSyncTokenChanged) {
        QDEBUG << m_DisplayName << ": "
               << "sync token has changed from" << m_syncToken << "to"
               << sSyncToken;
      }

      m_DisplayName = sDisplayName;
      m_cTag = sCTag;
      m_syncToken = sSyncToken;

      if (false != bDisplayNameChanged) {
        emit displayNameChanged(m_DisplayName);
      }

      if (false == bSyncTokenChanged) {
        QDEBUG << m_DisplayName << ": "
               << "sync token is unchanged";
        emit syncTokenHasNotChanged();

        if ((m_Year != lastSyncYear) || (m_Month != lastSyncMonth)) {
          QDEBUG << m_DisplayName << ": "
                 << "year/month has changed from" << lastSyncYear
                 << lastSyncMonth << "to" << m_Year << m_Month
                 << "=> update required";
          lastSyncMonth = m_Month;
          lastSyncYear = m_Year;
          emit calendarUpdateRequired();
        } else {
          QDEBUG << m_DisplayName << ": "
                 << "calendar has not changed, no update required";
          m_SynchronizationTimer.start();
          emit calendarHasNotChanged();
        }
      } else {
        QDEBUG << m_DisplayName << ": "
               << "sync token has changed";
        emit syncTokenChanged();
        lastSyncMonth = m_Month;
        lastSyncYear = m_Year;
        emit calendarUpdateRequired();
      }
    } else {
      QDEBUG << m_DisplayName << ": "
             << "ERROR: Receiving sync token failed. Status:" << sStatus;
      emit error("Receiving sync token failed.");
    }

    /*
    QDEBUG << m_DisplayName << ": " << "\r\nHeaders:" <<
    m_pReply->rawHeaderList() << "\r\n";

    if (m_pReply->hasRawHeader("DAV")) {
      QDEBUG << m_DisplayName << ": " << "DAV:" << m_pReply->rawHeader("DAV");
    }
    if (m_pReply->hasRawHeader("Server")) {
      QDEBUG << m_DisplayName << ": " << "Server:" <<
    m_pReply->rawHeader("Server");
    }
    QDEBUG << m_DisplayName << ": " << "Status code:" <<
    m_pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    */
  } else {
    QDEBUG << m_DisplayName << ": "
           << "ERROR: Invalid reply pointer when receiving sync token.";
    emit error("Invalid reply pointer when receiving sync token.");
  }
}

void CalendarClient_CalDAV::handleRequestChangesFinished(void) {
  m_RequestTimeoutTimer.stop();

  if (E_STATE_ERROR == m_State) {
    QDEBUG << m_DisplayName << ": "
           << "Error state - aborting";
  }

  QDEBUG << m_DisplayName << ": "
         << "HTTP RequestChanges finished";

  if (NULL != m_pReply) {
    QDomDocument doc;

    // QDEBUG << m_DisplayName << ": " << "received:\r\n" <<
    // m_pReply->readAll();

    doc.setContent(m_pReply);

    m_EventList.clear();

    QDomNodeList list_response = doc.elementsByTagName("d:response");
    for (int i = 0; i < list_response.size(); i++) {
      QDomNode thisResponse = list_response.item(i);

      // QDEBUG << m_DisplayName << ": " << "Response" << i;

      QString sHref = "";
      QString sETag = "";
      QString sPropStatus = "";
      QString strCalendarData = "";

      QDomElement elHref = thisResponse.firstChildElement("d:href");
      if (!elHref.isNull()) {
        // QDEBUG << m_DisplayName << ": " << "  HREF = " << elHref.text();
        sHref = elHref.text();
      } else {
        QDEBUG << m_DisplayName << ": "
               << "  HREF = ";
      }

      QDomNode thisPropStat = thisResponse.firstChildElement("d:propstat");
      if (!thisPropStat.isNull()) {

        QDomElement elPropStatus = thisPropStat.firstChildElement("d:status");
        if (!elPropStatus.isNull()) {
          // QDEBUG << m_DisplayName << ": " << "    PROPSTATUS = " <<
          // elPropStatus.text();
          sPropStatus = elPropStatus.text();
        } else {
          QDEBUG << m_DisplayName << ": "
                 << "    PROPSTATUS = ";
        }

        QDomNode thisProp = thisPropStat.firstChildElement("d:prop");
        if (!thisProp.isNull()) {
          QDomElement elETag = thisProp.firstChildElement("d:getetag");
          if (!elETag.isNull()) {
            // QDEBUG << m_DisplayName << ": " << "    ETAG = " <<
            // elETag.text();
            sETag = elETag.text();
          } else {
            QDEBUG << m_DisplayName << ": "
                   << "    ETAG = ";
          }

          QDomElement elCalendarData =
              thisProp.firstChildElement("cal:calendar-data");
          if (!elCalendarData.isNull()) {
            // QDEBUG << m_DisplayName << ": " << "    CALENDARDATA = " <<
            // elCalendarData.text();

            if (m_DataStream) {
              delete m_DataStream;
            }
            m_DataStream = new QTextStream(elCalendarData.text().toLatin1());

            parseCALENDAR(sHref);

            strCalendarData = elCalendarData.text();
          } else {
            QDEBUG << m_DisplayName << ": "
                   << "    CALENDARDATA = ";
          }
        }
      }
    }

    int iStatusCode =
        m_pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QDEBUG << m_DisplayName << ": "
           << "Status code:" << iStatusCode;

    if (207 != iStatusCode) {
      QDEBUG << m_DisplayName << ": "
             << "ERROR: Invalid HTTP return code:" << iStatusCode;
      emit error("Invalid HTTP return code.");
    } else {
      QDEBUG << m_DisplayName << ": "
             << "OK, restarting synchronization\r\n\r\n";
      m_SynchronizationTimer.start();
      emit eventsUpdated();
    }

  } else {
    QDEBUG << m_DisplayName << ": "
           << "ERROR: Invalid reply pointer when receiving changes.";
    emit error("Invalid reply pointer when receiving changes.");
  }
}

/**
 * @brief         Methods to send HTTP(S) requests to CalDAV server.
 */

void CalendarClient_CalDAV::sendRequestSyncToken(void) {
  if (NULL != m_pReply) {
    QDEBUG << m_DisplayName << ": "
           << "cleaning up m_pReply";
    m_pReply->abort();
    m_pReply->disconnect();
  }

  QString authorization = "Basic ";
  authorization.append(encodeBase64(m_Username + ":" + m_Password));

  QBuffer *buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);

  QString requestString = "<d:propfind xmlns:d=\"DAV:\" "
                          "xmlns:cs=\"http://calendarserver.org/ns/\">\r\n"
                          "  <d:prop>\r\n"
                          "    <d:displayname />\r\n"
                          "    <cs:getctag />\r\n"
                          "    <d:sync-token />"
                          "  </d:prop>\r\n"
                          "</d:propfind>";

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QString::number(buffersize).toUtf8());

  QNetworkRequest request;
  request.setUrl(m_HostURL);
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Prefer", "return-minimal");
  request.setRawHeader("Content-Type", "text/xml; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  m_pReply = m_NetworkManager.sendCustomRequest(request, QByteArray("PROPFIND"),
                                                buffer);

  if (NULL != m_pReply) {
    connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(handleHTTPError()));

    connect(m_pReply, SIGNAL(finished()), this,
            SLOT(handleRequestSyncTokenFinished()));

    m_RequestTimeoutTimer.start(m_RequestTimeoutMS);
  } else {
    QDEBUG << m_DisplayName << ": "
           << "ERROR: Invalid reply pointer when requesting sync token.";
    emit error("Invalid reply pointer when requesting sync token.");
  }
}

void CalendarClient_CalDAV::sendRequestChanges(void) {
  QString authorization = "Basic ";
  authorization.append(encodeBase64(m_Username + ":" + m_Password));

  QBuffer *buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);

  QString monthString = QString::number(m_Month);
  if (monthString.length() < 2)
    monthString.prepend("0");

  QString lastDayString =
      QString::number(DateUtils::lastDayOfMonth(m_Year, m_Month));
  if (lastDayString.length() < 2)
    lastDayString.prepend("0");

  QString requestString = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n"
                          "<C:calendar-query xmlns:D=\"DAV:\" "
                          "xmlns:C=\"urn:ietf:params:xml:ns:caldav\">\r\n"
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
                          "       <C:time-range start=\"" +
                          QString::number(m_Year) + monthString +
                          "01T000000Z\" end=\"" + QString::number(m_Year) +
                          monthString + lastDayString +
                          "T235959Z\"/>\r\n"
                          "     </C:comp-filter>\r\n"
                          "   </C:comp-filter>\r\n"
                          " </C:filter>\r\n"
                          "</C:calendar-query>\r\n";

  // QDEBUG << m_DisplayName << ": " << "requesting:";
  // QDEBUG << m_DisplayName << ": " << requestString;
  // QDEBUG << m_DisplayName << ": " << "<C:time-range start=\"" +
  // QString::number(m_Year) + monthString + "01T000000Z\" end=\"" +
  // QString::number(m_Year) + monthString + lastDayString + "T235959Z\"/>";

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

  m_pReply =
      m_NetworkManager.sendCustomRequest(request, QByteArray("REPORT"), buffer);

  if (NULL != m_pReply) {
    connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(handleHTTPError()));

    connect(m_pReply, SIGNAL(finished()), this,
            SLOT(handleRequestChangesFinished()));

    m_RequestTimeoutTimer.start(m_RequestTimeoutMS);
  } else {
    QDEBUG << m_DisplayName << ": "
           << "ERROR: Invalid reply pointer when requesting sync token.";
    emit error("Invalid reply pointer when requesting sync token.");
  }
}

/**
 * @brief         State machine implementation of class CalendarClient_CalDAV.
 */

void CalendarClient_CalDAV::setupStateMachine(void) {
  QStateMachine *pStateMachine = new QStateMachine(this);

  QState *pStateWaiting = new QState(pStateMachine);
  QState *pStateRequestingSyncToken = new QState(pStateMachine);
  QState *pStateRequestingChanges = new QState(pStateMachine);
  QState *pStateProcessingChanges = new QState(pStateMachine);
  QState *pStateError = new QState(pStateMachine);

  connect(&m_SynchronizationTimer, SIGNAL(timeout()), this,
          SLOT(debug_handleTimerTimeout()));

  // pStateWaiting
  pStateWaiting->addTransition(&m_SynchronizationTimer, SIGNAL(timeout()),
                               pStateRequestingSyncToken);
  pStateWaiting->addTransition(this, SIGNAL(forceSynchronization()),
                               pStateRequestingSyncToken);
  connect(pStateWaiting, SIGNAL(entered()), this,
          SLOT(handleStateWaitingEntry()));
  connect(pStateWaiting, SIGNAL(exited()), this,
          SLOT(handleStateWaitingExit()));

  // pStateRequestingSyncToken
  pStateRequestingSyncToken->addTransition(
      this, SIGNAL(calendarUpdateRequired()), pStateRequestingChanges);
  pStateRequestingSyncToken->addTransition(
      this, SIGNAL(calendarHasNotChanged()), pStateWaiting);
  pStateRequestingSyncToken->addTransition(this, SIGNAL(error(QString)),
                                           pStateError);
  connect(pStateRequestingSyncToken, SIGNAL(entered()), this,
          SLOT(handleStateRequestingSyncTokenEntry()));
  connect(pStateRequestingSyncToken, SIGNAL(exited()), this,
          SLOT(handleStateRequestingSyncTokenExit()));

  // pStateRequestingChanges
  pStateRequestingChanges->addTransition(this, SIGNAL(calendarUpdateRequired()),
                                         pStateProcessingChanges);
  pStateRequestingChanges->addTransition(this, SIGNAL(calendarHasNotChanged()),
                                         pStateWaiting);
  pStateRequestingChanges->addTransition(this, SIGNAL(eventsUpdated()),
                                         pStateWaiting);
  pStateRequestingChanges->addTransition(this, SIGNAL(error(QString)),
                                         pStateError);
  connect(pStateRequestingChanges, SIGNAL(entered()), this,
          SLOT(handleStateRequestingChangesEntry()));
  connect(pStateRequestingChanges, SIGNAL(exited()), this,
          SLOT(handleStateRequestingChangesExit()));

  // pStateProcessingChanges
  pStateProcessingChanges->addTransition(this, SIGNAL(error(QString)),
                                         pStateError);
  pStateProcessingChanges->addTransition(this, SIGNAL(eventsUpdated()),
                                         pStateWaiting);

  connect(pStateProcessingChanges, SIGNAL(entered()), this,
          SLOT(handleStateProcessingChangesEntry()));
  connect(pStateProcessingChanges, SIGNAL(exited()), this,
          SLOT(handleStateProcessingChangesExit()));

  // pStateError
  pStateError->addTransition(this, SIGNAL(recoverSignal()), pStateWaiting);
  connect(pStateError, SIGNAL(entered()), this, SLOT(handleStateErrorEntry()));

  pStateMachine->setInitialState(pStateWaiting);

  pStateMachine->start();
}

/***** End of: protfunct Protected functions ****************************/ /*@}*/

/******************************************************************************/
/* Private functions                                                          */
/*************************/
/*!@addtogroup privfunct Private functions   */ /*@{*/
/***** End of: privfunct Private functions ******************************/ /*@}*/

/******************************************************************************/
/* Public slots                                                               */
/*************************/
/*!@addtogroup pubslots Public slots         */ /*@{*/
/***** End of: pubslots Public slots ************************************/ /*@}*/

/******************************************************************************/
/* Protected slots                                                            */
/*********************/
/*!@addtogroup protslots Protected slots         */ /*@{*/

void CalendarClient_CalDAV::handleStateWaitingEntry(void) {
  QDEBUG << m_DisplayName << ": "
         << "entering pStateWaiting";
  m_State = E_STATE_IDLE;
  emit syncStateChanged(m_State);

  if ((m_YearToBeRequested != m_Year) || (m_MonthToBeRequested != m_Month)) {
    QDEBUG << m_DisplayName << ": "
           << "year/month has requested from" << m_Year << m_Month << "to"
           << m_YearToBeRequested << m_MonthToBeRequested
           << "=> update required";
    setYear(m_YearToBeRequested);
    setMonth(m_MonthToBeRequested);
    startSynchronization();
  } else if (false != m_bRecoveredFromError) {
    QDEBUG << m_DisplayName << ": "
           << "recovery from ERROR state => update required";
    m_bRecoveredFromError = false;
    startSynchronization();
  }
}

void CalendarClient_CalDAV::handleStateWaitingExit(void) {
  m_State = E_STATE_BUSY;
  emit syncStateChanged(m_State);
  QDEBUG << m_DisplayName << ": "
         << "leaving pStateWaiting";
}

void CalendarClient_CalDAV::handleStateRequestingSyncTokenEntry(void) {
  QDEBUG << m_DisplayName << ": "
         << "entering pStateRequestingSyncToken";
  sendRequestSyncToken();
}

void CalendarClient_CalDAV::handleStateRequestingSyncTokenExit(void) {
  QDEBUG << m_DisplayName << ": "
         << "leaving pStateRequestingSyncToken";
}

void CalendarClient_CalDAV::handleStateRequestingChangesEntry(void) {
  QDEBUG << m_DisplayName << ": "
         << "entering pStateRequestingChanges";
  sendRequestChanges();
}

void CalendarClient_CalDAV::handleStateRequestingChangesExit(void) {
  QDEBUG << m_DisplayName << ": "
         << "leaving pStateRequestingChanges";
}

void CalendarClient_CalDAV::handleStateProcessingChangesEntry(void) {
  QDEBUG << m_DisplayName << ": "
         << "entering pStateProcessingChanges";
}

void CalendarClient_CalDAV::handleStateProcessingChangesExit(void) {
  QDEBUG << m_DisplayName << ": "
         << "leaving pStateProcessingChanges";
}

void CalendarClient_CalDAV::handleStateErrorEntry(void) {
  QDEBUG << m_DisplayName << ": "
         << "entering state error";
}

void CalendarClient_CalDAV::debug_handleTimerTimeout(void) {
  QDEBUG << m_DisplayName << ": "
         << "~~~~~~sync timer timeout~~~~~~";
}
