/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    GetChangedEvent.cpp
 * @brief   Invia una richiesta per recuperare tutti gli eventi del calendario
 * da una certa start_date fino ad una end_date
 *
 */

#include "ClientCalDAV.h"

void ClientCalDAV::retrieveChangedEvent(void) {
  QDEBUG << "[i] (" << _displayName
         << ") Start getChangedEvent with year: " << _year
         << "month: " << _month;
  QString authorization = "";
  if (_auth == E_AUTH_TOKEN) {
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
  request.setRawHeader("User-Agent", "ClientCalDAV");
  if (_auth == E_AUTH_TOKEN) {
    request.setRawHeader("Authorization", authorization.toUtf8());
  }

  request.setRawHeader("Depth", "1");
  request.setRawHeader("Content-Type", "application/xml; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  _pReply =
      _networkManager.sendCustomRequest(request, QByteArray("REPORT"), buffer);

  if (_pReply) {
    connect(_pReply, &QNetworkReply::errorOccurred, this,
            &ClientCalDAV::handleHTTPError);

    connect(_pReply, SIGNAL(finished()), this,
            SLOT(handleRequestChangesEventFinished()));

    connect(&_networkManager, &QNetworkAccessManager::authenticationRequired,
            this, &ClientCalDAV::handleRequestAuthentication);

    _requestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when requesting sync token.";
    emit error("Invalid reply pointer when requesting sync token.");
  }
}

void ClientCalDAV::handleRequestChangesEventFinished(void) {
  _requestTimeoutTimer.stop();

  bool changes = false;
  if (E_STATE_ERROR == _state) {
    QDEBUG << "[i] (" << _displayName << ") "
           << "Error state - aborting";
  }

  QDEBUG << "[i] (" << _displayName << ")  HTTP RequestChangesEvent finished";

  if (nullptr != _pReply) {
    QDomDocument doc;

    QString reply = _pReply->readAll();
    QDEBUG << "[i] (" << _displayName << ") Response: " << reply;
    reply = reply.replace("<D:", "<")
                .replace("</D:", "</")
                .replace("<d:", "<")
                .replace("</d:", "</")
                .replace("<cs:", "<")
                .replace("</cs:", "</")
                .replace("<cal:calendar-data", "<calendardata")
                .replace("</cal:calendar-data", "</calendardata")
                .replace("<caldav:calendar-data", "<calendardata")
                .replace("</caldav:calendar-data", "</calendardata");
    QDEBUG << "[i] (" << _displayName << ") Response replaced: " << reply;

    doc.setContent(reply);

    _eventList.clear();

    QDomNodeList list_response = doc.elementsByTagName("response");
    QDEBUG << "[i] (" << _displayName << ") Response amount:" << list_response.size();
    for (int i = 0; i < list_response.size(); i++) {
      QDomNode thisResponse = list_response.item(i);

      QDEBUG << "[i] (" << _displayName << ") Event number:" << i;

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
                   << "    ETAG = empty";
          }

          QDomElement elCalendarData =
              thisProp.firstChildElement("calendardata");
          if (!elCalendarData.isNull()) {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "    CALENDARDATA = " << elCalendarData.text();

            submit(sHref, std::move(new QTextStream(
                              elCalendarData.text().toLatin1())));

            strCalendarData = elCalendarData.text();
          } else {
            QDEBUG << "[i] (" << _displayName << ") "
                   << "    CALENDARDATA = ";
          }
        }
      }
      changes = true;
    }

    int iStatusCode =
        _pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QDEBUG << "[i] (" << _displayName << ") Status code:" << iStatusCode;

    if (207 != iStatusCode) {
      QDEBUG << "[i] (" << _displayName << ") "
             << "ERROR: Invalid HTTP return code:" << iStatusCode;
      emit error("Invalid HTTP return code.");
    } else {
      if (!changes) {
        emit noEventsUpdated();
      }
      if (!_synchronizationTimer.isActive()) {
        QDEBUG << "[i] (" << _displayName << ") Restarting synchronization";
        _synchronizationTimer.start();
      }
    }

  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when receiving changes.";
    emit error("Invalid reply pointer when receiving changes.");
  }
}