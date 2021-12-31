/**
 *
 * @author  Marco Manco Davide Manco
 * @date    04/12/21.
 * @file    customDialog.cpp
 * @brief
 *
 */

#include "CustomDialog.h"

CustomDialog::CustomDialog(QLayout *layout, QWidget *parent) : QDialog(parent) {
  this->setWindowFlags(Qt::WindowCloseButtonHint |
                       Qt::WindowMinimizeButtonHint);
  setLayout(layout);
}

CustomDialog::~CustomDialog() {}

void CustomDialog::keyPressEvent(QKeyEvent *e) {
  if (e->key() != Qt::Key_Escape) {
    QDialog::keyPressEvent(e);
  } else {
    this->close();
    delete this;
  }
}
