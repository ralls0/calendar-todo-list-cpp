#include "qpushbutton_extended.h"

#include <QDebug>

QPushButtonExtended::QPushButtonExtended(const char *text, QWidget *parent) : QPushButton(text, parent)
{
    //clicked can't be overwritten
    connect(this, &QPushButtonExtended::clicked, this, &QPushButtonExtended::button_clicked);
}

QPushButtonExtended::QPushButtonExtended(QWidget *parent) : QPushButton(parent)
{
    //clicked can't be overwritten
    connect(this, &QPushButtonExtended::clicked, this, &QPushButtonExtended::button_edit_clicked);
}

void QPushButtonExtended::button_edit_clicked() {
    emit on_click_edit(this);
}

void QPushButtonExtended::button_clicked() {
    emit on_click(this->index);
}

void QPushButtonExtended::setIndex(int index) {
    this->index = index;
}
