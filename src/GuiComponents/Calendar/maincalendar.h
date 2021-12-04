#ifndef CALENDAR_TODO_LIST_CPP_WINDOWNEWCALENDAR_H
#define CALENDAR_TODO_LIST_CPP_WINDOWNEWCALENDAR_H

#include<QApplication>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QTableWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QTextCharFormat>
#include <QWidget>
#include <iostream>
#include <QMainWindow>
#include "viewComponent/qwidget_extended.h"
#include "viewComponent/qframe_extended.h"
#include "viewComponent/util/dateutil.h"
#include "viewComponent/qlabel_event.h"
#include "viewComponent/qpushbutton_extended.h"
#include "viewComponent/customdialog.h"
#include "../NewEvent/newEventDialog.h"
#include "viewComponent/API/APImain.h"

/* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted.
 * This should be used when you don't care about the day */
#define CURRENT_MONTH *this->frames[21]->getDate()

#define MAINWINDOW_STYLE "#mainwindow { background-color: #FFFFFF; border: 2px solid #DDDDFF; }"

#define LABELDATE_STYLE "QLabel { color: #000000; qproperty-alignment: AlignCenter; padding-left: 100px; padding-right: 100px; font-size: 20px; }"
#define CELL_STYLE "QFrame { background-color: #FFFFFF; color: #000000; border: 1px outset #CCCCCC; }" \
                   "QFrame#holiday { background-color: #FFF5F5; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; background-color:rgba(0,0,0,0); }" \
                   "QLabel#today { background-color: #FFFF88; color: #FF0000;  border: 1px solid #FF0000; }" \
                   "QFrame#selected { background-color: #EEEEFF;  }" \
                   "QLabel#header { font-weight: bold; }" \
                   "QFrame#header { background-color: #DDDDFF; }" \
                   "QFrame#disabled { background-color: #EFEFEF; }"""


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
  QFrameExtended *frames[42]; //I have a 7x7 grid, but without consider the first row I've a total of 6x7 cells
  void createPreviewGroupBox();
  QGroupBox *previewGroupBox;
  QGridLayout *previewLayout;
  QVBoxLayout *layout;
  QCalendarWidget *calendar;
  QLabel *label_date; //la label centrale
};

#endif

