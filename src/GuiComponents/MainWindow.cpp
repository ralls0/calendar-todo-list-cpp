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
    _newCalendarDialog = nullptr;
    _newEventDialog = nullptr;
    _calendar = new MainCalendar;

    _cals = new CalendarManager(QString(QDir::currentPath() + "/initCals.ini"));
    connect(_cals,&CalendarManager::listOfCalendarsChanged,_calendar,&MainCalendar::setCalendarList);
    connect(_cals,&CalendarManager::listOfEventsChanged,_calendar,&MainCalendar::updateListOfEvents);

    createPreviewGroupBox();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(_previewGroupBox, 0, 0);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    setWindowTitle(tr("Calendar Widget"));
}

/*MainWindow::~MainWindow() {
  if(_previewGroupBox)
    delete _previewGroupBox;

  if(_previewLayout)
    delete _previewLayout;

  if(_newCalendarDialog)
    delete _newCalendarDialog;

  if(_newEventDialog)
    delete _newEventDialog;

  if(_calendar)
    delete _calendar;

  if(_cals)
    delete _cals;
}*/

void MainWindow::createNewCalendar(const QString &displayName,
                                   const QString &hostURL, bool isBasicAuth,
                                   const QString &username,
                                   const QString &password,
                                   const QString &clientSecret) {

    QDEBUG << "[i] Creating new calendar\n";

    QString newDisplayName =
            this->checkDisplayName(_cals->getListOfCalendarsClient(), displayName);

    if (isBasicAuth) {
        _cals->addCalDAV_Calendar(
                QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF).name(),
                newDisplayName, hostURL, username, password,
                CalendarClient_CalDAV::E_CalendarAuth::E_AUTH_UPWD);
    } else {
        _cals->addCalDAV_Calendar(
                QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF).name(),
                newDisplayName, hostURL, clientSecret, "",
                CalendarClient_CalDAV::E_CalendarAuth::E_AUTH_TOKEN);
    }
}

void MainWindow::createNewEvent(QString uid, QString filename, QString summary,
                                QString location, QString description,
                                QString rrule, QDateTime startDateTime,
                                QDateTime endDateTime, QString calendar) {
    foreach (CalendarClient_CalDAV *pListItem,
             _cals->getListOfCalendarsClient()) {
        if (calendar == pListItem->getDisplayName()) {
            pListItem->saveEvent(uid, filename, summary, location, description, rrule,
                                 startDateTime, endDateTime);
            emit _cals->eventsUpdated();
            emit _cals->listOfEventsChanged(_cals->getListOfEvents());
            break;
        }
    }
}

void MainWindow::createNewCalendarDialog() {
    _newCalendarDialog = new NewCalendarDialog;
    connect(_newCalendarDialog, &NewCalendarDialog::newCalendar, this,
            &MainWindow::createNewCalendar);
    _newCalendarDialog->show();
}
void MainWindow::createNewEventDialog() {
    _newEventDialog = new NewEventDialog(_cals->getListOfCalendarsName());
    connect(_newEventDialog, &NewEventDialog::newEvent, this,
            &MainWindow::createNewEvent);
    _newEventDialog->show();
}

void MainWindow::createPreviewGroupBox() {
    _previewGroupBox = new QGroupBox;
    _previewGroupBox->setFlat(true);
    _previewGroupBox->setStyleSheet("QGroupBox {  border:0; }");

    QPushButton *btn_addCalendar = new QPushButton("Add Calendar +", nullptr);
    connect(btn_addCalendar, &QPushButton::clicked, this,
            &MainWindow::createNewCalendarDialog);

    QPushButton *btn_addEvent = new QPushButton("Create +", nullptr);
    connect(btn_addEvent, &QPushButton::clicked, this,
            &MainWindow::createNewEventDialog);

    QWidget *_todo = new QWidget;
    _todo->setMinimumSize(450, 320);

    _previewLayout = new QGridLayout;
    _previewLayout->addWidget(_calendar, 0, 0);
    _previewLayout->addWidget(_todo, 0, 1);
    _previewLayout->addWidget(btn_addCalendar, 1, 0, Qt::AlignRight);
    _previewLayout->addWidget(btn_addEvent, 1, 1, Qt::AlignRight);
    _previewGroupBox->setLayout(_previewLayout);
}

QString MainWindow::checkDisplayName(QList<CalendarClient_CalDAV *> cals,
                                     QString displayName) {
    QString ndn = displayName;
    foreach (CalendarClient *pListItem, cals) {
        if (displayName == pListItem->getDisplayName()) {
            ndn.append("-");
            break;
        }
    }
    return ndn;
}
