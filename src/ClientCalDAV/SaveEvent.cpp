/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    SaveEvent.cpp
 * @brief   Salva un evento nel calendario
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

void CalendarClient_CalDAV::saveEvent(QString uid, QString filename,
                                      QString summary, QString location,
                                      QString description, QString rrule,
                                      QString exdate, QDateTime startDateTime,
                                      QDateTime endDateTime) {
  QDEBUG << _displayName << ": "
         << "saving event" << summary;

  if (NULL != _pUploadReply) {
    QDEBUG << _displayName << ": "
           << "cleaning up _pUploadReply";
    _pUploadReply->abort();
    _pUploadReply->disconnect();
  }

  std::cout << "non sono abbortito\n";
  QString authorization = "Basic ";
  authorization.append(encodeBase64(_username + ":" + _password));

  QBuffer *buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);

  if (uid.isEmpty()) {
    std::cout << "uid empty\n";
    uid = QDateTime::currentDateTime().toString("yyyyMMdd-HHMM-00ss") +
          "-0000-" + startDateTime.toString("yyyyMMddHHMM");
  }

  if (filename.isEmpty()) {
    std::cout << "filename empty\n";
    filename = uid + ".ics";
  }

  std::cout << "creo stringa di richiesta\n";
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
    std::cout << "rrule non e' empty\n";
    requestString.append("RRULE:" + rrule + "\r\n");
  }

  if (!exdate.isEmpty()) {
    std::cout << "exdata non e' empty\n";
    requestString.append("EXDATE:" + exdate + "\r\n");
  }

  std::cout << "appendo la fine\n";
  requestString.append("END:VEVENT\r\nEND:VCALENDAR");

  std::cout << "prendo la dime del buffer\n";
  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QString::number(buffersize).toUtf8());

  std::cout << "creo richiesta header\n";
  QNetworkRequest request;
  request.setUrl(QUrl(_hostURL.toString() + filename));
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Prefer", "return-minimal");
  request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  std::cout << "start ssl config\n";
  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  std::cout << "put _pUploadReply\n";
  _pUploadReply = _uploadNetworkManager.put(request, buffer);

  if (NULL != _pUploadReply) {
    //    connect(_pUploadReply, SIGNAL(error(QNetworkReply::NetworkError)),
    //    this,
    //            SLOT(handleUploadHTTPError()));

    connect(_pUploadReply, SIGNAL(finished()), this,
            SLOT(handleUploadFinished()));

    _uploadRequestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << _displayName << ": "
           << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }
}
