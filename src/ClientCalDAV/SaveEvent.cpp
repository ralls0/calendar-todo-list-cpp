/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    SaveEvent.cpp
 * @brief   Salva un evento nel calendario
 *
 */

#include "ClientCalDAV.h"

void ClientCalDAV::saveEvent(QString uid, QString filename, QString summary,
                             QString location, QString description,
                             QString rrule, QDateTime startDateTime,
                             QDateTime endDateTime) {
  QDEBUG << "[i] (" << _displayName << ") Saving event" << summary;

  if (NULL != _pUploadReply) {
    QDEBUG << "[i] (" << _displayName << ") "
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

  QDEBUG << "[i] (" << _displayName << ") "
         << "auth" << authorization;

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

  requestString.append("END:VEVENT\r\nEND:VCALENDAR");

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QString::number(buffersize).toUtf8());

  QNetworkRequest request;
  request.setUrl(QUrl(_hostURL.toString() + filename));
  request.setRawHeader("User-Agent", "ClientCalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Prefer", "return-minimal");
  request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  _pUploadReply = _uploadNetworkManager.put(request, buffer);

  if (_pUploadReply) {
    connect(_pReply, &QNetworkReply::errorOccurred, this,
            &ClientCalDAV::handleHTTPError);

    connect(_pUploadReply, SIGNAL(finished()), this,
            SLOT(handleUploadFinished()));

    _uploadRequestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }
}
