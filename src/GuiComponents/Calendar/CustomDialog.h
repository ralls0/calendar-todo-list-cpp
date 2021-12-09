/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    customDialog.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_CUSTOMDIALOG_H
#define CALENDAR_TODO_LIST_CPP_CUSTOMDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QLabel>


class CustomDialog : public QDialog {
  Q_OBJECT

public:
  explicit CustomDialog(QLayout *layout, QWidget *parent = 0);
  ~CustomDialog();

protected:
  virtual void keyPressEvent(QKeyEvent *e);
};

#endif // CALENDAR_TODO_LIST_CPP_CUSTOMDIALOG_H
