/**
 *
 * @author  Marco Manco
 * @date    01/12/21.
 * @file    getCTag.cpp
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

  if (NULL != _pReply) {
    connect(_pReply, SIGNAL(finished()), this,
            SLOT(handleRequestCTagFinished()));

    _requestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << _displayName << ": "
           << "ERROR: Invalid reply pointer when requesting  ctag.";
    emit error("Invalid reply pointer when requesting ctag.");
  }
}
