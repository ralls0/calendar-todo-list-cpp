/**
 * @author      Created by Marco Manco.
 * @file        OAuth.cpp
 *
 * @brief       OAuth class used for Google OAuth 2.0
 */


#include "OAuth.h"

OAuth::OAuth(): _isEmpty(true) {
    _google = new QOAuth2AuthorizationCodeFlow();
}

OAuth::~OAuth() {
    delete _google;
}

OAuth::OAuth(const QString &scope): _scope(scope), _isEmpty(true) {
    _google = new QOAuth2AuthorizationCodeFlow();
    _google->setScope(_scope);
}

OAuth::OAuth(const QString &filepath, const QString &scope): _scope(scope), _isEmpty(true) {
    _google = new QOAuth2AuthorizationCodeFlow(this);
    _google->setScope(_scope);

    parseJson(filepath);
}

QString OAuth::getScope() const{
    return _scope;
}

void OAuth::setScope(const QString &scope) {
    _scope = scope;
}

QUrl OAuth::getAuthUri() const{
    return _authUri;
}

void OAuth::setAuthUri(const QUrl &authUri) {
    _authUri = authUri;
}

QString OAuth::getClientId() const{
    return _clientId;
}

void OAuth::setClientId(const QString &clientId) {
    _clientId = clientId;
}

QUrl OAuth::getTokenUri() const{
    return _tokenUri;
}

void OAuth::setTokenUri(const QUrl &tokenUri) {
    _tokenUri = tokenUri;
}

QString OAuth::getClientSecret() const{
    return _clientSecret;
}

void OAuth::setClientSecret(const QString &clientSecret) {
    _clientSecret = clientSecret;
}

QUrl OAuth::getRedirectUri() const{
    return _redirectUri;
}

void OAuth::setRedirectUri(const QUrl &redirectUri) {
    _redirectUri = redirectUri;
}

int OAuth::getPort() const{
    return _port;
}

void OAuth::setPort(const int port) {
    _port = port;
}

QString OAuth::getAccessToken() const{
    return _accessToken;
}

bool OAuth::parseJson (const QString &filepath) {

    std::cout<<"Opening file: "<<filepath.toStdString()<<"\n";
    QFile file;
    file.setFileName(filepath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        std::cerr<<"[e] Error: File not open\n"<<file.errorString().toStdString()<<"\n";
        exit(-1);
    }

    /*
    * Parse the JSON to get the settings and information needed.
    */
    _document = QJsonDocument::fromJson(file.readAll());
    const auto object = _document.object();
    const auto settingsObject = object["web"].toObject();
    _authUri = settingsObject["auth_uri"].toString();
    _clientId = settingsObject["client_id"].toString();
    _tokenUri = settingsObject["token_uri"].toString();
    _clientSecret = settingsObject["client_secret"].toString();
    const auto redirectUris = settingsObject["redirect_uris"].toArray();

    _redirectUri = redirectUris[0].toString(); // Get the first URI
    _port = static_cast<quint16>(_redirectUri.port()); // Get the port
    _isEmpty = false;

    _google->setAuthorizationUrl(_authUri);
    _google->setClientIdentifier(_clientId);
    _google->setAccessTokenUrl(_tokenUri);
    _google->setClientIdentifierSharedKey(_clientSecret);

    _google->setModifyParametersFunction([](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
        // Percent-decode the "code" parameter so Google can match it
        if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
            QByteArray code = parameters->value("code").toByteArray();
            (*parameters).replace("code", QUrl::fromPercentEncoding(code));
        }
    });

    std::cout<<"Auth url: "<<_google->authorizationUrl().toString().toStdString()<<"\n";

    /*
     * Create and assign a QOAuthHttpServerReplyHandler as the reply handler
     * of the QOAuth2AuthorizationCodeFlow object
     */
    auto replyHandler = new QOAuthHttpServerReplyHandler(_port, this);
    _google->setReplyHandler(replyHandler);

    /*
     * Connect the authorizeWithBrowser signal to the QDesktopServices::openUrl
     * function to open an external browser to complete the authorization.
     */
    connect(_google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);
    connect(_google, &QOAuth2AuthorizationCodeFlow::granted,
            this, &OAuth::onGoogleGranted);

    return true;
}

void OAuth::startAuth () {
    if(!_google) {
        std::cerr<<"[e] Error: _google variable was null when startAuth was called\n";
        exit(-1);
    }
    _google->grant();
}

bool OAuth::isEmpty() const{
    return _scope.isEmpty() && _authUri.isEmpty() && _tokenUri.isEmpty() &&
            _clientId.isEmpty() && _clientSecret.isEmpty() && _redirectUri.isEmpty();
}

void OAuth::onGoogleGranted() {
    _accessToken = _google->token();
    std::cout<<"OAuth Token: "<<_accessToken.toStdString()<<"\n";
    emit accessTokenChanged(_accessToken);
}

QString OAuth::toString() {
    QString str = "OAuth setup";
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
