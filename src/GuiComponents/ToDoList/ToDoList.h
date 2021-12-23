//
// Created by Davide on 21/12/2021.
//

#ifndef CALENDAR_TODO_LIST_CPP_TODOLIST_H
#define CALENDAR_TODO_LIST_CPP_TODOLIST_H
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>


#define EDIT_PATH "../img/edit.png"
#define DELETE_PATH "../img/delete.png"
#define STYLE_TODO_TITLE "font-size: 24px; font-weight:bold;margin-top:13px;background-color:yellow;"
#define STYLE_TODO_COMPLETE "font-size: 12px; font-weight:bold;"

class ToDoList : public QWidget {
    Q_OBJECT
public:
    explicit ToDoList(QWidget *parent = nullptr);

private:
    QVBoxLayout *layout;
};


#endif //CALENDAR_TODO_LIST_CPP_TODOLIST_H
