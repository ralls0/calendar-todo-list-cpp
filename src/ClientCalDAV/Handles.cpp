/**
 *
 * @author  Marco Manco
 * @date    10/12/21.
 * @file    Handles.cpp
 * @brief
 *
 */

#include "ClientCalDAV.h"

void ClientCalDAV::startSynchronization(void) {
  QDEBUG << "[i] (" << _displayName << ") "
         << "!!!forcing synchronization!!!";
  emit forceSynchronization();
}

void ClientCalDAV::stopSynchronization(void) {}

void ClientCalDAV::recover(void) {
  QDEBUG << "[i] (" << _displayName << ") "
         << "trying to recover from EEROR state";
  _bRecoveredFromError = true;
  emit recoverSignal();
}

void ClientCalDAV::handleUploadHTTPError(void) {
  _uploadRequestTimeoutTimer.stop();
  if (nullptr != _pUploadReply) {
    QDEBUG << "[i] (" << _displayName << ") "
           << "HTTP upload error:" << _pUploadReply->errorString();
    emit error(_pUploadReply->errorString());
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when handling HTTP error.";
    emit error("Invalid reply pointer when handling HTTP error.");
  }
}

/**
 * @brief         Methods to receive HTTP(S) responses from CalDAV server.
 */
void ClientCalDAV::handleRequestAuthentication(QNetworkReply *reply,
                                               QAuthenticator *_authenticator) {
  _authenticator->setUser(_username);
  _authenticator->setPassword(_password);
}
void ClientCalDAV::handleHTTPError(void) {
  _state = E_STATE_ERROR;
  emit syncStateChanged(_state);
  _requestTimeoutTimer.stop();
  if (nullptr != _pReply) {
    QDEBUG << "[i] (" << _displayName << ") "
           << "HTTP request error:" << _pReply->errorString();
    emit error(_pReply->errorString());
  } else {
    QDEBUG << "[i] (" << _displayName << ") "
           << "ERROR: Invalid reply pointer when handling HTTP error.";
    emit error("Invalid reply pointer when handling HTTP error.");
  }
}

void ClientCalDAV::handleStateWaitingEntry(void) {
  QDEBUG << "[i] (" << _displayName << ") Entering pStateWaiting";
  _state = E_STATE_IDLE;
  emit syncStateChanged(_state);

  if ((_yearToBeRequested != _year) || (_monthToBeRequested != _month)) {
    QDEBUG << "[i] (" << _displayName << ") "
           << "year/month has requested from" << _year << _month << "to"
           << _yearToBeRequested << _monthToBeRequested << "=> update required";
    setYear(_yearToBeRequested);
    setMonth(_monthToBeRequested);
    startSynchronization();
  } else if (_bRecoveredFromError) {
    QDEBUG << "[i] (" << _displayName << ") "
           << "recovery from ERROR state => update required";
    _bRecoveredFromError = false;
    startSynchronization();
  }
}

void ClientCalDAV::handleStateWaitingExit(void) {
  _state = E_STATE_BUSY;
  emit syncStateChanged(_state);
  QDEBUG << "[i] (" << _displayName << ") Leaving pStateWaiting";
}

void ClientCalDAV::handleStateCheckingChangesEntry(void) {
  QDEBUG << "[i] (" << _displayName << ") Entering pStateCheckingChanges";
  retrieveCTag();
}

void ClientCalDAV::handleStateCheckingChangesExit(void) {
  QDEBUG << "[i] (" << _displayName << ") Leaving pStateCheckingChanges";
}

void ClientCalDAV::handleStateRequestingChangesEntry(void) {
  QDEBUG << "[i] (" << _displayName << ") Entering pStateRequestingChanges";
  emit calendarUpdateRequired();
  retrieveChangedEvent();
}

void ClientCalDAV::handleStateRequestingChangesExit(void) {
  QDEBUG << "[i] (" << _displayName << ") Leaving pStateRequestingChanges";
}

void ClientCalDAV::handleStateProcessingChangesEntry(void) {
  QDEBUG << "[i] (" << _displayName << ") Entering pStateProcessingChanges";
}

void ClientCalDAV::handleStateProcessingChangesExit(void) {
  QDEBUG << "[i] (" << _displayName << ") Leaving pStateProcessingChanges";
}

void ClientCalDAV::handleStateErrorEntry(void) {
  QDEBUG << "[i] (" << _displayName << ") Entering state error";
}

void ClientCalDAV::debug_handleTimerTimeout(void) {
  QDEBUG << "[i] (" << _displayName << ") "
         << "~~~~~~sync timer timeout~~~~~~";
  emit calendarCheckCTag();
}
