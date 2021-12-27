//
// Created by Davide on 22/12/2021.
//

#ifndef CALENDAR_TODO_LIST_CPP_TASKELEMENT_H
#define CALENDAR_TODO_LIST_CPP_TASKELEMENT_H
#include <QDateTime>


class TaskElement {
public:
  TaskElement(const QDateTime deadline, bool status, const QString name,
              QString id);

  void setDeadline(const QDateTime &deadline);

  const QDateTime &getDeadline() const;

  bool isStatus() const;

  const QString &getName() const;

  QString getId() const;

  void setStatus(bool status);

  void setName(const QString &name);

  void setId(QString id);

protected:
  QDateTime _deadline;
  bool _status;
  QString _name;
  QString _id;
};

#endif // CALENDAR_TODO_LIST_CPP_TASKELEMENT_H
