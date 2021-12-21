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
    xTitolo->addWidget(todoTitle);
    this->layout->addLayout(xTitolo);


    QHBoxLayout *nascondi = new QHBoxLayout;
    QCheckBox *switched = new QCheckBox("Nascondi Completate");
    switched->setStyleSheet(STYLE_TODO_COMPLETE);
    nascondi->addWidget(switched);
    this->layout->addLayout(nascondi);

    QHBoxLayout *hl = new QHBoxLayout;
    QCheckBox *do1 = new QCheckBox;
    do1->setText("To-Do List1");
    QLabel *datatodo = new QLabel("21/12/1998");
    QPushButton *buttonEdit = new QPushButton("EDIT");
    QPixmap pixmap(EDIT_PATH);
    QIcon EditIcon(pixmap);
    buttonEdit->setIcon(EditIcon);
    buttonEdit->setIconSize(pixmap.rect().size());
    QPushButton *buttonDelete = new QPushButton("DELETE");
    QPixmap pixmapD(DELETE_PATH);
    pixmapD = pixmapD.scaled(QSize(18, 18), Qt::KeepAspectRatio);
    QIcon DeleteIcon(pixmapD);
    buttonDelete->setIcon(DeleteIcon);
    buttonDelete->setMask(pixmapD.mask());
    //buttonDelete->setIcon(DeleteIcon);
    buttonDelete->setIconSize(pixmapD.rect().size());


    hl->addWidget(do1);
    hl->addWidget(datatodo);
    hl->addWidget(buttonEdit);
    hl->addWidget(buttonDelete);
    this->layout->addLayout(hl);
    setLayout(this->layout);
}
