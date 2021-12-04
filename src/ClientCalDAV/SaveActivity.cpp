/**
 *
 * @author  Marco Manco
 * @date    03/12/21.
 * @file    SaveActivity.cpp
 * @brief   Salva un attivita' nel calendario
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

void CalendarClient_CalDAV::saveActivity(QString uid, QString filename,
                                         QString summary, QString description,
                                         QDateTime startDateTime,
                                         QDateTime endDateTime) {
  QDEBUG << "[i] (" << _displayName << ") "
         << "saving event" << summary;

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
      "DESCRIPTION:" +
      description +
      "\r\n"
      "TRANSP:OPAQUE\r\n";

  requestString.append("END:VEVENT\r\nEND:VCALENDAR");

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QString::number(buffersize).toUtf8());

  QNetworkRequest request;
  request.setUrl(QUrl(_hostURL.toString() + filename));
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
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
            &CalendarClient_CalDAV::handleHTTPError);

    connect(_pUploadReply, SIGNAL(finished()), this,
            SLOT(handleUploadFinished()));

    _uploadRequestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }
}
