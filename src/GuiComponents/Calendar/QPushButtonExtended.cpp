/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    QPushButtonExtended.cpp
 * @brief
 *
 */

#include "QPushButtonExtended.h"

QPushButtonExtended::QPushButtonExtended(const char *text, QWidget *parent)
    : QPushButton(text, parent) {
  // clicked can't be overwritten
  int u = 3;
  if (strcmp(text, "DELETE") == 0) {
    connect(this, &QPushButtonExtended::clicked, this,
            &QPushButtonExtended::button_clicked_delete);
  } else if (strcmp(text, "EDIT") == 0) {
    connect(this, &QPushButtonExtended::clicked, this,
            &QPushButtonExtended::button_edit_clicked);
  } else if (strcmp(text, "Show All") == 0) {
    connect(this, &QPushButtonExtended::clicked, this,
            &QPushButtonExtended::button_clicked);
  }
}

QPushButtonExtended::QPushButtonExtended(QWidget *parent)
    : QPushButton(parent) {
  // clicked can't be overwritten
  connect(this, &QPushButtonExtended::clicked, this,
          &QPushButtonExtended::button_edit_clicked);
}

void QPushButtonExtended::button_clicked_delete() {
  emit on_click_delete(this);
}
void QPushButtonExtended::button_edit_clicked() { emit on_click_edit(this); }

void QPushButtonExtended::button_clicked() { emit on_click(this->index); }

void QPushButtonExtended::setIndex(int index) { this->index = index; }
