/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    OAuth.cpp
 * @brief    OAuth class used for Google OAuth 2.0
 *
 */

#include "OAuth.h"

#define DEBUG_OAUTH 1
#if DEBUG_OAUTH
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

OAuth::OAuth() {
  _google = new QOAuth2AuthorizationCodeFlow;
  _requestTimeoutTimer.setSingleShot(true);
}

OAuth::OAuth(const QString &filepath, const QString &scope) : _scope(scope) {
  _google = new QOAuth2AuthorizationCodeFlow;

  _requestTimeoutTimer.setSingleShot(true);

  _google->setScope(_scope);

  parseJson(filepath);

  _google->setAuthorizationUrl(_authUri);
  _google->setClientIdentifier(_clientId);
  _google->setAccessTokenUrl(_tokenUri);
  _google->setClientIdentifierSharedKey(_clientSecret);

  _google->setModifyParametersFunction(
      [](QAbstractOAuth::Stage stage,
         QMultiMap<QString, QVariant> *parameters) {
        // Percent-decode the "code" parameter so Google can match it
        if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
          QByteArray code = parameters->value("code").toByteArray();
          (*parameters).replace("code", QUrl::fromPercentEncoding(code));
        }
      });

  /*
   * Create and assign a QOAuthHttpServerReplyHandler as the reply handler
   * of the QOAuth2AuthorizationCodeFlow object
   */
  QDEBUG << "[i] QOAuthHttpServerReplyHandler port: " << _port;
  auto replyHandler = new QOAuthHttpServerReplyHandler(_port);
  _google->setReplyHandler(replyHandler);

  /*
   * Connect the authorizeWithBrowser signal to the QDesktopServices::openUrl
   * function to open an external browser to complete the authorization.
   */
  connect(_google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
          &QDesktopServices::openUrl);
  connect(_google, &QOAuth2AuthorizationCodeFlow::granted, this,
          &OAuth::onGoogleGranted);
  connect(this, &OAuth::dataParsed, this, &OAuth::startAuth);
  emit dataParsed();
}

OAuth::~OAuth() { delete _google; }

void OAuth::parseJson(const QString &filepath) {

  QDEBUG << "[i] Opening file: " << filepath << "\n";
  QFile file;
  file.setFileName(filepath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QDEBUG << " [e] Error: File not open\n" << file.errorString() << "\n";
    exit(-1);
  }

  /*
   * Parse the JSON to get the settings and information needed.
   */
  QDEBUG << "[i] Parse the JSON to get the settings and information needed\n";
  _document = QJsonDocument::fromJson(file.readAll());
  const auto object = _document.object();
  const auto settingsObject = object["web"].toObject();
  _authUri = settingsObject["auth_uri"].toString();
  _clientId = settingsObject["client_id"].toString();
  _tokenUri = settingsObject["token_uri"].toString();
  _clientSecret = settingsObject["client_secret"].toString();
  const auto redirectUris = settingsObject["redirect_uris"].toArray();

  _redirectUri = redirectUris[0].toString();         // Get the first URI
  _port = static_cast<quint16>(_redirectUri.port()); // Get the port
  QDEBUG << "[i] Port: " << _port;
}

void OAuth::startAuth() {
  if (!_google) {
    QDEBUG
        << "[e] Error: _google variable was null when startAuth was called\n";
    exit(-1);
  }
  QDEBUG << "[i] Grant...\n";
  connect(&_requestTimeoutTimer, &QTimer::timeout, this, &OAuth::onGoogleError);
  _requestTimeoutTimer.setInterval(16000);
  _requestTimeoutTimer.start();
  _google->grant();
}

void OAuth::onGoogleGranted() {
  _requestTimeoutTimer.stop();
  _accessToken = _google->token();
  QDEBUG << "[i] Token: " << _accessToken << "\n";
  emit accessTokenChanged(_accessToken);
}

void OAuth::onGoogleError() {
  QDEBUG << "[e] Error access token timeout";
  emit accessTokenTimeout();
}

QString OAuth::toString() {
  QString str = "\nOAuth setup";
  str.append("\nScope: ");
  str.append(_scope);
  str.append("\nAuthUri: ");
  str.append(_authUri.toString());
  str.append("\nClientId: ");
  str.append(_clientId);
  str.append("\nTokenUri: ");
  str.append(_tokenUri.toString());
  str.append("\nClientSecret: ");
  str.append(_clientSecret);
  str.append("\nRedirectUri: ");
  str.append(_redirectUri.toString());
  str.append("\nPort: ");
  str.append(QString::number(_port));
  str.append("\n");

  return str;
}
