/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    QWidgetExtended.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_QWIDGETEXTENDED_H
#define CALENDAR_TODO_LIST_CPP_QWIDGETEXTENDED_H

#include "QFrameExtended.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QString>
#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>
#include <QWidget>

enum { PRESSED, RELEASED, MOVED };

class QWidgetExtended : public QWidget {
  Q_OBJECT

private:
  void signalEvent(QMouseEvent *, int code);

public:
  explicit QWidgetExtended(QWidget *parent = 0);

protected:
  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  virtual void paintEvent(QPaintEvent *pe);

signals:
  void mousePress(QFrameExtended *);
  void mouseRelease(QFrameExtended *);
  void mouseMove(QFrameExtended *);
};

#endif // CALENDAR_TODO_LIST_CPP_QWIDGETEXTENDED_H
