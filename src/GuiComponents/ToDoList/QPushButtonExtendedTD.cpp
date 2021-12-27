//
// Created by Davide on 27/12/2021.
//

#include "QPushButtonExtendedTD.h"

#include <QDebug>

QPushButtonExtendedTD::QPushButtonExtendedTD(const char *text, QWidget *parent)
        : QPushButton(text, parent) {
    // clicked can't be overwritten
    if (strcmp(text, "DELETE") == 0) {
        connect(this, &QPushButtonExtendedTD::clicked, this,
                &QPushButtonExtendedTD::button_clicked_delete);
    } else if (strcmp(text, "EDIT") == 0) {
        connect(this, &QPushButtonExtendedTD::clicked, this,
                &QPushButtonExtendedTD::button_edit_clicked);
    }
}

void QPushButtonExtendedTD::button_clicked_delete() {
    emit on_click_delete(this);
}
void QPushButtonExtendedTD::button_edit_clicked() {
    emit on_click_edit(this);
}
