/**
 *
 * @author  Marco Manco
 * @date    05/12/21.
 * @file    MainWindow.cpp
 * @brief
 *
 */

#include "MainWindow.h"

#define DEBUG_MAINWINDOW 1
#if DEBUG_MAINWINDOW
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  _newCalendarDialog = nullptr;
  _newEventDialog = nullptr;

  _calendar = new MainCalendar(this);
  connect(_calendar, &MainCalendar::delete_event, this,
          &MainWindow::deleteEvent);
  connect(_calendar, &MainCalendar::modifyEvent, this,
          &MainWindow::createNewEventDialogM);

  _todo = new ToDoList(this);

  _cals =
      new CalendarManager(QString(QDir::currentPath() + "/initCals.ini"), this);
  connect(_cals, &CalendarManager::listOfCalendarsChanged, _calendar,
          &MainCalendar::setCalendarList);
  connect(_cals, &CalendarManager::listOfEventsChanged, _calendar,
          &MainCalendar::updateListOfEvents);
  connect(_calendar, &MainCalendar::calendarDateChanged, _cals,
          &CalendarManager::setDate);
  connect(_cals, &CalendarManager::setToDoList, this, &MainWindow::createToDo);

  createPreviewGroupBox();

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(_previewGroupBox, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  setWindowTitle(tr("Calendar Widget"));
  _cals->handleLoadSetting();
}

MainWindow::~MainWindow() {}

void MainWindow::createNewCalendar(const QString &displayName,
                                   const QString &hostURL, bool isBasicAuth,
                                   const QString &username,
                                   const QString &password,
                                   const QString &clientSecret) {

  QDEBUG << "[i] Creating new calendar\n";

  QString newDisplayName =
      this->checkDisplayName(_cals->getListOfCalendars(), displayName);

  if (isBasicAuth) {
    _cals->addCalendarCalDAVUP(newDisplayName, hostURL, username, password);
  } else {
    _cals->addCalendarCalDAVOA(newDisplayName, hostURL, clientSecret);
  }
}

void MainWindow::createNewEvent(QString uid, QString filename, QString summary,
                                QString location, QString description,
                                QString rrule, QDateTime startDateTime,
                                QDateTime endDateTime, QString calendar) {
  QDEBUG << "[i] Calendar" << calendar << " invoke saveEvent";
  int i = 0;
  foreach (QObject *pListItem, _cals->getListOfCalendars()) {
    if (calendar == pListItem->property("displayName").toString()) {
      ClientCalDAV *cal = _cals->getListItemAt(i);
      cal->saveEvent(uid, filename, summary, location, description, rrule,
                     startDateTime, endDateTime);
      emit _cals->eventsUpdated();
      emit _cals->listOfEventsChanged(_cals->getListOfEvents());
      break;
    }
    i++;
  }
}

void MainWindow::deleteEvent(CalendarEvent *ev) {
  int i = 0;
  foreach (QObject *pListItem, _cals->getListOfCalendars()) {
    if (ev->calendarName() == pListItem->property("displayName").toString()) {
      ClientCalDAV *cal = _cals->getListItemAt(i);
      cal->deleteEvent(ev->getHREF());
      emit _cals->eventsUpdated();
      emit _cals->listOfEventsChanged(_cals->getListOfEvents());
      break;
    }
    i++;
  }
}

void MainWindow::createNewCalendarDialog() {
  _newCalendarDialog = new NewCalendarDialog(this);
  connect(_newCalendarDialog, &NewCalendarDialog::newCalendar, this,
          &MainWindow::createNewCalendar);
  _newCalendarDialog->show();
}
void MainWindow::createNewEventDialog() {
  QList<QString> calsName;
  foreach (QObject *c, _cals->getListOfCalendars()) {
    calsName.append(c->property("displayName").toString());
  }
  _newEventDialog = new NewEventDialog(calsName, this);
  connect(_newEventDialog, &NewEventDialog::newEvent, this,
          &MainWindow::createNewEvent);
  _newEventDialog->show();
}

void MainWindow::createNewEventDialogM(CalendarEvent *event) {

  _newEventDialog = new NewEventDialog(event);
  connect(_newEventDialog, &NewEventDialog::newEvent, this,
          &MainWindow::createNewEvent);
  connect(_newEventDialog, &NewEventDialog::deleteEvent, this,
          &MainWindow::deleteEvent);
  _newEventDialog->show();
}

void MainWindow::createToDo(QString acc) {
  _taskm = new TasksManager(acc);
  _taskm->getMyTaskLists(_taskm->getAccT());
}

void MainWindow::createPreviewGroupBox() {
  _previewGroupBox = new QGroupBox(this);
  _previewGroupBox->setFlat(true);
  _previewGroupBox->setStyleSheet("QGroupBox {  border:0; }");

  QPushButton *btn_addCalendar = new QPushButton("Add Calendar +", this);
  connect(btn_addCalendar, &QPushButton::clicked, this,
          &MainWindow::createNewCalendarDialog);

  QPushButton *btn_addEvent = new QPushButton("Create +", this);
  connect(btn_addEvent, &QPushButton::clicked, this,
          &MainWindow::createNewEventDialog);

  // QWidget *_todo = new QWidget(this);
  _todo->setMinimumSize(450, 0);

  _previewLayout = new QGridLayout(this);
  _previewLayout->addWidget(_calendar, 0, 0);
  _previewLayout->addWidget(_todo, 0, 1);
  _previewLayout->addWidget(btn_addCalendar, 1, 0, Qt::AlignRight);
  _previewLayout->addWidget(btn_addEvent, 1, 1, Qt::AlignRight);
  _previewGroupBox->setLayout(_previewLayout);
}

QString MainWindow::checkDisplayName(QList<QObject *> cals,
                                     QString displayName) {
  QString ndn = displayName;
  foreach (QObject *pListItem, cals) {
    if (displayName == pListItem->property("displayName").toString()) {
      ndn.append("-");
      break;
    }
  }
  return ndn;
}
