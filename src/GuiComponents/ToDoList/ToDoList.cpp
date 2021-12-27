//
// Created by Davide on 21/12/2021.
//

#include "ToDoList.h"

ToDoList::ToDoList(QWidget *parent) : QWidget(parent) {
  this->layout = new QVBoxLayout;

  QHBoxLayout *xTitolo = new QHBoxLayout;
  QLabel *todoTitle = new QLabel("ToDo List");
  todoTitle->setStyleSheet(STYLE_TODO_TITLE);
  todoTitle->setMaximumHeight(40);
  todoTitle->setFixedWidth(400);
  QSizePolicy sp = todoTitle->sizePolicy();
  sp.setVerticalPolicy(QSizePolicy::Maximum);
  todoTitle->setSizePolicy(sp);
  xTitolo->setAlignment(Qt::AlignTop);
  xTitolo->addWidget(todoTitle);
  this->layout->addLayout(xTitolo);

  QHBoxLayout *nascondi = new QHBoxLayout;
  QCheckBox *switched = new QCheckBox("Nascondi Completate");
  switched->setStyleSheet(STYLE_TODO_COMPLETE);

  nascondi->addWidget(switched);

  this->layout->addLayout(nascondi);

  //updateTaskList();
 // lista = new QVBoxLayout;
  //this->layout->addLayout(lista);
  setLayout(this->layout);
}


void ToDoList::updateTaskList(QList<TaskElement* > _tasks){
    QVBoxLayout *lista = new QVBoxLayout;

        for (TaskElement *t: _tasks) {
            QCheckBox *do1 = new QCheckBox;
            do1->setText(t->getName());
            QStringList listaD = t->getDeadline().toString().split(" ");
            QLabel *datatodo = new QLabel(listaD[0]+" "+listaD[1]+ " "+listaD[2]);
            QPushButtonExtendedTD *buttonEdit = new QPushButtonExtendedTD("EDIT");
            buttonEdit->setTask(t);
            QPixmap pixmapE(EDIT_PATH);
            QIcon EditIcon(pixmapE);
            buttonEdit->setIcon(EditIcon);
            connect(buttonEdit, &QPushButtonExtendedTD::on_click_edit, this,
                    &ToDoList::on_button_edit_click);
            QPushButtonExtendedTD *buttonDelete = new QPushButtonExtendedTD("DELETE");
            buttonDelete->setTask(t);
            QPixmap pixmapD(DELETE_PATH);
            QIcon DeleteIcon(pixmapD);
            buttonDelete->setIcon(DeleteIcon);
            connect(buttonDelete, &QPushButtonExtendedTD::on_click_delete, this,
                    &ToDoList::on_button_delete_click);
            QHBoxLayout *hl = new QHBoxLayout;
            hl->addWidget(do1);
            hl->addWidget(datatodo);
            hl->addWidget(buttonEdit);
            hl->addWidget(buttonDelete);
            lista->addLayout(hl);

    }
    this->layout->addLayout(lista);
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
    reply = QMessageBox::question(this, "Delete Task",
                                  "Are you sure to delete task?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // QApplication::quit();
        emit deleteTask(e);
    }
}