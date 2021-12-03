/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    DeleteEvent.cpp
 * @brief   Delete an event from the calendar
 *
 */

#include "CalendarClient_CalDAV.h"

#define DEBUG_CALENDARCLIENT_CALDAV 0
#if DEBUG_CALENDARCLIENT_CALDAV
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

void CalendarClient_CalDAV::deleteEvent(QString href) {
  if (href.isEmpty()) {
    return;
  }

  QDEBUG << _displayName << ": "
         << "deleting event with HREF" << href;

  if (NULL != _pUploadReply) {
    QDEBUG << _displayName << ": "
           << "cleaning up _pUploadReply";
    _pUploadReply->abort();
    _pUploadReply->disconnect();
  }

  QString authorization = "";
  if (_auth == E_AUTH_UPWD) {
    authorization.append("Basic ");
    authorization.append(encodeBase64(_username + ":" + _password));
  } else {
    authorization.append("Bearer ");
    authorization.append(_accessToken);
  }

  QString filename = QUrl(href).fileName();

  QNetworkRequest request;
  request.setUrl(QUrl(_hostURL.toString() + filename));
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Prefer", "return-minimal");
  request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
  request.setRawHeader("Content-Length", 0);

  QDEBUG << _displayName << ": "
         << "deleting" << request.url();

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  _pUploadReply = _uploadNetworkManager.deleteResource(request);

  if (NULL != _pUploadReply) {
    connect(_pReply, SIGNAL(CalendarClient::error()), this,
            SLOT(CalendarClient_CalDAV::handleHTTPError()));

    connect(_pUploadReply, SIGNAL(finished()), this,
            SLOT(handleUploadFinished()));

    _uploadRequestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << _displayName << ": "
           << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }
}

void CalendarClient_CalDAV::handleUploadFinished(void) {
  _uploadRequestTimeoutTimer.stop();

  QDEBUG << _displayName << ": "
         << "HTTP upload finished";

  if (nullptr != _pUploadReply) {
    std::cout << _displayName.toStdString() << ": "
              << "received:\r\n"
              << _pUploadReply->readAll().toStdString();
    emit forceSynchronization();
  }
}