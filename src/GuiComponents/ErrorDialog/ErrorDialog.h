/**
 *
 * @author  Marco Manco
 * @date    16/01/22.
 * @file    ErrorDialog.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_ERRORDIALOG_H
#define CALENDAR_TODO_LIST_CPP_ERRORDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class ErrorDialog : public QDialog {
  Q_OBJECT
public:
  ErrorDialog(QWidget *parent = nullptr);
  ~ErrorDialog() = default;

public slots:
      void handleCloseClick(void);

signals:
  void pushedCloseButton(void);

private:
  void createBaseInfoGroupBox();

  QGroupBox *_baseInfoGroupBox;
  QGridLayout *_baseInfoLayout;

  QLabel *lbl_err;

  QPushButton *btn_close;
};

#endif // CALENDAR_TODO_LIST_CPP_ERRORDIALOG_H
