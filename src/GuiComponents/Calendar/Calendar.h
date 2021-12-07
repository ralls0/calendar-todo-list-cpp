/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Calendar.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_CALENDAR_H
#define CALENDAR_TODO_LIST_CPP_CALENDAR_H

#include "../../Utils/ColorUtils.h"
#include "../NewEvent/NewEventDialog.h"
#include "APImain.h"
#include "CustomDialog.h"
#include "DateUtils.h"
#include "QFrameExtended.h"
#include "QLabelEvent.h"
#include "QPushButtonExtended.h"
#include "QWidgetExtended.h"
#include <QApplication>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextCharFormat>
#include <QWidget>
#include <iostream>

/* Gets the current month displayed using an hack. Infact, the cell in the
 * middle will have always a value setted. This should be used when you don't
 * care about the day */
#define CURRENT_MONTH *this->frames[21]->getDate()

class MainCalendar : public QWidget {
  Q_OBJECT

public:
  MainCalendar(QWidget *parent = nullptr);
  ~MainCalendar();
  void display_events(Date date);

public slots:
  void on_back_button_click();
  void on_next_button_click();
  void on_event_click(QLabelEvent *label_event, Qt::MouseButton button);

private:
  WindowStyle _colorStyle;
  APImain *API;
  void on_button_edit_click(QPushButtonExtended *d);
  QTime timeToQTime(time_t datax);
  void on_button_extended_click(int index);
  void display_days(Date date);
  void remove_events_from_all_frames();
  void remove_events_from_frame(int i);
  QLabelEvent *selected_event;
  QLabelEvent *createLabelEvent(Event *event);
  QFrameExtended *createQFrameExtended(Date *date);
  QFrameExtended *frames[42]; // I have a 7x7 grid, but without consider the
                              // first row I've a total of 6x7 cells
  void createPreviewGroupBox();
  QGroupBox *previewGroupBox;
  QGridLayout *previewLayout;
  QVBoxLayout *layout;
  QCalendarWidget *calendar;
  QLabel *label_date; // la label centrale
};

#endif // CALENDAR_TODO_LIST_CPP_CALENDAR_H
