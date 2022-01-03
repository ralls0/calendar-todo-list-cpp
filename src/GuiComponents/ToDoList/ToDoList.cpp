//
// Created by Davide on 21/12/2021.
//

#include "ToDoList.h"

ToDoList::ToDoList(QWidget *parent) : QWidget(parent) {

  _layout = new QGridLayout(this);
  lbl_title = new QLabel("ToDo List", this);
  lbl_title->setStyleSheet(STYLE_TODO_TITLE);
  _layout->addWidget(lbl_title, 0, 0);
  setLayout(_layout);
}

ToDoList::~ToDoList() {
  delete _layout;
  delete lbl_title;
}

void ToDoList::updateTaskList(QList<TaskElement *> tasks) {
  QDEBUG << "[i] Starting updateTaskList with " << tasks.size() << " tasks";
  if (!_listaTask.isEmpty()) {
    QLayoutItem *child;
    while ((child = _layout->takeAt(1)) != nullptr) {
      delete child->widget();
      delete child;
    }
    _listaTask.clear();
  }

  int i = 1;
  for (TaskElement *t : tasks) {
    QWidget *wget = new QWidget();
    QGridLayout *wly = new QGridLayout(wget);
    _listaTask.push_front(t->getName());
    QCheckBoxExtended *do1 = new QCheckBoxExtended(wget);
    connect(do1, &QCheckBoxExtended::on_click_done, this, &ToDoList::on_check_edit_click);
    do1->setText(t->getName());
    do1->setTask(t);
    QLabel *datatodo;
    if (t->getDeadline().isValid()) {
      QStringList listaD = t->getDeadline().toString().split(" ");
      datatodo = new QLabel(listaD[0] + " " + listaD[1] + " " + listaD[2], wget);
    } else {
      datatodo = new QLabel("NO DEADLINE", wget);
    }
    QPushButtonExtendedTD *buttonEdit = new QPushButtonExtendedTD("EDIT", wget);
    buttonEdit->setTask(t);
    QPixmap pixmapE(EDIT_PATH);
    QIcon EditIcon(pixmapE);
    buttonEdit->setIcon(EditIcon);
    connect(buttonEdit, &QPushButtonExtendedTD::on_click_edit, this, &ToDoList::on_button_edit_click);
    QPushButtonExtendedTD *buttonDelete = new QPushButtonExtendedTD("DELETE", wget);
    buttonDelete->setTask(t);
    QPixmap pixmapD(DELETE_PATH);
    QIcon DeleteIcon(pixmapD);
    buttonDelete->setIcon(DeleteIcon);
    connect(buttonDelete, &QPushButtonExtendedTD::on_click_delete, this, &ToDoList::on_button_delete_click);
    wly->addWidget(do1, 0, 0);
    wly->addWidget(datatodo, 0, 1);
    wly->addWidget(buttonEdit, 0, 2);
    wly->addWidget(buttonDelete, 0, 3);
    wget->setLayout(wly);
    wget->setVisible(true);
    _layout->addWidget(wget, i++, 0);
  }
}

void ToDoList::on_check_edit_click(QCheckBoxExtended *d){
    TaskElement *e = nullptr;
    if (d != nullptr) {
        e = d->getTask();
        emit doneTask(e);
    }
}
void ToDoList::on_button_edit_click(QPushButtonExtendedTD *d) {
  TaskElement *e = nullptr;
  if (d != nullptr) {
    e = d->getTask();
    emit modifyTask(e);
  }
}

void ToDoList::on_button_delete_click(QPushButtonExtendedTD *d) {
  TaskElement *e = nullptr;
  if (d != nullptr)
    e = d->getTask();
  QMessageBox::StandardButton reply;
  reply =
      QMessageBox::question(this, "Delete Task", "Are you sure to delete task '"+d->getTask()->getName()+"' ?",
                            QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    emit deleteTask(e);
  }
}