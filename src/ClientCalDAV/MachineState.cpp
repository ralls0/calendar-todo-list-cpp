/**
 *
 * @author  Marco Manco
 * @date    10/12/21.
 * @file    MachineState.cpp
 * @brief
 *
 */

#include "ClientCalDAV.h"

void ClientCalDAV::setupStateMachine(void) {
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
  pStateRequestingChanges->addTransition(
      this, SIGNAL(calendarHasNotChanged()), // FIXME REPLICARE PER IL TODO
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