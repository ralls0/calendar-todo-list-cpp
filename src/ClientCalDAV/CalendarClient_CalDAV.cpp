/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    CalendarClient_CalDAV.cpp
 * @brief
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

CalendarClient_CalDAV::CalendarClient_CalDAV(QObject *parent)
    : CalendarClient(parent) {
  _calendarType = E_CALENDAR_CALDAV;
  _auth = E_AUTH_UPWD;
  _dataStream = nullptr;
  _pUploadReply = nullptr;
  _au = nullptr;

  _username = "";
  _password = "";
  _hostURL = "";
  _syncToken = "";
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
  _calendarType = E_CALENDAR_CALDAV;
  _auth = E_AUTH_UPWD;
  _dataStream = nullptr;
  _pUploadReply = nullptr;
  _au = nullptr;

  _syncToken = "";
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
  _calendarType = E_CALENDAR_CALDAV;
  _auth = E_AUTH_TOKEN;
  _dataStream = nullptr;
  _pUploadReply = nullptr;
  _au = new (std::nothrow) OAuth(filepath, scope);
  connect(_au, &OAuth::accessTokenChanged, this,
          &CalendarClient_CalDAV::setAccessToken);

  _username = "";
  _password = "";
  _syncToken = "";
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
}

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

void CalendarClient_CalDAV::handleUploadFinished(void) {
  _uploadRequestTimeoutTimer.stop();

  QDEBUG << _displayName << ": "
         << "HTTP upload finished";

  if (nullptr != _pUploadReply) {
    std::cout << _displayName.toStdString() << ": "
              << "received:\r\n"
              << _pUploadReply->readAll().toStdString();
    emit forceSynchronization();
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

void CalendarClient_CalDAV::handleRequestSyncTokenFinished(void) {
  _requestTimeoutTimer.stop();

  if (E_STATE_ERROR == _state) {
    QDEBUG << _displayName << ": "
           << "Error state - aborting";
  }

  QDEBUG << _displayName << ": "
         << "HTTP RequestSyncToken finished";

  if (nullptr != _pReply) {
    QDomDocument doc;

    doc.setContent(_pReply);

    QString sDisplayName = "";
    QString sCTag = "";
    QString sSyncToken = "";
    QString sStatus = "";

    QDomNodeList response = doc.elementsByTagName("d:prop");
    for (int i = 0; i < response.size(); i++) {
      QDomNode n = response.item(i);
      QDomElement displayname = n.firstChildElement("d:displayname");
      if (!displayname.isNull()) {
        // QDEBUG << _displayName << ": " << "DISPLAYNAME = " <<
        // displayname.text();
        sDisplayName = displayname.text();
      }
      QDomElement ctag = n.firstChildElement("cs:getctag");
      if (!ctag.isNull()) {
        // QDEBUG << _displayName << ": " << "CTAG = " << ctag.text();
        sCTag = ctag.text();
      }
      QDomElement syncToken = n.firstChildElement("d:sync-token");
      if (!syncToken.isNull()) {
        // QDEBUG << _displayName << ": " << "SYNC-TOKEN = " <<
        // syncToken.text();
        sSyncToken = syncToken.text();
      }
    }

    response = doc.elementsByTagName("d:propstat");
    for (int i = 0; i < response.size(); i++) {
      QDomNode n = response.item(i);
      QDomElement status = n.firstChildElement("d:status");
      if (!status.isNull()) {
        // QDEBUG << _displayName << ": " << "STATUS = " << status.text();
        sStatus = status.text();
      }
    }

    if ((!sSyncToken.isEmpty()) && (sStatus.endsWith("200 OK"))) {
      bool bDisplayNameChanged = (_displayName != sDisplayName);
      bool bSyncTokenChanged = (_syncToken != sSyncToken);

      if (true == bSyncTokenChanged) {
        QDEBUG << _displayName << ": "
               << "sync token has changed from" << _syncToken << "to"
               << sSyncToken;
      }

      _displayName = sDisplayName;
      _cTag = sCTag;
      _syncToken = sSyncToken;

      if (false != bDisplayNameChanged) {
        emit displayNameChanged(_displayName);
      }

      if (false == bSyncTokenChanged) {
        QDEBUG << _displayName << ": "
               << "sync token is unchanged";
        emit syncTokenHasNotChanged();

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
               << "sync token has changed";
        emit syncTokenChanged();
        lastSyncMonth = _month;
        lastSyncYear = _year;
        emit calendarUpdateRequired();
      }
    } else {
      QDEBUG << _displayName << ": "
             << "ERROR: Receiving sync token failed. Status:" << sStatus;
      emit error("Receiving sync token failed.");
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
           << "ERROR: Invalid reply pointer when receiving sync token.";
    emit error("Invalid reply pointer when receiving sync token.");
  }
}

void CalendarClient_CalDAV::handleRequestChangesFinished(void) {
  _requestTimeoutTimer.stop();

  if (E_STATE_ERROR == _state) {
    QDEBUG << _displayName << ": "
           << "Error state - aborting";
  }

  QDEBUG << _displayName << ": "
         << "HTTP RequestChanges finished";

  if (nullptr != _pReply) {
    QDomDocument doc;

    // QDEBUG << _displayName << ": " << "received:\r\n" <<
    // _pReply->readAll();

    doc.setContent(_pReply);

    _eventList.clear();

    QDomNodeList list_response = doc.elementsByTagName("d:response");
    for (int i = 0; i < list_response.size(); i++) {
      QDomNode thisResponse = list_response.item(i);

      // QDEBUG << _displayName << ": " << "Response" << i;

      QString sHref = "";
      QString sETag = "";
      QString sPropStatus = "";
      QString strCalendarData = "";

      QDomElement elHref = thisResponse.firstChildElement("d:href");
      if (!elHref.isNull()) {
        // QDEBUG << _displayName << ": " << "  HREF = " << elHref.text();
        sHref = elHref.text();
      } else {
        QDEBUG << _displayName << ": "
               << "  HREF = ";
      }

      QDomNode thisPropStat = thisResponse.firstChildElement("d:propstat");
      if (!thisPropStat.isNull()) {

        QDomElement elPropStatus = thisPropStat.firstChildElement("d:status");
        if (!elPropStatus.isNull()) {
          // QDEBUG << _displayName << ": " << "    PROPSTATUS = " <<
          // elPropStatus.text();
          sPropStatus = elPropStatus.text();
        } else {
          QDEBUG << _displayName << ": "
                 << "    PROPSTATUS = ";
        }

        QDomNode thisProp = thisPropStat.firstChildElement("d:prop");
        if (!thisProp.isNull()) {
          QDomElement elETag = thisProp.firstChildElement("d:getetag");
          if (!elETag.isNull()) {
            // QDEBUG << _displayName << ": " << "    ETAG = " <<
            // elETag.text();
            sETag = elETag.text();
          } else {
            QDEBUG << _displayName << ": "
                   << "    ETAG = ";
          }

          QDomElement elCalendarData =
              thisProp.firstChildElement("cal:calendar-data");
          if (!elCalendarData.isNull()) {
            // QDEBUG << _displayName << ": " << "    CALENDARDATA = " <<
            // elCalendarData.text();

            if (_dataStream) {
              delete _dataStream;
            }
            _dataStream = new QTextStream(elCalendarData.text().toLatin1());

            parseCALENDAR(sHref);

            strCalendarData = elCalendarData.text();
          } else {
            QDEBUG << _displayName << ": "
                   << "    CALENDARDATA = ";
          }
        }
      }
    }

    int iStatusCode =
        _pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QDEBUG << _displayName << ": "
           << "Status code:" << iStatusCode;

    if (207 != iStatusCode) {
      QDEBUG << _displayName << ": "
             << "ERROR: Invalid HTTP return code:" << iStatusCode;
      emit error("Invalid HTTP return code.");
    } else {
      QDEBUG << _displayName << ": "
             << "OK, restarting synchronization\r\n\r\n";
      _synchronizationTimer.start();
      emit eventsUpdated();
    }

  } else {
    QDEBUG << _displayName << ": "
           << "ERROR: Invalid reply pointer when receiving changes.";
    emit error("Invalid reply pointer when receiving changes.");
  }
}

/**
 * @brief         State machine implementation of class CalendarClient_CalDAV.
 */
void CalendarClient_CalDAV::setupStateMachine(void) {
  QStateMachine *pStateMachine = new QStateMachine(this);

  QState *pStateWaiting = new QState(pStateMachine);
  QState *pStateRequestingSyncToken = new QState(pStateMachine);
  QState *pStateRequestingChanges = new QState(pStateMachine);
  QState *pStateProcessingChanges = new QState(pStateMachine);
  QState *pStateError = new QState(pStateMachine);

  connect(&_synchronizationTimer, SIGNAL(timeout()), this,
          SLOT(debug_handleTimerTimeout()));

  // pStateWaiting
  pStateWaiting->addTransition(&_synchronizationTimer, SIGNAL(timeout()),
                               pStateRequestingSyncToken);
  pStateWaiting->addTransition(this, SIGNAL(forceSynchronization()),
                               pStateRequestingSyncToken);
  connect(pStateWaiting, SIGNAL(entered()), this,
          SLOT(handleStateWaitingEntry()));
  connect(pStateWaiting, SIGNAL(exited()), this,
          SLOT(handleStateWaitingExit()));

  // pStateRequestingSyncToken
  pStateRequestingSyncToken->addTransition(
      this, SIGNAL(calendarUpdateRequired()), pStateRequestingChanges);
  pStateRequestingSyncToken->addTransition(
      this, SIGNAL(calendarHasNotChanged()), pStateWaiting);
  pStateRequestingSyncToken->addTransition(this, SIGNAL(error(QString)),
                                           pStateError);
  connect(pStateRequestingSyncToken, SIGNAL(entered()), this,
          SLOT(handleStateRequestingSyncTokenEntry()));
  connect(pStateRequestingSyncToken, SIGNAL(exited()), this,
          SLOT(handleStateRequestingSyncTokenExit()));

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

void CalendarClient_CalDAV::handleStateRequestingSyncTokenEntry(void) {
  QDEBUG << _displayName << ": "
         << "entering pStateRequestingSyncToken";
  sendRequestSyncToken();
}

void CalendarClient_CalDAV::handleStateRequestingSyncTokenExit(void) {
  QDEBUG << _displayName << ": "
         << "leaving pStateRequestingSyncToken";
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
