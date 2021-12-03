/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    CalendarClient_CalDAV.cpp
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

CalendarClient_CalDAV::CalendarClient_CalDAV(QObject *parent)
    : CalendarClient(parent) {
  _auth = E_AUTH_UPWD;
  _dataStream = nullptr;
  _pUploadReply = nullptr;
  _au = nullptr;

  _username = "";
  _password = "";
  _hostURL = "";
  _displayName = "";
  _cTag = "";
  _year = 1;
  _month = 1;
  _yearToBeRequested = QDate::currentDate().year();
  ;
  _monthToBeRequested = QDate::currentDate().month();
  lastSyncYear = -1;
  lastSyncMonth = -1;
  _bRecoveredFromError = false;

  setupStateMachine();
}

CalendarClient_CalDAV::CalendarClient_CalDAV(const QString &username,
                                             const QString &password,
                                             const QString &hostURL,
                                             const QString &displayName,
                                             QObject *parent)
    : CalendarClient(hostURL, displayName, parent), _username(username),
      _password(password) {
  _auth = E_AUTH_UPWD;
  _dataStream = nullptr;
  _pUploadReply = nullptr;
  _au = nullptr;

  _cTag = "";
  _year = 1;
  _month = 1;
  _yearToBeRequested = QDate::currentDate().year();
  ;
  _monthToBeRequested = QDate::currentDate().month();
  lastSyncYear = -1;
  lastSyncMonth = -1;
  _bRecoveredFromError = false;

  setupStateMachine();
}

CalendarClient_CalDAV::CalendarClient_CalDAV(
    const QString &filepath, const QString &hostURL, const QString &displayName,
    const QString &scope, const QString &username, QObject *parent)
    : CalendarClient(hostURL, displayName, parent) {
  _auth = E_AUTH_TOKEN;
  _dataStream = nullptr;
  _pUploadReply = nullptr;
  _au = new (std::nothrow) OAuth(filepath, scope);
  connect(_au, &OAuth::accessTokenChanged, this,
          &CalendarClient_CalDAV::setAccessToken);

  _username = "";
  _password = "";
  _cTag = "";
  _year = 1;
  _month = 1;
  _yearToBeRequested = QDate::currentDate().year();
  ;
  _monthToBeRequested = QDate::currentDate().month();
  lastSyncYear = -1;
  lastSyncMonth = -1;
  _bRecoveredFromError = false;

  setupStateMachine();
}

CalendarClient_CalDAV::~CalendarClient_CalDAV() {
  _synchronizationTimer.stop();
}

int CalendarClient_CalDAV::getYear() const { return _year; }

int CalendarClient_CalDAV::getMonth() const { return _month; }

void CalendarClient_CalDAV::setAccessToken(QString accessToken) {
  _accessToken = accessToken;
  emit accessTokenChanged(_accessToken);
}

QString CalendarClient_CalDAV::getAccessToken() const { return _accessToken; }

void CalendarClient_CalDAV::setUsername(const QString username) {
  _username = username;
  emit usernameChanged(_username);
}

QString CalendarClient_CalDAV::getUsername(void) const { return _username; }

void CalendarClient_CalDAV::setPassword(const QString password) {
  _password = password;
  emit passwordChanged(_password);
}

QString CalendarClient_CalDAV::getPassword(void) const { return _password; }

void CalendarClient_CalDAV::startSynchronization(void) {
  QDEBUG << _displayName << ": "
         << "!!!forcing synchronization!!!";
  emit forceSynchronization();
}

void CalendarClient_CalDAV::stopSynchronization(void) {}

void CalendarClient_CalDAV::recover(void) {
  QDEBUG << _displayName << ": "
         << "trying to recover from EEROR state";
  _bRecoveredFromError = true;
  emit recoverSignal();
}

void CalendarClient_CalDAV::setYear(const int &year) {
  if (E_STATE_IDLE == _state) {
    if (_year != year) {
      QDEBUG << _displayName << ": "
             << "Year changed from" << _year << "to" << year;
      _year = year;
      emit yearChanged(_year);
      _yearToBeRequested = year;
      startSynchronization();
    }
  } else {
    QDEBUG << _displayName << ": "
           << "requested Year changed from" << _yearToBeRequested << "to"
           << year;
    _yearToBeRequested = year;
  }
}

void CalendarClient_CalDAV::setMonth(const int &month) {
  if (E_STATE_IDLE == _state) {
    if (_month != month) {
      QDEBUG << _displayName << ": "
             << "Month changed from" << _month << "to" << month;
      _month = month;
      emit monthChanged(_month);
      _monthToBeRequested = month;
      startSynchronization();
    }
  } else {
    QDEBUG << _displayName << ": "
           << "requested Month changed from" << _monthToBeRequested << "to"
           << month;
    _monthToBeRequested = month;
  }
}

void CalendarClient_CalDAV::handleUploadHTTPError(void) {
  _uploadRequestTimeoutTimer.stop();
  if (nullptr != _pUploadReply) {
    QDEBUG << _displayName << ": "
           << "HTTP upload error:" << _pUploadReply->errorString();
    emit error(_pUploadReply->errorString());
  } else {
    QDEBUG << _displayName << ": "
           << "ERROR: Invalid reply pointer when handling HTTP error.";
    emit error("Invalid reply pointer when handling HTTP error.");
  }
}

/**
 * @brief         Methods to receive HTTP(S) responses from CalDAV server.
 */

void CalendarClient_CalDAV::handleHTTPError(void) {
  _state = E_STATE_ERROR;
  emit syncStateChanged(_state);
  _requestTimeoutTimer.stop();
  if (nullptr != _pReply) {
    QDEBUG << _displayName << ": "
           << "HTTP request error:" << _pReply->errorString();
    emit error(_pReply->errorString());
  } else {
    QDEBUG << _displayName << ": "
           << "ERROR: Invalid reply pointer when handling HTTP error.";
    emit error("Invalid reply pointer when handling HTTP error.");
  }
}

void CalendarClient_CalDAV::handleRequestCTagFinished(void) {
  _requestTimeoutTimer.stop();

  if (E_STATE_ERROR == _state) {
    QDEBUG << _displayName << ": "
           << "Error state - aborting";
  }

  QDEBUG << _displayName << ": "
         << "HTTP RequestCTag finished";

  if (nullptr != _pReply) {
    QDomDocument doc;

    QString reply = _pReply->readAll();
    QDEBUG << _displayName << "Response: " << reply;
    reply = reply.replace("<D:", "<")
                .replace("</D:", "</")
                .replace("<cs:", "<")
                .replace("</cs:", "</");
    QDEBUG << _displayName << ": "
           << "Response replaced: " << reply;

    doc.setContent(reply);

    QString sDisplayName = "";
    QString sCTag = "";
    QString sStatus = "";

    QDomNodeList response = doc.elementsByTagName("prop");
    QDEBUG << _displayName << ": "
           << "response.size() = " << response.size();
    for (int i = 0; i < response.size(); i++) {
      QDomNode n = response.item(i);
      QDomElement displayname = n.firstChildElement("displayname");
      if (!displayname.isNull()) {
        QDEBUG << _displayName << ": "
               << "DISPLAYNAME = " << displayname.text();
        sDisplayName = displayname.text();
      }
      QDomElement ctag = n.firstChildElement("getctag");
      if (!ctag.isNull()) {
        QDEBUG << _displayName << ": "
               << "CTAG = " << ctag.text();
        sCTag = ctag.text();
      }
    }

    response = doc.elementsByTagName("propstat");
    for (int i = 0; i < response.size(); i++) {
      QDomNode n = response.item(i);
      QDomElement status = n.firstChildElement("status");
      if (!status.isNull()) {
        QDEBUG << _displayName << ": "
               << "STATUS = " << status.text();
        sStatus = status.text();
      }
    }

    if ((!sCTag.isEmpty()) && (sStatus.endsWith("200 OK"))) {
      bool bDisplayNameChanged = (_displayName != sDisplayName);
      bool bCTagChanged = (_cTag != sCTag);

      _displayName = sDisplayName;
      _cTag = sCTag;

      if (false != bDisplayNameChanged) {
        emit displayNameChanged(_displayName);
      }

      if (false == bCTagChanged) {
        QDEBUG << _displayName << ": "
               << "ctag is unchanged";
        // emit syncTokenHasNotChanged(); FIXME

        if ((_year != lastSyncYear) || (_month != lastSyncMonth)) {
          QDEBUG << _displayName << ": "
                 << "year/month has changed from" << lastSyncYear
                 << lastSyncMonth << "to" << _year << _month
                 << "=> update required";
          lastSyncMonth = _month;
          lastSyncYear = _year;
          emit calendarUpdateRequired();
        } else {
          QDEBUG << _displayName << ": "
                 << "calendar has not changed, no update required";
          _synchronizationTimer.start();
          emit calendarHasNotChanged();
        }
      } else {
        QDEBUG << _displayName << ": "
               << "ctag has changed";
        // emit syncTokenChanged(); FIXME
        lastSyncMonth = _month;
        lastSyncYear = _year;
        emit calendarUpdateRequired();
      }
    } else {
      QDEBUG << _displayName << ": "
             << "ERROR: Receiving ctag failed. Status:" << sStatus;
      emit error("Receiving ctag failed.");
    }

    /*
    QDEBUG << _displayName << ": " << "\r\nHeaders:" <<
    _pReply->rawHeaderList() << "\r\n";
    if (_pReply->hasRawHeader("DAV")) {
      QDEBUG << _displayName << ": " << "DAV:" << _pReply->rawHeader("DAV");
    }
    if (_pReply->hasRawHeader("Server")) {
      QDEBUG << _displayName << ": " << "Server:" <<
    _pReply->rawHeader("Server");
    }
    QDEBUG << _displayName << ": " << "Status code:" <<
    _pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    */
  } else {
    QDEBUG << _displayName << ": "
           << "ERROR: Invalid reply pointer when receiving ctag.";
    emit error("Invalid reply pointer when receiving ctag.");
  }
}

/**
 * @brief         State machine implementation of class CalendarClient_CalDAV.
 */
void CalendarClient_CalDAV::setupStateMachine(void) {
  QStateMachine *pStateMachine = new QStateMachine(this);

  QState *pStateWaiting = new QState(pStateMachine);
  QState *pStateRequestingChanges = new QState(pStateMachine);
  QState *pStateProcessingChanges = new QState(pStateMachine);
  QState *pStateError = new QState(pStateMachine);

  connect(&_synchronizationTimer, SIGNAL(timeout()), this,
          SLOT(debug_handleTimerTimeout()));

  // pStateWaiting
  connect(pStateWaiting, SIGNAL(entered()), this,
          SLOT(handleStateWaitingEntry()));
  connect(pStateWaiting, SIGNAL(exited()), this,
          SLOT(handleStateWaitingExit()));

  // pStateRequestingChanges
  pStateRequestingChanges->addTransition(this, SIGNAL(calendarUpdateRequired()),
                                         pStateProcessingChanges);
  pStateRequestingChanges->addTransition(this, SIGNAL(calendarHasNotChanged()),
                                         pStateWaiting);
  pStateRequestingChanges->addTransition(this, SIGNAL(eventsUpdated()),
                                         pStateWaiting);
  pStateRequestingChanges->addTransition(this, SIGNAL(error(QString)),
                                         pStateError);
  connect(pStateRequestingChanges, SIGNAL(entered()), this,
          SLOT(handleStateRequestingChangesEntry()));
  connect(pStateRequestingChanges, SIGNAL(exited()), this,
          SLOT(handleStateRequestingChangesExit()));

  // pStateProcessingChanges
  pStateProcessingChanges->addTransition(this, SIGNAL(error(QString)),
                                         pStateError);
  pStateProcessingChanges->addTransition(this, SIGNAL(eventsUpdated()),
                                         pStateWaiting);

  connect(pStateProcessingChanges, SIGNAL(entered()), this,
          SLOT(handleStateProcessingChangesEntry()));
  connect(pStateProcessingChanges, SIGNAL(exited()), this,
          SLOT(handleStateProcessingChangesExit()));

  // pStateError
  pStateError->addTransition(this, SIGNAL(recoverSignal()), pStateWaiting);
  connect(pStateError, SIGNAL(entered()), this, SLOT(handleStateErrorEntry()));

  pStateMachine->setInitialState(pStateWaiting);

  pStateMachine->start();
}

CalendarClient_CalDAV::E_CalendarAuth
CalendarClient_CalDAV::getClientAuth(void) {
  return _auth;
}

void CalendarClient_CalDAV::handleStateWaitingEntry(void) {
  QDEBUG << _displayName << ": "
         << "entering pStateWaiting";
  _state = E_STATE_IDLE;
  emit syncStateChanged(_state);

  if ((_yearToBeRequested != _year) || (_monthToBeRequested != _month)) {
    QDEBUG << _displayName << ": "
           << "year/month has requested from" << _year << _month << "to"
           << _yearToBeRequested << _monthToBeRequested << "=> update required";
    setYear(_yearToBeRequested);
    setMonth(_monthToBeRequested);
    startSynchronization();
  } else if (false != _bRecoveredFromError) {
    QDEBUG << _displayName << ": "
           << "recovery from ERROR state => update required";
    _bRecoveredFromError = false;
    startSynchronization();
  }
}

void CalendarClient_CalDAV::handleStateWaitingExit(void) {
  _state = E_STATE_BUSY;
  emit syncStateChanged(_state);
  QDEBUG << _displayName << ": "
         << "leaving pStateWaiting";
}

void CalendarClient_CalDAV::handleStateRequestingChangesEntry(void) {
  QDEBUG << _displayName << ": "
         << "entering pStateRequestingChanges";
  getChangedEvent();
}

void CalendarClient_CalDAV::handleStateRequestingChangesExit(void) {
  QDEBUG << _displayName << ": "
         << "leaving pStateRequestingChanges";
}

void CalendarClient_CalDAV::handleStateProcessingChangesEntry(void) {
  QDEBUG << _displayName << ": "
         << "entering pStateProcessingChanges";
}

void CalendarClient_CalDAV::handleStateProcessingChangesExit(void) {
  QDEBUG << _displayName << ": "
         << "leaving pStateProcessingChanges";
}

void CalendarClient_CalDAV::handleStateErrorEntry(void) {
  QDEBUG << _displayName << ": "
         << "entering state error";
}

void CalendarClient_CalDAV::debug_handleTimerTimeout(void) {
  QDEBUG << _displayName << ": "
         << "~~~~~~sync timer timeout~~~~~~";
}
