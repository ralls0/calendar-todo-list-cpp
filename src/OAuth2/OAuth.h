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

private:
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

public slots:
  void onGoogleGranted();

signals:
  void accessTokenChanged(QString accessToken);

public:
  OAuth();
  OAuth(const QString &authUri, const QString &clientId, const QUrl &tokenUri,
        const QString &clientSecret, const quint16 _port, const QString &scope);
  OAuth(const QString &filepath, const QString &scope);
  ~OAuth();
  void parseJson(const QString &filepath);
  void startAuth();

  QString getScope() const;
  void setScope(const QString &scope);
  QUrl getAuthUri() const;
  void setAuthUri(const QUrl &authUri);
  QString getClientId() const;
  void setClientId(const QString &clientId);
  QUrl getTokenUri() const;
  void setTokenUri(const QUrl &tokenUri);
  QString getClientSecret() const;
  void setClientSecret(const QString &clientSecret);
  QUrl getRedirectUri() const;
  void setRedirectUri(const QUrl &redirectUri);
  int getPort() const;
  void setPort(const int port);
  QString getAccessToken() const;
  QString toString();
};

#endif // CALENDAR_TODO_LIST_CPP_OAUTH_H
