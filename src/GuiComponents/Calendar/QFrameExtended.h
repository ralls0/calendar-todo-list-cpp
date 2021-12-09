/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    QFrameExtended.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_QFRAMEEXTENDED_H
#define CALENDAR_TODO_LIST_CPP_QFRAMEEXTENDED_H

#include "Date.h"
#include "DateUtil.h"
#include <QDebug>
#include <QFrame>
#include <QObject>

class QFrameExtended : public QFrame {
  Q_OBJECT


private:
  Date *date;

public:
  explicit QFrameExtended(QWidget *parent = nullptr);
  ~QFrameExtended();
  Date *getDate();
  void setDate(Date *d);
};

#endif // CALENDAR_TODO_LIST_CPP_QFRAMEEXTENDED_H
