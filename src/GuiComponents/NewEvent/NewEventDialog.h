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

#include "../../Utils/ColorUtils.h"
#include "../Calendar/ViewComponents/Model/Event.h"
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

class NewEventDialog : public QDialog {
  Q_OBJECT

public:
  NewEventDialog(QList<QString> cals, QWidget *parent = nullptr);
  NewEventDialog(Event *event, QWidget *parent = nullptr);
public slots:
  void onSaveClick(void);

signals:
  void newEvent(QString uid, QString filename, QString summary,
                QString location, QString description, QString rrule,
                QDateTime startDateTime, QDateTime endDateTime,
                QString calendar);
  void newAction(); // FIXME

private:
  void createBaseInfoLayout(QList<QString> cals, Event *event = nullptr);
  void createEventLayout(QList<QString> cals, Event *event = nullptr);
  void createActivityLayout(QList<QString> cals, Event *event = nullptr);
  void createButtonGroupBox(Event *event = nullptr);

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

  WindowStyle _colorStyle;
};

#endif // CALENDAR_TODO_LIST_CPP_NEWEVENTDIALOG_H
