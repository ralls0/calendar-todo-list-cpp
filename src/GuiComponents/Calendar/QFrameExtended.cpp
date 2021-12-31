/**
 *
 * @author  Marco Manco Davide Manco
 * @date    04/12/21.
 * @file    QFrameExtended.cpp
 * @brief
 *
 */

#include "QFrameExtended.h"

QFrameExtended::QFrameExtended(QWidget *parent) : QFrame(parent), date(NULL) {}
QFrameExtended::~QFrameExtended() { delete this->date; }
Date *QFrameExtended::getDate() { return date; }
void QFrameExtended::setDate(Date *d) {
  if (this->date != NULL)
    delete this->date;
  this->date = d;
}
