//
// Created by Davide on 23/12/2021.
//

#ifndef CALENDAR_TODO_LIST_CPP_TASKSMANAGER_H
#define CALENDAR_TODO_LIST_CPP_TASKSMANAGER_H

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include "TaskElement.h"


class TasksManager : public QObject {
  Q_OBJECT
public:
  explicit TasksManager(QString a, QObject *parent = 0);
  void getMyTaskLists(const QString &access_token);
  void getMyTasks(const QString &access_token, const QString &taskListID);
  void deleteTask(const QString &access_token, const QString &taskListID,
                  const QString &taskID);
  void createTask(const QString &access_token, const QString &taskListID,
                  const QString &title, const QString &prevTaskID,
                  const QString &parentID);
  void updateTask(const QString &access_token, const QString &taskListID,
                  const QString &taskID, const QVariant &json_object);

  void createList(const QString &access_token, const QString &title);
  void deleteList(const QString &access_token, const QString &taskListID);
  QList<TaskElement*>  getTasks();
  QVariantList getTaskLists();
  const QString &getAccT() const;
  void setAccT(const QString &accT);

signals:
    void getAllTask(QList<TaskElement* > _tasks);
    void getAll(QString t, QString r);
private slots:
  void replyFinished(QNetworkReply *);

private:
  QString _accT;
public:
    const QString &getId() const;

    void setId(const QString &id);

private:
    QString _id;
  QNetworkAccessManager *m_pNetworkAccessManager;
  QVariantList m_taskLists;

  QList<TaskElement* > _tasks;
};

#endif // CALENDAR_TODO_LIST_CPP_TASKSMANAGER_H
