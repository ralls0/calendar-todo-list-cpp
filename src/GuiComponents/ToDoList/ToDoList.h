//
// Created by Davide on 21/12/2021.
//

#ifndef CALENDAR_TODO_LIST_CPP_TODOLIST_H
#define CALENDAR_TODO_LIST_CPP_TODOLIST_H
#include <QApplication>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <QMessageBox>
#include "../../Task/TaskElement.h"
#define EDIT_PATH "../img/edit.png"
#include "./QPushButtonExtendedTD.h"
#define DELETE_PATH "../img/delete.png"
#define STYLE_TODO_TITLE                                                       \
  "font-size: 24px; font-weight:bold;margin-top:13px;background-color:yellow;"
#define STYLE_TODO_COMPLETE "font-size: 12px; font-weight:bold;"

class ToDoList : public QWidget {
  Q_OBJECT
  signals:
    void deleteTask(TaskElement *t);
    void modifyTask(TaskElement *t);
public:
  explicit ToDoList(QWidget *parent = nullptr);
  void updateTaskList(QList<TaskElement* > _tasks);
private:
    void on_button_edit_click(QPushButtonExtendedTD *d);
    void on_button_delete_click(QPushButtonExtendedTD *d);
  QVBoxLayout *layout;
  QVBoxLayout *lista;
};

#endif // CALENDAR_TODO_LIST_CPP_TODOLIST_H
