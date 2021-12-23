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


    QVBoxLayout *lista = new QVBoxLayout;


    for(int i=0;i<4;i++){
        QCheckBox *do1 = new QCheckBox;
        do1->setText("To-Do List1");
        QLabel *datatodo = new QLabel("21/12/1998");
        QPushButton *buttonEdit = new QPushButton("EDIT");
        QPixmap pixmapE(EDIT_PATH);
        QIcon EditIcon(pixmapE);
        buttonEdit->setIconSize(pixmapE.rect().size());
        QPushButton *buttonDelete = new QPushButton("DELETE");
        QPixmap pixmapD(DELETE_PATH);
        QIcon DeleteIcon(pixmapD);
        buttonDelete->setIcon(DeleteIcon);
        buttonEdit->setIconSize(pixmapD.rect().size());

        QHBoxLayout *hl = new QHBoxLayout;
        hl->addWidget(do1);
        hl->addWidget(datatodo);
        hl->addWidget(buttonEdit);
        hl->addWidget(buttonDelete);
        lista->addLayout(hl);
    }

    this->layout->addLayout(lista);
    setLayout(this->layout);
}
