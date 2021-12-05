/**
 *
 * @author  Marco Manco
 * @date    05/12/21.
 * @file    MainWindow.cpp
 * @brief
 *
 */

#include "MainWindow.h"

#define DEBUG_OAUTH 1
#if DEBUG_OAUTH
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  _newCalendarDialog = new NewCalendarDialog;
  _newEventDialog = new NewEventDialog;
  _calendar = new MainCalendar;
  _cals = nullptr;
  connect(_newCalendarDialog, &NewCalendarDialog::newCalendar, this,
          &MainWindow::createNewCalendar);

  createPreviewGroupBox();

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(_previewGroupBox, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  setWindowTitle(tr("Calendar Widget"));
}

void MainWindow::createNewCalendar(const QString &displayName,
                                   const QString &hostURL, bool isBasicAuth,
                                   const QString &username,
                                   const QString &password,
                                   const QString &clientSecret) {

  QDEBUG << "[i] Creating new calendar\n";

  if (isBasicAuth) {
    _cals = new CalendarClient_CalDAV(username, password, hostURL, displayName,
                                      nullptr);
  } else {
    _cals =
        new CalendarClient_CalDAV(clientSecret, hostURL, displayName, nullptr);
  }
  connect(_newEventDialog, &NewEventDialog::newEvent, _cals,
          &CalendarClient_CalDAV::saveEvent);
}

void MainWindow::createPreviewGroupBox() {
  _previewGroupBox = new QGroupBox;
  _previewGroupBox->setFlat(true);
  _previewGroupBox->setStyleSheet("QGroupBox {  border:0; }");

  QPushButton *btn_addCalendar = new QPushButton("Add Calendar +", nullptr);
  connect(btn_addCalendar, &QPushButton::clicked, _newCalendarDialog,
          &QWidget::show);

  QPushButton *btn_addEvent = new QPushButton("Create +", nullptr);
  connect(btn_addEvent, &QPushButton::clicked, _newEventDialog, &QWidget::show);

  QWidget *_todo = new QWidget;
  _todo->setMinimumSize(450, 420);

  _previewLayout = new QGridLayout;
  _previewLayout->addWidget(_calendar, 0, 0);
  _previewLayout->addWidget(_todo, 0, 1);
  _previewLayout->addWidget(btn_addCalendar, 1, 0, Qt::AlignRight);
  _previewLayout->addWidget(btn_addEvent, 1, 1, Qt::AlignRight);
  _previewGroupBox->setLayout(_previewLayout);
}