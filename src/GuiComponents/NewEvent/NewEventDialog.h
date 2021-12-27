/**
 *
 * @author  Marco Manco
 * @date    29/11/21.
 * @file    NewEventDialog.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_NEWEVENTDIALOG_H
#define CALENDAR_TODO_LIST_CPP_NEWEVENTDIALOG_H

#define DEBUG_ED 1
#if DEBUG_ED
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

#include "../../CalendarEvent/CalendarEvent.h"
#include "../../Utils/ColorUtils.h"
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QWidget>

#define EDIT_PATH "../img/edit.png"
#define DELETE_PATH "../img/delete.png"
#define CLOSE_PATH "../img/close.png"
#define ADD_PATH "../img/add.png"

class NewEventDialog : public QDialog {
  Q_OBJECT

public:
  NewEventDialog(QList<QString> cals, QWidget *parent = nullptr);
  NewEventDialog(CalendarEvent *event, QWidget *parent = nullptr);
  ~NewEventDialog();

public slots:
  void onSaveClick(void);
  void onDeleteClick(void);

signals:
  void newEvent(QString uid, QString filename, QString summary,
                QString location, QString description, QString rrule,
                QDateTime startDateTime, QDateTime endDateTime,
                QString calendar);
  void deleteEvent(CalendarEvent *t);
  void newAction(); // FIXME

private:
  void createBaseInfoLayout(QList<QString> cals,
                            CalendarEvent *event = nullptr);
  void createEventLayout(QList<QString> cals, CalendarEvent *event = nullptr);
  void createActivityLayout(const QList<QString> &cals,
                            CalendarEvent *event = nullptr);
  void createButtonGroupBox(CalendarEvent *event = nullptr);

  CalendarEvent *_event;
  QGridLayout *_baseInfoLayout;
  QGridLayout *_eventLayout;
  QGridLayout *_activityLayout;

  QGroupBox *gb_baseInfo;
  QWidget *e_event;
  QWidget *e_activity;

  QLineEdit *le_title;

  QGroupBox *_groupBox;
  QHBoxLayout *_vbox;
  QRadioButton *rb_event;
  QRadioButton *rb_activity;

  QDateTimeEdit *dte_startDateE;
  QDateTimeEdit *dte_endDateE;
  QDateTimeEdit *dte_startDateA;
  QDateTimeEdit *dte_endDateA;
  QComboBox *cb_rrule;
  QComboBox *cb_calendar;
  QComboBox *cb_activity;

  QLabel *lbl_location;
  QPixmap _pixmap;
  QLineEdit *le_location;

  QTextEdit *te_descriptionE;
  QTextEdit *te_descriptionA;

  QDialogButtonBox *_buttonBox;
  QPushButton *btn_cancel;
  QPushButton *btn_save;
  QPushButton *btn_delete;

  WindowStyle _colorStyle;
};

#endif // CALENDAR_TODO_LIST_CPP_NEWEVENTDIALOG_H
