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

#define DEBUG_SC 1
#if DEBUG_SC
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

#include <QDialog>
#include <QWidget>
#include <QDebug>
#include <QComboBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>

#define EDIT_PATH "../img/edit.png"
#define DELETE_PATH "../img/delete.png"
#define CLOSE_PATH "../img/close.png"
#define ADD_PATH "../img/add.png"

class ShareCalendarDialog: public QDialog {
  Q_OBJECT

public:
  ShareCalendarDialog(QList<QString> cals, QWidget *parent = nullptr);
  ~ShareCalendarDialog();

public slots:
  void onShareClick(void);

signals:
  void shareCalendar(QString displayName, QString email, QString Comments);

private:
  void createBaseLayout(QList<QString> cals);
  void createButtonGroupBox(void);

  QGridLayout *_baseInfoLayout;
  QGroupBox *gb_baseInfo;

  QLineEdit *le_email;
  QLabel *lbl_email;

  QComboBox *cb_calendar;

  QTextEdit *te_comment;

  QDialogButtonBox *_buttonBox;
  QPushButton *btn_cancel;
  QPushButton *btn_share;

};

#endif // CALENDAR_TODO_LIST_CPP_SHARECALENDARDIALOG_H
