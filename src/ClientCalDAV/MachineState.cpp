/**
 *
 * @author  Marco Manco
 * @date    10/12/21.
 * @file    MachineState.cpp
 * @brief
 *
 */

#include "ClientCalDAV.h"
/**
 * @brief       About QStateMachine se [QStateMachine
 * details](https://doc.qt.io/qt-5/qstatemachine.html#details)
 */
void ClientCalDAV::setupStateMachine(void) {
  QStateMachine *pStateMachine = new QStateMachine(this);

  QState *pStateWaiting = new QState(pStateMachine);
  QState *pStateRequestingChanges = new QState(pStateMachine);
  QState *pStateCheckingChanges = new QState(pStateMachine);
  QState *pStateProcessingChanges = new QState(pStateMachine);
  QState *pStateError = new QState(pStateMachine);

  pStateWaiting->addTransition(&_synchronizationTimer, SIGNAL(timeout()),
                               pStateCheckingChanges);
  pStateWaiting->addTransition(this, SIGNAL(accessTokenChanged(QString)),
                               pStateCheckingChanges);
  pStateWaiting->addTransition(this, SIGNAL(passwordChanged(QString)),
                               pStateCheckingChanges);
  pStateWaiting->addTransition(this, SIGNAL(forceSynchronization()),
                               pStateRequestingChanges);

  // pStateWaiting
  connect(pStateWaiting, SIGNAL(entered()), this,
          SLOT(handleStateWaitingEntry()));
  connect(pStateWaiting, SIGNAL(exited()), this,
          SLOT(handleStateWaitingExit()));

  pStateCheckingChanges->addTransition(this, SIGNAL(error(QString)),
                                       pStateError);
  pStateCheckingChanges->addTransition(this, SIGNAL(calendarCTagChanged()),
                                       pStateRequestingChanges);
  pStateCheckingChanges->addTransition(
      this, SIGNAL(calendarCTagHasNotChanged()), pStateWaiting);

  connect(pStateCheckingChanges, SIGNAL(entered()), this,
          SLOT(handleStateCheckingChangesEntry()));
  connect(pStateCheckingChanges, SIGNAL(exited()), this,
          SLOT(handleStateCheckingChangesExit()));

  // pStateRequestingChanges
  pStateRequestingChanges->addTransition(
      this, SIGNAL(calendarHasNotChanged()),
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