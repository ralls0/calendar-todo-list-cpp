/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    GetChangedEvent.cpp
 * @brief
 *
 */

#include "CalendarClient_CalDAV.h"

#define DEBUG_CALENDARCLIENT_CALDAV 1
#if DEBUG_CALENDARCLIENT_CALDAV
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

void CalendarClient_CalDAV::getChangedEvent(void) {
  QDEBUG << "[i] (" << _displayName
         << ") Start getChangedEvent with year: " << _year
         << "month: " << _month;
  QString authorization = "";
  if (_auth == E_AUTH_UPWD) {
    authorization.append("Basic ");
    authorization.append(encodeBase64(_username + ":" + _password));
  } else {
    authorization.append("Bearer ");
    authorization.append(_accessToken);
  }
  QDEBUG << "[i] (" << _displayName << ") Authorization: " << authorization;

  QBuffer *buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);

  QString monthString = QString::number(_month);
  if (monthString.length() < 2)
    monthString.prepend("0");

  QString lastDayString =
      QString::number(DateUtils::lastDayOfMonth(_year, _month));
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
                          QString::number(_year) + monthString +
                          "01T000000Z\" end=\"" + QString::number(_year) +
                          monthString + lastDayString +
                          "T235959Z\"/>\r\n"
                          "     </C:comp-filter>\r\n"
                          "   </C:comp-filter>\r\n"
                          " </C:filter>\r\n"
                          "</C:calendar-query>\r\n";

  QDEBUG << "[i] (" << _displayName << ") "
         << "requesting:";
  QDEBUG << "[i] (" << _displayName << ") " << requestString;
  QDEBUG << "[i] (" << _displayName << ") "
         << "<C:time-range start=\"" + QString::number(_year) + monthString +
                "01T000000Z\" end=\"" + QString::number(_year) + monthString +
                lastDayString + "T235959Z\"/>";

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QString::number(buffersize).toUtf8());

  QNetworkRequest request;
  request.setUrl(_hostURL);
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "1");
  request.setRawHeader("Content-Type", "application/xml; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  _pReply =
      _networkManager.sendCustomRequest(request, QByteArray("REPORT"), buffer);

  if (NULL != _pReply) {
    connect(_pReply, SIGNAL(CalendarClient::error()), this,
            SLOT(CalendarClient_CalDAV::handleHTTPError()));

    connect(_pReply, SIGNAL(finished()), this,
            SLOT(handleRequestChangesFinished()));

    _requestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when requesting sync token.";
    emit error("Invalid reply pointer when requesting sync token.");
  }
}

void CalendarClient_CalDAV::handleRequestChangesFinished(void) {
  _requestTimeoutTimer.stop();

  if (E_STATE_ERROR == _state) {
    QDEBUG << "[i] (" << _displayName << ") "
           << "Error state - aborting";
  }

  QDEBUG << "[i] (" << _displayName << ") "
         << "HTTP RequestChanges finished";

  if (nullptr != _pReply) {
    QDomDocument doc;

    QString reply = _pReply->readAll();
    QDEBUG << _displayName << "Response: " << reply;
    reply = reply.replace("<D:", "<")
                .replace("</D:", "</")
                .replace("<cs:", "<")
                .replace("</cs:", "</")
                .replace("<cal:calendar-data", "<calendardata")
                .replace("</cal:calendar-data", "</calendardata")
                .replace("<caldav:calendar-data", "<calendardata")
                .replace("</caldav:calendar-data", "</calendardata");
    QDEBUG << "[i] (" << _displayName << ") "
           << "Response replaced: " << reply;

    doc.setContent(reply);

    _eventList.clear();

    QDomNodeList list_response = doc.elementsByTagName("response");
    for (int i = 0; i < list_response.size(); i++) {
      QDomNode thisResponse = list_response.item(i);

      QDEBUG << "[i] (" << _displayName << ") "
             << "Response" << i;

      QString sHref = "";
      QString sETag = "";
      QString sPropStatus = "";
      QString strCalendarData = "";

      QDomElement elHref = thisResponse.firstChildElement("href");
      if (!elHref.isNull()) {
        QDEBUG << "[i] (" << _displayName << ") "
               << "  HREF = " << elHref.text();
        sHref = elHref.text();
      } else {
        QDEBUG << "[i] (" << _displayName << ") "
               << "  HREF = ";
      }

      QDomNode thisPropStat = thisResponse.firstChildElement("propstat");
      if (!thisPropStat.isNull()) {

        QDomElement elPropStatus = thisPropStat.firstChildElement("status");
        if (!elPropStatus.isNull()) {
          QDEBUG << "[i] (" << _displayName << ") "
                 << "    PROPSTATUS = " << elPropStatus.text();
          sPropStatus = elPropStatus.text();
        } else {
          QDEBUG << "[i] (" << _displayName << ") "
                 << "    PROPSTATUS = ";
        }

        QDomNode thisProp = thisPropStat.firstChildElement("prop");
        if (!thisProp.isNull()) {
          QDomElement elETag = thisProp.firstChildElement("getetag");
          if (!elETag.isNull()) {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "    ETAG = " << elETag.text();
            sETag = elETag.text();
          } else {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "    ETAG = ";
          }

          QDomElement elCalendarData =
              thisProp.firstChildElement("calendardata");
          if (!elCalendarData.isNull()) {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "    CALENDARDATA = " << elCalendarData.text();

            if (_dataStream) {
              delete _dataStream;
            }
            _dataStream = new QTextStream(elCalendarData.text().toLatin1());

            parseCALENDAR(sHref);

            strCalendarData = elCalendarData.text();
          } else {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "    CALENDARDATA = ";
          }
        }
      }
    }

    int iStatusCode =
        _pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QDEBUG << "[i] (" << _displayName << ") "
           << "Status code:" << iStatusCode;

    if (207 != iStatusCode) {
      QDEBUG << "[i] (" << _displayName << ") "
             << "ERROR: Invalid HTTP return code:" << iStatusCode;
      emit error("Invalid HTTP return code.");
    } else {
      QDEBUG << "[i] (" << _displayName << ") "
             << "OK, restarting synchronization\r\n\r\n";
      _synchronizationTimer.start();
      emit eventsUpdated();
    }

  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when receiving changes.";
    emit error("Invalid reply pointer when receiving changes.");
  }
}