/**
 *
 * @author  Marco Manco
 * @date    27/12/21.
 * @file    ShareCalendarDialog.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_SHARECALENDARDIALOG_H
#define CALENDAR_TODO_LIST_CPP_SHARECALENDARDIALOG_H

#define DEBUG_ 1
#if DEBUG_
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

#define EDIT_PATH "../img/edit.png"
#define DELETE_PATH "../img/delete.png"
#define CLOSE_PATH "../img/close.png"
#define ADD_PATH "../img/add.png"

class ShareCalendarDialog : public QDialog {
  Q_OBJECT

public:
  ShareCalendarDialog(QList<QString> cals, QWidget *parent = nullptr);
  ~ShareCalendarDialog() = default;

public slots:
  void onShareClick(void);

signals:
  void shareCalendar(QString displayName, QString email, QString comments);

private:
  void createBaseLayout(QList<QString> cals);
  void createButtonGroupBox(void);

  QGridLayout *_baseLayout;
  QGroupBox *gb_base;

  QLineEdit *le_email;
  QLabel *lbl_email;

  QComboBox *cb_calendar;

  QTextEdit *te_comment;

  QDialogButtonBox *_buttonBox;
  QPushButton *btn_cancel;
  QPushButton *btn_share;
};

#endif // CALENDAR_TODO_LIST_CPP_SHARECALENDARDIALOG_H
