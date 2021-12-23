//
// Created by Davide on 22/12/2021.
//

#include "TaskElement.h"

TaskElement::TaskElement(const QDateTime &deadline, bool status,
                         const QString &name, int id)
    : _deadline(deadline), _status(status), _name(name), _id(id) {}

void TaskElement::setDeadline(const QDateTime &deadline) {
  _deadline = deadline;
}

void TaskElement::setStatus(bool status) { _status = status; }

void TaskElement::setName(const QString &name) { _name = name; }

void TaskElement::setId(int id) { _id = id; }

const QDateTime &TaskElement::getDeadline() const { return _deadline; }

bool TaskElement::isStatus() const { return _status; }

const QString &TaskElement::getName() const { return _name; }

int TaskElement::getId() const { return _id; }
