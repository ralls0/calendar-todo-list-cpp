//
// Created by Davide on 22/12/2021.
//

#ifndef CALENDAR_TODO_LIST_CPP_TASKELEMENT_H
#define CALENDAR_TODO_LIST_CPP_TASKELEMENT_H
#include <QDateTime>

class TaskElement {
public:
  TaskElement(const QDateTime deadline, bool status, const QString name,
              QString id, QDateTime _updated, QString _etag, QString _selflink,
              QString _statusS, QString _kind, QString dString,
              QString uString);

  void setDeadline(const QDateTime &deadline);

  const QDateTime &getDeadline() const;

  bool isStatus() const;

  const QString &getName() const;

  QString getId() const;

  void setStatus(bool status);

  void setName(const QString &name);

  void setId(QString id);

  const QString &getEtag() const;

  const QString &getPosition() const;

  const QString &getSelflink() const;

  const QString &getStatusS() const;

  const QDateTime &getUpdated() const;

  const QString &getKind() const;

  const QString &getDeadlineString() const;

  void setDeadlineString(const QString &deadlineString);

  const QString &getUpdatedString() const;

  void setUpdatedString(const QString &updatedString);

protected:
  QDateTime _deadline;
  QString _deadlineString;
  bool _status;
  QString _name;
  QString _id;
  QString _etag;
  QString _position;
  QString _selflink;
  QString _statusS;
  QDateTime _updated;
  QString _updatedString;
  QString _kind;
};

#endif // CALENDAR_TODO_LIST_CPP_TASKELEMENT_H
