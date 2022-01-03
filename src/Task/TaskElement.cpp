//
// Created by Davide on 22/12/2021.
//

#include "TaskElement.h"

TaskElement::TaskElement(const QDateTime deadline, bool status,
                         const QString name, QString id, QDateTime updated,
                         QString etag, QString selfLink, QString statusS,
                         QString kind, QString dString, QString uString, int type)
    : _deadline(deadline), _status(status), _name(name), _id(id),
      _updated(updated), _etag(etag),
      _selflink(selfLink), _statusS(statusS), _kind(kind),
      _updatedString(uString), _deadlineString(dString), _type(type) {}

const QString &TaskElement::getEtag() const { return _etag; }

const QString &TaskElement::getPosition() const { return _position; }

const QString &TaskElement::getSelflink() const { return _selflink; }

const QString &TaskElement::getStatusS() const { return _statusS; }

const QDateTime &TaskElement::getUpdated() const { return _updated; }

const QString &TaskElement::getKind() const { return _kind; }

void TaskElement::setDeadline(const QDateTime &deadline) {
  _deadline = deadline;
}

void TaskElement::setStatus(bool status) { _status = status; }

void TaskElement::setName(const QString &name) { _name = name; }

void TaskElement::setId(QString id) { _id = id; }

const QDateTime &TaskElement::getDeadline() const { return _deadline; }

bool TaskElement::isStatus() const { return _status; }

const QString &TaskElement::getName() const { return _name; }

QString TaskElement::getId() const { return _id; }

const QString &TaskElement::getDeadlineString() const {
  return _deadlineString;
}

void TaskElement::setDeadlineString(const QString &deadlineString) {
  _deadlineString = deadlineString;
}

const QString &TaskElement::getUpdatedString() const { return _updatedString; }

void TaskElement::setUpdatedString(const QString &updatedString) {
  _updatedString = updatedString;
}

int TaskElement::getType() const {
    return _type;
}

void TaskElement::setType(int type) {
    _type = type;
}
