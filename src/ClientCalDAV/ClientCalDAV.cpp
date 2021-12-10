/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    ClientCalDAV.cpp
 * @brief   File di implementazione della classe ClientCalDAV
 *
 */

#include "ClientCalDAV.h"

ClientCalDAV::ClientCalDAV(const QString &username, const QString &password,
                           const QString &hostURL, const QString &displayName,
                           QObject *parent)
    : QObject(parent), _hostURL(hostURL), _displayName(displayName),
      _username(username), _password(password) {
  _requestTimeoutMS = 32000;
  _requestTimeoutTimer.setSingleShot(true);

  /*
   * Il timer è impostato su singolo scatto,
   * quindi non è necessario interromperlo
   * nel caso in cui la sincronizzazione non sia riuscita
   */
  _synchronizationTimer.setSingleShot(true);
  _synchronizationTimer.setInterval(64000);

  // Creazione di un colore generato in modo randomico
  _color = QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF).name();

  _auth = E_AUTH_UPWD;
  _dataStream = nullptr;
  _pUploadReply = nullptr;
  _pReply = nullptr;
  _au = nullptr;

  _accessToken = "";
  _filepath = "";

  _cTag = "0";
  _year = QDate::currentDate().year();
  _month = QDate::currentDate().month();
  _yearToBeRequested = QDate::currentDate().year();
  ;
  _monthToBeRequested = QDate::currentDate().month();
  lastSyncYear = -1;
  lastSyncMonth = -1;
  _bRecoveredFromError = false;

  setupStateMachine();

  emit calendarUpdateRequired();
  // retrieveChangedEvent(); FIXME: Se l'emit non funziona
}

ClientCalDAV::ClientCalDAV(const QString &filepath, const QString &hostURL,
                           const QString &displayName, QObject *parent)
    : QObject(parent), _hostURL(hostURL), _displayName(displayName),
      _filepath(filepath) {

  _requestTimeoutMS = 32000;
  _requestTimeoutTimer.setSingleShot(true);

  /*
   * Il timer è impostato su singolo scatto,
   * quindi non è necessario interromperlo
   * nel caso in cui la sincronizzazione non sia riuscita
   */
  _synchronizationTimer.setSingleShot(true);
  _synchronizationTimer.setInterval(64000);

  // Creazione di un colore generato in modo randomico
  _color = QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF).name();

  _auth = E_AUTH_TOKEN;
  _dataStream = nullptr;
  _pUploadReply = nullptr;
  _pReply = nullptr;
  _accessToken = "";

  _au = new (std::nothrow) OAuth(
      filepath,
      "https://www.googleapis.com/auth/calendar"); // FIXME IN CASO DI ERROR PER
                                                   // IL TODO FIX LO SCOPE
  connect(_au, &OAuth::accessTokenChanged, this, &ClientCalDAV::setAccessToken);

  _username = "";
  _password = "";
  _cTag = "0";
  _year = QDate::currentDate().year();
  _month = QDate::currentDate().month();
  _yearToBeRequested = QDate::currentDate().year();
  ;
  _monthToBeRequested = QDate::currentDate().month();
  lastSyncYear = -1;
  lastSyncMonth = -1;
  _bRecoveredFromError = false;

  setupStateMachine();

  emit calendarUpdateRequired();
  // retrieveChangedEvent(); FIXME: Se l'emit non funziona
}

ClientCalDAV::~ClientCalDAV() {
  _eventList.clear();
  _synchronizationTimer.stop();

  if (_au)
    delete _au;
}

int ClientCalDAV::getRequestTimeoutMS(void) const { return _requestTimeoutMS; }

void ClientCalDAV::setRequestTimeoutMS(const int requestTimeoutMS) {
  if (requestTimeoutMS > 0) {
    _requestTimeoutMS = requestTimeoutMS;
    emit requestTimeoutMSChanged(_requestTimeoutMS);
  }
}

QString ClientCalDAV::encodeBase64(QString string) {
  QByteArray ba;
  ba.append(string.toUtf8());
  return ba.toBase64();
}

QString ClientCalDAV::ascii2utf8(QString str) {
  return QString::fromUtf8(str.toLocal8Bit().data());
}

ClientCalDAV::E_CalendarState ClientCalDAV::getSyncState(void) {
  return _state;
}

QString ClientCalDAV::getColor(void) const { return _color; }

void ClientCalDAV::setColor(const QString &color) {
  if (color != _color) {
    _color = color;
    for (int i = 0; i < _eventList.count(); i++) {
      CalendarEvent *pEvent = &(_eventList[i]);
      pEvent->setColor(_color);
    }
    emit colorChanged(_color);
  }
}

bool ClientCalDAV::setHostURL(const QUrl hostURL) {
  bool bRet = false;

  if (hostURL.isValid()) {
    _hostURL = hostURL;
    bRet = true;
    emit hostURLChanged(_hostURL.toString());
  }

  return bRet;
}

QString ClientCalDAV::getHostURL(void) const { return _hostURL.toString(); }

QString ClientCalDAV::getDisplayName(void) const { return _displayName; }

void ClientCalDAV::setDisplayName(QString name) {
  _displayName = name;
  emit displayNameChanged(_displayName);
}


int ClientCalDAV::getYear() const { return _year; }

int ClientCalDAV::getMonth() const { return _month; }

void ClientCalDAV::setAccessToken(QString accessToken) {
  _accessToken = accessToken;
  emit accessTokenChanged(_accessToken);
}

void ClientCalDAV::setFilePath(QString filepath) {
  _filepath = filepath;
  emit filePathChanged(_filepath);
}

QString ClientCalDAV::getAccessToken() const { return _accessToken; }
QString ClientCalDAV::getFilePath() const { return _filepath; }

QString ClientCalDAV::getCTag() const { return _cTag; }

void ClientCalDAV::setUsername(const QString username) {
  _username = username;
  emit usernameChanged(_username);
}

QString ClientCalDAV::getUsername(void) const { return _username; }

void ClientCalDAV::setPassword(const QString password) {
  _password = password;
  emit passwordChanged(_password);
}

QString ClientCalDAV::getPassword(void) const { return _password; }

void ClientCalDAV::setYear(const int &year) {
  if (E_STATE_IDLE == _state) {
    if (_year != year) {
      QDEBUG << "[i] (" << _displayName << ") "
             << "Year changed from" << _year << "to" << year;
      _year = year;
      emit yearChanged(_year);
      _yearToBeRequested = year;
      startSynchronization();
    }
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "requested Year changed from" << _yearToBeRequested << "to"
           << year;
    _yearToBeRequested = year;
  }
}

void ClientCalDAV::setMonth(const int &month) {
  if (E_STATE_IDLE == _state) {
    if (_month != month) {
      QDEBUG << "[i] (" << _displayName << ") "
             << "Month changed from" << _month << "to" << month;
      _month = month;
      emit monthChanged(_month);
      _monthToBeRequested = month;
      startSynchronization();
    }
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "requested Month changed from" << _monthToBeRequested << "to"
           << month;
    _monthToBeRequested = month;
  }
}

ClientCalDAV::E_CalendarAuth
ClientCalDAV::getClientAuth(void) const {
  return _auth;
}
