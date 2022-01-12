//
// Created by Davide on 21/12/2021.
//

#ifndef CALENDAR_TODO_LIST_CPP_TODOLIST_H
#define CALENDAR_TODO_LIST_CPP_TODOLIST_H
#include "../../Task/TaskElement.h"
#include <QApplication>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#define EDIT_PATH "../img/edit.png"
#include "./QCheckBoxExtended.h"
#include "./QPushButtonExtendedTD.h"
#define DELETE_PATH "../img/delete.png"
#define STYLE_TODO_TITLE                                                       \
  "font-size: 24px; font-weight:bold; margin-bottom: 100px;"
#define STYLE_TODO_COMPLETE "font-size: 12px; font-weight:bold;"

#define DEBUG_ 1
#if DEBUG_
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

class ToDoList : public QWidget {
  Q_OBJECT

signals:
  void deleteTask(TaskElement *t);
  void modifyTask(TaskElement *t);
  void doneTask(TaskElement *t);

public:
  explicit ToDoList(QWidget *parent = nullptr);
  ~ToDoList();
  void updateTaskList(QList<TaskElement *> _tasks);

private:
  void on_button_edit_click(QPushButtonExtendedTD *d);
  void on_button_delete_click(QPushButtonExtendedTD *d);
  void on_check_edit_click(QCheckBoxExtended *d);
  QGridLayout *_layout;
  QList<QString> _listaTask;
  QLabel *lbl_title;
};

#endif // CALENDAR_TODO_LIST_CPP_TODOLIST_H
