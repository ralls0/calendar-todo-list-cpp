//
// Created by Davide on 03/01/2022.
//

#include "QCheckBoxExtended.h"
#include <QDebug>

QCheckBoxExtended::QCheckBoxExtended(QWidget *parent) : QCheckBox(parent) {
  connect(this, &QCheckBoxExtended::stateChanged, this,
          &QCheckBoxExtended::checkbox_done_clicked);
}

void QCheckBoxExtended::checkbox_done_clicked(int x) {
  emit on_click_done(this);
}