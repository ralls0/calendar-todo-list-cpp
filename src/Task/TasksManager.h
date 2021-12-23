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
  QVariantList getTasks();
  QVariantList getTaskLists();
  const QString &getAccT() const;
  void setAccT(const QString &accT);

private slots:
  void replyFinished(QNetworkReply *);

private:
  QString _accT;
  QNetworkAccessManager *m_pNetworkAccessManager;
  QVariantList m_taskLists;
  QVariantList m_tasks;
};

#endif // CALENDAR_TODO_LIST_CPP_TASKSMANAGER_H
