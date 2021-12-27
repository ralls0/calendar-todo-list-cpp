/**
 *
 * @author  Marco Manco
 * @date    27/12/21.
 * @file    ShareCalendar.cpp
 * @brief
 *
 */

#include "ClientCalDAV.h"

void ClientCalDAV::shareCalendar(QString email, QString comment) {
  QDEBUG << "[i] (" << _displayName << ") Sharing calendar" << email;

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

  QBuffer *buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);

  QString requestString = "<D:share-resource xmlns:D=\"DAV:\">\n"
                          "     <D:sharee>\n"
                          "       <D:href>"+email+"</D:href>\n"
                          "       <D:prop>\n"
                          "         <D:displayname> "+_displayName+"</D:displayname>\n"
                          "       </D:prop>\n"
                          "       <D:comment>"+comment+"</D:comment>\n"
                          "       <D:share-access>\n"
                          "         <D:read-write />\n"
                          "       </D:share-access>\n"
                          "     </D:sharee>\n"
                          "   </D:share-resource>";

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QString::number(buffersize).toUtf8());

  QNetworkRequest request;
  request.setUrl(_hostURL);
  request.setRawHeader("User-Agent", "ClientCalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Content-Type", "application/xml; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  _pUploadReply = _uploadNetworkManager.put(request, buffer);

  if (_pUploadReply) {
    connect(_pReply, &QNetworkReply::errorOccurred, this,
            &ClientCalDAV::handleHTTPError);

    connect(_pUploadReply, SIGNAL(finished()), this,
            SLOT(handleShareFinished()));

    _uploadRequestTimeoutTimer.start(_requestTimeoutMS);
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }
}