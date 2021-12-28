/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    OAuth.h
 * @brief   OAuth class used for Google OAuth 2.0
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_OAUTH_H
#define CALENDAR_TODO_LIST_CPP_OAUTH_H

#include <QDebug>
#include <QDesktopServices>
#include <QtNetwork>
#include <QtNetworkAuth>
#include <fstream>
#include <iostream>

#define DEBUG_ 1
#if DEBUG_
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

/**
 * QOAuth2AuthorizationCodeFlow object use scope need to access from the
 * application. for calendar:
 * [Calendar](https://www.googleapis.com/auth/calendar)
 *
 * To get a list of scope supported by Google APIs see the upstream:
 * [OAuth 2.0 Scopes for Google APIs
 * ](https://developers.google.com/identity/protocols/oauth2/scopes)
 */

class OAuth : public QObject {
  Q_OBJECT

public slots:
  void onGoogleGranted(void);
  void onGoogleError(void);
  void startAuth(void);

signals:
  void accessTokenTimeout(void);
  void accessTokenChanged(QString accessToken);
  void dataParsed(void);

public:
  OAuth();
  OAuth(const QString &filepath, const QString &scope);
  ~OAuth();

  QString toString();

private:
  void parseJson(const QString &filepath);

  QOAuth2AuthorizationCodeFlow *_google;
  QString _scope;
  QUrl _authUri;
  QString _clientId;
  QUrl _tokenUri;
  QString _clientSecret;
  QUrl _redirectUri;
  quint16 _port;
  QJsonDocument _document;
  QString _accessToken;

  QTimer _requestTimeoutTimer;
};

#endif // CALENDAR_TODO_LIST_CPP_OAUTH_H
