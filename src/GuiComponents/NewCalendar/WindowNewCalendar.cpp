/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    WindowNewCalendar.cpp
 * @brief
 *
 */

#include "WindowNewCalendar.h"

#define DEBUG_OAUTH 1
#if DEBUG_OAUTH
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

WindowNewCalendar::WindowNewCalendar(QWidget *parent) : QWidget(parent) {
  _newCalendarDialog = new NewCalendarDialog;
  _newEventDialog = new NewEventDialog;
  _cals = nullptr;
  connect(_newCalendarDialog, &NewCalendarDialog::newCalendar, this,
          &WindowNewCalendar::createNewCalendar);

  createPreviewGroupBox();

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(_previewGroupBox, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  setWindowTitle(tr("Calendar Widget"));
}

void WindowNewCalendar::createNewCalendar(const QString &displayName,
                                          const QString &hostURL,
                                          bool isBasicAuth,
                                          const QString &username,
                                          const QString &password,
                                          const QString &clientSecret) {

  QDEBUG << "[i] Creating new calendar\n";

  if (isBasicAuth) {
    _cals = new CalendarClient_CalDAV(username, password, hostURL, displayName,
                                      nullptr);
  } else {
    _cals = new CalendarClient_CalDAV(
        clientSecret, hostURL, displayName,
        "https://www.googleapis.com/auth/calendar", "Rallso", nullptr);
  }
  connect(_newEventDialog, &NewEventDialog::newEvent, _cals,
          &CalendarClient_CalDAV::saveEvent);
  connect(_cals, &CalendarClient_CalDAV::accessTokenChanged, _cals,
          &CalendarClient_CalDAV::getChangedEvent);
}

void WindowNewCalendar::createPreviewGroupBox() {
  _previewGroupBox = new QGroupBox(tr("Main"));

  QPushButton *btn_addCalendar = new QPushButton("Add Calendar", nullptr);
  connect(btn_addCalendar, &QPushButton::clicked, _newCalendarDialog,
          &QWidget::show);

  QPushButton *btn_addEvent = new QPushButton("Add Event", nullptr);
  connect(btn_addEvent, &QPushButton::clicked, _newEventDialog, &QWidget::show);

  _previewLayout = new QGridLayout;
  _previewLayout->addWidget(btn_addCalendar, 0, 0, Qt::AlignCenter);
  _previewLayout->addWidget(btn_addEvent, 1, 0, Qt::AlignCenter);
  _previewGroupBox->setLayout(_previewLayout);
}