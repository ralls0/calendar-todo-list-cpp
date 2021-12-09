/**
 *
 * @author  Marco Manco
 * @date    01/12/21.
 * @file    getCTag.cpp
 * @brief   Chiede il CTag per controllare se vi sono modifiche nel calendario
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

void CalendarClient_CalDAV::getCTag(void) {
  QString authorization = "";
  if (_auth == E_AUTH_UPWD) {
    authorization.append("Basic ");
    authorization.append(encodeBase64(_username + ":" + _password));
  } else {
    authorization.append("Bearer ");
    authorization.append(_accessToken);
  }

  QBuffer *buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);

  QString requestString = "<d:propfind xmlns:d=\"DAV:\" "
                          "xmlns:cs=\"http://calendarserver.org/ns/\">\r\n"
                          "  <d:prop>\r\n"
                          "    <d:displayname />\r\n"
                          "    <cs:getctag />\r\n"
                          "  </d:prop>\r\n"
                          "</d:propfind>";

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QString::number(buffersize).toUtf8());

  QNetworkRequest request;
  request.setUrl(_hostURL);
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Content-Type", "application/xml; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  _pReply = _networkManager.sendCustomRequest(request, QByteArray("PROPFIND"),
                                              buffer);

  if (_pReply) {
    connect(_pReply, &QNetworkReply::errorOccurred, this,
            &CalendarClient_CalDAV::handleHTTPError);
    connect(_pReply, SIGNAL(finished()), this,
            SLOT(handleRequestCTagFinished()));

    _requestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when requesting  ctag.";
    emit error("Invalid reply pointer when requesting ctag.");
  }
}

void CalendarClient_CalDAV::handleRequestCTagFinished(void) {
  _requestTimeoutTimer.stop();

  if (E_STATE_ERROR == _state) {
    QDEBUG << "[i] (" << _displayName << ") "
           << "Error state - aborting";
  }

  QDEBUG << "[i] (" << _displayName << ") "
         << "HTTP RequestCTag finished";

  if (_pReply) {
    QDomDocument doc;

    QString reply = _pReply->readAll();
    QDEBUG << "[i] (" << _displayName << ") Response: " << reply;
    reply = reply.replace("<D:", "<")
                .replace("</D:", "</")
                .replace("<cs:", "<")
                .replace("</cs:", "</");
    QDEBUG << "[i] (" << _displayName << ") Response replaced: " << reply;

    doc.setContent(reply);

    QString sDisplayName = "";
    QString sCTag = "";
    QString sStatus = "";

    QDomNodeList response = doc.elementsByTagName("prop");
    QDEBUG << "[i] (" << _displayName << ") "
           << "response.size() = " << response.size();
    for (int i = 0; i < response.size(); i++) {
      QDomNode n = response.item(i);
      QDomElement displayname = n.firstChildElement("displayname");
      if (!displayname.isNull()) {
        QDEBUG << "[i] (" << _displayName << ") "
               << "DISPLAYNAME = " << displayname.text();
        sDisplayName = displayname.text();
      }
      QDomElement ctag = n.firstChildElement("getctag");
      if (!ctag.isNull()) {
        QDEBUG << "[i] (" << _displayName << ") "
               << "CTAG = " << ctag.text();
        sCTag = ctag.text();
      }
    }

    response = doc.elementsByTagName("propstat");
    for (int i = 0; i < response.size(); i++) {
      QDomNode n = response.item(i);
      QDomElement status = n.firstChildElement("status");
      if (!status.isNull()) {
        QDEBUG << "[i] (" << _displayName << ") "
               << "STATUS = " << status.text();
        sStatus = status.text();
      }
    }

    if ((!sCTag.isEmpty()) && (sStatus.endsWith("200 OK"))) {
      bool bDisplayNameChanged = (_displayName != sDisplayName);
      bool bCTagChanged = (_cTag != sCTag);

      _displayName = sDisplayName;
      _cTag = sCTag;

      if (bDisplayNameChanged) {
        emit displayNameChanged(_displayName);
      }

      if (bCTagChanged) {
        QDEBUG << "[i] (" << _displayName << ") "
               << "ctag has changed";
        emit calendarUpdateRequired();
      }
    } else {
      QDEBUG << "[i] (" << _displayName << ") "
             << "ERROR: Receiving ctag failed. Status:" << sStatus;
      emit error("Receiving ctag failed.");
    }

    QDEBUG << "[i] (" << _displayName << ") "
           << "\r\nHeaders:" << _pReply->rawHeaderList() << "\r\n";
    if (_pReply->hasRawHeader("DAV")) {
      QDEBUG << "[i] (" << _displayName << ") "
             << "DAV:" << _pReply->rawHeader("DAV");
    }
    if (_pReply->hasRawHeader("Server")) {
      QDEBUG << "[i] (" << _displayName << ") "
             << "Server:" << _pReply->rawHeader("Server");
    }
    QDEBUG << "[i] (" << _displayName << ") "
           << "Status code:"
           << _pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when receiving ctag.";
    emit error("Invalid reply pointer when receiving ctag.");
  }
}
