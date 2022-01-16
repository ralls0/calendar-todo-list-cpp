/**
 *
 * @author  Marco Manco
 * @date    16/01/22.
 * @file    ErrorDialog.cpp
 * @brief
 *
 */

#include "ErrorDialog.h"

ErrorDialog::ErrorDialog(QWidget *parent): QDialog(parent) {
  createBaseInfoGroupBox();

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(_baseInfoGroupBox, 0, 0);
  setLayout(layout);

  setWindowTitle(tr("Error"));
}

void ErrorDialog::createBaseInfoGroupBox() {
  _baseInfoGroupBox = new QGroupBox(this);

  lbl_err = new QLabel(tr("An error has occurred!"), _baseInfoGroupBox);
  btn_close = new QPushButton("Close window", _baseInfoGroupBox);
  btn_close->setCheckable(true);
  connect(btn_close, &QPushButton::clicked, this, &ErrorDialog::handleCloseClick);

  _baseInfoLayout = new QGridLayout(this);
  _baseInfoLayout->addWidget(lbl_err, 0, 0, Qt::AlignCenter);
  _baseInfoLayout->addWidget(btn_close, 1, 1, Qt::AlignCenter);
  _baseInfoGroupBox->setLayout(_baseInfoLayout);
}

void ErrorDialog::handleCloseClick() {
  emit pushedCloseButton();
  close();
}