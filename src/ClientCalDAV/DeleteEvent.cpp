/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    DeleteEvent.cpp
 * @brief   Delete an event from the calendar
 *
 */

#include "ClientCalDAV.h"

void ClientCalDAV::deleteEvent(QString href) {
  // href="/caldav/v2/pdsmariorossi@gmail.com/events/20211218-1212-0009-0000-202112181212.ics";
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
  if (_auth == E_AUTH_TOKEN) {
      authorization.append("Bearer ");
      authorization.append(_accessToken);
  }

  QString filename = QUrl(href).fileName();

  QNetworkRequest request;
  request.setUrl(QUrl(_hostURL.toString() + filename));
  request.setRawHeader("User-Agent", "ClientCalDAV");
  if (_auth == E_AUTH_TOKEN) {
        request.setRawHeader("Authorization", authorization.toUtf8());
    }

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

  if (_pUploadReply) {
    connect(_pReply, &QNetworkReply::errorOccurred, this,
            &ClientCalDAV::handleHTTPError);
      connect(&_uploadNetworkManager, &QNetworkAccessManager::authenticationRequired, this, &ClientCalDAV::handleRequestAuthentication);

    connect(_pUploadReply, SIGNAL(finished()), this,
            SLOT(handleUploadFinished()));

    _uploadRequestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << _displayName << ": "
           << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }
}

void ClientCalDAV::handleUploadFinished(void) {
  _uploadRequestTimeoutTimer.stop();

  QDEBUG << _displayName << ": "
         << "HTTP upload finished";

  if (_pUploadReply) {
    std::cout << _displayName.toStdString() << ": "
              << "received:\r\n"
              << _pUploadReply->readAll().toStdString();
    emit forceSynchronization();
  }
}
