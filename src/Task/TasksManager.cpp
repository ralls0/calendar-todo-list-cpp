//
// Created by Davide on 23/12/2021.
//

#include "TasksManager.h"
TasksManager::TasksManager(QString accT, QObject *parent) : QObject(parent) {
  m_pNetworkAccessManager = new QNetworkAccessManager(this);
  _accT = accT;
  _id = "";
  connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(replyFinished(QNetworkReply *)));
}

QVariantList TasksManager::getTaskLists() { return m_taskLists; }

QList<TaskElement *> TasksManager::getTasks() { return _tasks; }

void TasksManager::getMyTaskLists(const QString &access_token) {
  QString s = QString("https://www.googleapis.com/tasks/v1/users/@me/"
                      "lists?&access_token=%1")
                  .arg(access_token);
  QNetworkReply *reply = m_pNetworkAccessManager->get(QNetworkRequest(QUrl(s)));
  /*QString json = reply->readAll();
  QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
  QJsonObject jsonObj = doc.object();*/
}

void TasksManager::getMyTasks(const QString &access_token,
                              const QString &taskListID) {
  QString s =
      QString(
          "https://www.googleapis.com/tasks/v1/lists/%1/tasks?&access_token=%2")
          .arg(taskListID)
          .arg(access_token);
  m_pNetworkAccessManager->get(QNetworkRequest(QUrl(s)));
}

void TasksManager::createList(const QString &access_token,
                              const QString &title) {
  QString s =
      QString(
          "https://www.googleapis.com/tasks/v1/users/@me/lists?access_token=%1")
          .arg(access_token);
  QByteArray params;
  params.append(QString("{ title: \"%1\" }").arg(title).toUtf8());
  QNetworkRequest request;
  request.setUrl(QUrl(s));
  request.setRawHeader("Content-Type", "application/json");
  m_pNetworkAccessManager->post(request, params);
}

void TasksManager::deleteList(const QString &access_token,
                              const QString &taskListID) {
  QString s = QString("https://www.googleapis.com/tasks/v1/users/@me/lists/"
                      "%1?access_token=%2")
                  .arg(taskListID)
                  .arg(access_token);
  m_pNetworkAccessManager->deleteResource(QNetworkRequest(QUrl(s)));
}

void TasksManager::deleteTask(const QString &access_token,
                              const QString &taskListID,
                              const QString &taskID) {
  QString s = QString("https://www.googleapis.com/tasks/v1/lists/%1/tasks/"
                      "%2?access_token=%3")
                  .arg(taskListID)
                  .arg(taskID)
                  .arg(access_token);
  m_pNetworkAccessManager->deleteResource(QNetworkRequest(QUrl(s)));
}

void TasksManager::createTask(const QString &access_token,
                              const QString &taskListID, const QString &title, const QJsonObject &json_object) {
  QString s;
  s = QString(
          "https://www.googleapis.com/tasks/v1/lists/%1/tasks?access_token=%2")
          .arg(taskListID)
          .arg(access_token);
    QNetworkRequest request;
    request.setUrl(QUrl(s));
    QJsonDocument doc(json_object);
    QByteArray data = doc.toJson();

    request.setRawHeader("Content-Type", "application/json");
    m_pNetworkAccessManager->post(request, data);
/*
  QByteArray params;
  params.append(QString("{ title: \"%1\" }").arg(title).toUtf8());
  QNetworkRequest request;
  request.setUrl(QUrl(s));
  request.setRawHeader("Content-Type", "application/json");
  m_pNetworkAccessManager->post(request, params);*/
}

void TasksManager::updateTask(const QString &access_token,
                              const QString &taskListID, const QString &taskID,
                              const QJsonObject &json_object) {
  QString s = QString("https://www.googleapis.com/tasks/v1/lists/%1/tasks/"
                      "%2?access_token=%3")
                  .arg(taskListID)
                  .arg(taskID)
                  .arg(access_token);

  QNetworkRequest request;
  request.setUrl(QUrl(s));
  QJsonDocument doc(json_object);
  QByteArray data = doc.toJson();

  request.setRawHeader("Content-Type", "application/json");
  m_pNetworkAccessManager->put(request, data);
}

const QString &TasksManager::getAccT() const { return _accT; }

void TasksManager::setAccT(const QString &accT) { _accT = accT; }

const QString &TasksManager::getId() const { return _id; }

void TasksManager::setId(const QString &id) { _id = id; }

void TasksManager::replyFinished(QNetworkReply *reply) {

  _tasks.clear();
  QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
  QJsonObject jsonObj = doc.object();
  QString strUrl = reply->url().toString();
  QJsonArray jsonArray;
  if (jsonObj["kind"] == "tasks#taskLists") {
    setId(jsonObj["items"][0]["id"].toString());
    emit getAll(getAccT(), getId());
  } else if (jsonObj["kind"] == "tasks#tasks") {
    jsonArray = jsonObj["items"].toArray();
    foreach (const QJsonValue &value, jsonArray) {
      QJsonObject obj = value.toObject();
      QDateTime dataN = QDateTime::fromString(obj["due"].toString(),
                                              "yyyy-MM-ddTHH:mm:ss.zzzZ");
      QDateTime dataUpd = QDateTime::fromString(obj["updated"].toString(),
                                                "yyyy-MM-ddTHH:mm:ss.zzzZ");
      TaskElement *te =
          new TaskElement(dataN, false, obj["title"].toString(),
                          obj["id"].toString(), dataUpd, obj["etag"].toString(),
                          // obj["position"].toString(),
                          obj["selfLink"].toString(), obj["status"].toString(),
                          obj["kind"].toString(), obj["due"].toString(),
                          obj["updated"].toString(), 0);
      _tasks.append(te);
    }
    emit getAllTask(getTasks());
  } else {
    emit getAll(getAccT(), getId());
  }
}
