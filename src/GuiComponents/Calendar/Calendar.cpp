/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Calendar.cpp
 * @brief
 *
 */

#include <QPainter>
#include <QtGui/qpainterpath.h>
#include <ctime>

#include "Calendar.h"

MainCalendar::MainCalendar(QWidget *parent) : QWidget(parent) {
  Date current_date = DateUtil::get_current_date();
  this->labelDate = new QLabel;
  this->labelDate->setMaximumHeight(40);
  this->labelDate->setFixedWidth(400);
  this->labelDate->setStyleSheet(_colorStyle.getLabelDateStyle());
  QPushButton *back = new QPushButton("<");
  QPushButton *next = new QPushButton(">");
  back->setMaximumWidth(60);
  next->setMaximumWidth(60);
  back->setMaximumHeight(40);
  next->setMaximumHeight(40);
  back->setShortcut(QKeySequence(Qt::Key_Left));
  next->setShortcut(QKeySequence(Qt::Key_Right));
  next->setToolTip("Go to the next month, press ctrl to move to the next year");
  back->setToolTip(
      "Go to the previous month, press ctrl to move to the previous year");
  connect(back, &QPushButton::clicked, this,
          &MainCalendar::on_back_button_click);
  connect(next, &QPushButton::clicked, this,
          &MainCalendar::on_next_button_click);
  QHBoxLayout *hl =
      new QHBoxLayout; //è un layout orizzontale dove vanno i bottoni
  QHBoxLayout *hlbutton =
      new QHBoxLayout; //è un layout orizzontale dove vanno i bottoni
  QWidget *wbutton = new QWidget;
  hlbutton->addWidget(back, 1, Qt::AlignRight);
  hlbutton->addWidget(next, 1, Qt::AlignLeft);
  wbutton->setLayout(hlbutton);
  hl->addWidget(labelDate, 1);
  hl->addWidget(wbutton, 1, Qt::AlignRight);

  this->layout =
      new QVBoxLayout; // layout verticale dove sopra bottoni sotto calendario
  this->layout->addLayout(hl);
  _calendarList = new QList<QString>;
  _calendarListLayout = new QGridLayout;
  QLabel *ql = new QLabel("I tuoi calendari:");
  ql->setStyleSheet("font-size: 18px;");
  _calendarListLayout->addWidget(ql, 0, 0, Qt::AlignLeft);
  this->layout->addLayout(_calendarListLayout);
  QGridLayout *grid_layout = new QGridLayout;
  int i = 0, j = 0;
  // First row contains the names of the days of the week
  for (j = 0; j < 7; j++) {
    QFrame *frame = new QFrame;
    QHBoxLayout *hl = new QHBoxLayout;
    hl->setAlignment(Qt::AlignRight);
    frame->setFixedHeight(50);
    QLabel *wday_name =
        new QLabel(DateUtil::numeric2literal_day_of_week(j + 1).c_str());
    wday_name->setObjectName("header");
    frame->setObjectName("header");
    hl->addWidget(wday_name);
    frame->setLayout(hl);
    frame->setStyleSheet(_colorStyle.getCellStyle());
    grid_layout->addWidget(frame, 0, j);
  }
  // Next rows contain the days of the selected month
  for (i = 1; i < 7; i++) {   // rows
    for (j = 0; j < 7; j++) { // columns
      // Map a Time object to each frame
      QFrameExtended *frame = createQFrameExtended(NULL);
      grid_layout->addWidget(frame, i, j);
      this->frames[7 * (i - 1) + j] = frame;
    }
  }
  grid_layout->setHorizontalSpacing(0);
  grid_layout->setVerticalSpacing(0);

  this->layout->addLayout(grid_layout);

  display_days(current_date);
  setLayout(this->layout);
  connect(this, &MainCalendar::shownCalendarChanged, this,
          &MainCalendar::updateListOfEvents);
}

void MainCalendar::setCalendarList(QList<QObject *> t) {
  QDEBUG << "[i] Starting setCalendarList";
  _calList = t;
  QLayoutItem *child;
  while ((child = _calendarListLayout->takeAt(1)) != nullptr) {
    delete child->widget();
    delete child;
  }

  if (!_calendarList->isEmpty())
    _calendarList->clear();

  int i = 1;
  for (QObject *x : t) {
    QDEBUG << "[i] Add calendar: " << x->property("displayName").toString();
    QWidget *wget = new QWidget();
    QGridLayout *wly = new QGridLayout(wget);
    QCheckBox *cbox =
        new QCheckBox(x->property("displayName").toString(), wget);
    cbox->setStyleSheet(QString("QCheckBox {background-color: %1; font-size: 12px;}").arg(x->property("color").toString()));
    connect(cbox, &QCheckBox::stateChanged, this,
            &MainCalendar::filterCalendar);
    cbox->setCheckState(Qt::Checked);
    wly->addWidget(cbox, 0,0, Qt::AlignLeft);
    wget->setLayout(wly);
    wget->setVisible(true);

    _calendarListLayout->addWidget(wget, 0, i++, Qt::AlignLeft);
    _calendarList->push_front(x->property("displayName").toString());
  }
}

void MainCalendar::filterCalendar() {
  QDEBUG << "[i] Starting filterCalendar...";
  _calendarList->clear();
  QDEBUG << "[i] _calendarList size: " << _calendarList->size();
  QLayoutItem *child;
  while ((child = _calendarListLayout->takeAt(1)) != nullptr) {
    QWidget *wt = child->widget();
    QLayout *wtl = wt->layout();
    QCheckBox *cb = qobject_cast<QCheckBox *>(wtl->takeAt(0)->widget());
    if (cb && cb->isChecked()) {
      _calendarList->push_back(cb->text());
      delete cb;
    }
    delete wtl;
    delete wt;
    delete child;
  }

  resetCalendarList(_calList);
}

void MainCalendar::resetCalendarList(QList<QObject *> t) {
  QDEBUG << "[i] Starting resetCalendarList";
  QDEBUG << "[i] _calendarList size: " << _calendarList->size();
  QLayoutItem *child;
  while ((child = _calendarListLayout->takeAt(1)) != nullptr) {
    delete child->widget();
    delete child;
  }

  int i = 1;
  for (QObject *x : t) {
    QDEBUG << "[i] Add calendar: " << x->property("displayName").toString();
    QWidget *wget = new QWidget();
    QGridLayout *wly = new QGridLayout(wget);
    QCheckBox *cbox =
        new QCheckBox(x->property("displayName").toString(), wget);
    cbox->setStyleSheet(QString("QCheckBox {background-color: %1; font-size: 12px;}").arg(x->property("color").toString()));
    cbox->setCheckState(_calendarList->contains(x->property("displayName").toString()) ? Qt::Checked : Qt::Unchecked);
    connect(cbox, &QCheckBox::stateChanged, this,
            &MainCalendar::filterCalendar);
    wly->addWidget(cbox, 0,0, Qt::AlignLeft);
    wget->setLayout(wly);
    wget->setVisible(true);

    _calendarListLayout->addWidget(wget, 0, i++, Qt::AlignLeft);
  }
  emit shownCalendarChanged(_eventList);
}

void MainCalendar::updateListOfEvents(const QList<QObject *> &eventList) {
  QDEBUG << "[i] Starting updateListOfEvents..";
  if (eventList.isEmpty()) {
    QDEBUG << "[i] Return due to eventList is empty";
    return;
  }

  _eventList = eventList;
  QDate date = QDateTime::currentDateTime().date();

  // Remove all displayed events
  remove_events_from_all_frames();

  // Find at which cell the month starts
  int start_offset;
  for (start_offset = 0; start_offset < 42; start_offset++) {
    // Looks where is the first day of the month
    if (this->frames[start_offset]->getDate() != NULL)
      break;
  }

  for (QObject *event : _eventList) {
    if (_calendarList->contains(event->property("calendarName").toString())) {
      QDEBUG << "[i] _calendarList contains " << event->property("calendarName").toString();
      QDate start = event->property("startDateTime").toDate();
      QDate end = event->property("endDateTime").toDate();

      if (((start.month() < date.month()) && (start.year() == date.year())) ||
          (start.year() < date.year()))
        start = QDate(date.year(), date.month(), 1);

      if (((end.month() > start.month()) && (end.year() == start.year())) ||
          (end.year() > start.year()))
        end = start.addMonths(1).addDays(-1);
      // sovrascrive tutto perche' gli eventi vanno presi un mese alla volta,
      // non c'è controllo annoframe==annoevento && meseframe==meseevento
      for (int i = start_offset + start.day() - 1;
           i < (start_offset + end.day()); i++) {
        QLabelEvent *label_event = createLabelEvent(
            new CalendarEvent(event->property("color").toString(),
                              event->property("calendarName").toString(),
                              event->property("name").toString(),
                              event->property("location").toString(),
                              event->property("description").toString(),
                              event->property("startDateTime").toDateTime(),
                              event->property("endDateTime").toDateTime(),
                              event->property("rrule").toString(),  // RRULE
                              event->property("color").toString(),  // CATEGORY
                              event->property("uid").toString(),    // UID
                              event->property("href").toString(),   // HREF
                              event->property("exdates").toString() // EXDATE

                              ));

        /*COSE DA DIRE: SE IO CREO CON COSTRUTT DI COPIA FAI VEDERE CHE SUCCEDE
         * (DEVO PASSARE DA QOBJECT->cALENDAREVENT, COME?
         *
         * SE IO LO COSTRUISCO COSI, FUNZIONA MA ALCUNE PROPERTY VANNO A NULL ED
         * È PER QUESTO CHE POI GLI PASSO UN HREF A NULL, ED è PER LO STESSO
         * MOTIVO SECONDO ME CHE NON TI CREA UN EVENTO LASCIO TITTI PER FARTELO
         * VEDERE IN DEBUG mi ricordo di aver commentato la detele bastarda
         */
        QString titti = event->property("HREF").toString();
        QString titti2 = event->property("_HREF").toString();
        // serve se ho tanti eventi sulla stessa cella
        if (this->frames[i]->children().size() == 3) {
          QPushButtonExtended *button_show_all =
              new QPushButtonExtended("Show All");
          button_show_all->setIndex(i);
          connect(button_show_all, &QPushButtonExtended::on_click, this,
                  &MainCalendar::on_button_extended_click);
          this->frames[i]->layout()->addWidget(button_show_all);
          label_event->setHidden(true);
        } else if (this->frames[i]->children().size() > 4)
          label_event->setHidden(true);

        // Events will be copied and wrapped inside the QLabelEvent widgets
        (static_cast<QVBoxLayout *>(this->frames[i]->layout()))
            ->insertWidget(1, label_event);
      }
    }
  }
}

void MainCalendar::on_back_button_click() {
  Date newDate;
  // MODIFICA DI QAPPLICATION AGGIORNA A QT 6, COMPRENDI MEGLIO
  if (QApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    newDate = DateUtil::decrease_year(CURRENT_MONTH);
  else
    newDate = DateUtil::decrease_month(CURRENT_MONTH);
  display_days(newDate);
  /* Reload events */
  // this->selected_event = NULL;
  // display_events(newDate);
  emit calendarDateChanged(QDate(newDate.getYear(), newDate.getMonth(), 1));
}

void MainCalendar::on_next_button_click() {
  Date newDate;
  if (QApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    newDate = DateUtil::increase_year(CURRENT_MONTH);
  else
    newDate = DateUtil::increase_month(CURRENT_MONTH);
  display_days(newDate);
  /* Reload events */
  // this->selected_event = NULL;
  // display_events(newDate);
  emit calendarDateChanged(QDate(newDate.getYear(), newDate.getMonth(), 1));
}

void MainCalendar::remove_events_from_all_frames() {
  int i;
  for (i = 0; i < 42; i++) {
    // Delete all the event labels inside the frame
    remove_events_from_frame(i);
  }
}

void MainCalendar::remove_events_from_frame(int i) {
  QLabelEvent label_event;
  QPushButtonExtended button("");
  QListIterator<QObject *> it(this->frames[i]->children());
  // A meta-object contains information about a class that inherits QObject,
  // e.g. class name, superclass name, properties, signals and slots.
  while (it.hasNext()) {
    QObject *o = qobject_cast<QObject *>(it.next());
    /* Don't delete a QLabelEvent before returning to the event loop
     * (on_event_click), otherwise will be triggered a segmentation fault */
    if (o->metaObject()->className() == label_event.metaObject()->className()) {
      (qobject_cast<QLabelEvent *>(o))->setHidden(true);
      (qobject_cast<QLabelEvent *>(o))
          ->setParent(NULL); // Detach from parent QFrame
      (qobject_cast<QLabelEvent *>(o))->deleteLater();
    } else if (o->metaObject()->className() == button.metaObject()->className())
      delete o;
  }
}

void MainCalendar::on_button_extended_click(int index) {
  // Make a copy
  Date *date = new Date(this->frames[index]->getDate()->getMonthDay(),
                        this->frames[index]->getDate()->getWeekDay(),
                        this->frames[index]->getDate()->getMonth(),
                        this->frames[index]->getDate()->getYear());
  QFrameExtended *frame = createQFrameExtended(date);
  QString text =
      QString(
          DateUtil::numeric2literal_day_of_week(frame->getDate()->getWeekDay())
              .c_str()) +
      QString("    ") + QString::number(frame->getDate()->getMonthDay()) +
      QString(" ") +
      QString(
          DateUtil::get_literal_month(frame->getDate()->getMonth()).c_str()) +
      QString(" ") + QString::number(frame->getDate()->getYear());
  // QLabel *label_day = static_cast<QLabel*> (frame->children().at(1));
  QLabel *label_day = new QLabel(text);
  label_day->setStyleSheet(
      "background:linear-gradient(to bottom, #ffec64 5%, #ffab23 100%);"
      "background-color:#ffec64; border-radius:6px; border:1px solid #ffaa22; "
      "color:#333333; font-family:Arial; font-size:15px; "
      "font-weight:bold;padding:6px 24px;text-decoration:none;"
      "text-align: center;");

  char stime[14];
  int z = 0;
  QTableWidget *table =
      new QTableWidget(this->frames[index]->children().size() - 2, 5);
  table->setHorizontalHeaderItem(0, new QTableWidgetItem("NAME"));
  table->setHorizontalHeaderItem(1, new QTableWidgetItem("DESCRIPTION"));
  table->setHorizontalHeaderItem(2, new QTableWidgetItem("HOUR"));
  table->setHorizontalHeaderItem(3, new QTableWidgetItem("EDIT"));
  table->setHorizontalHeaderItem(4, new QTableWidgetItem("DELETE"));
  table->setStyleSheet(
      "QHeaderView {background-color: transparent;} "
      "QHeaderView::section {background-color: #646464;"
      "padding: 4px;font-size: 10pt; "
      "border-style: none;  border-bottom: 1px solid #fffff8; "
      "border-right: 1px solid #fffff8;  }"
      "QHeaderView::section:horizontal{ border-top: 1px solid #fffff8;  }"
      "QHeaderView::section:vertical  { border-left: 1px solid #fffff8;   }");

  for (QLabelEvent *label_event :
       this->frames[index]->findChildren<QLabelEvent *>()) {
    CalendarEvent *event = new CalendarEvent(*label_event->getEvent());
    QTime start = (event->getStartDateTime()).time();
    QTime end = (event->getEndDateTime()).time();
    snprintf(stime, 14, "%02d:%02d - %02d:%02d", start.hour(), start.minute(),
             end.hour(), end.minute());
    QLabel *time = new QLabel(stime);
    table->setCellWidget(z, 0, createLabelEvent(event));
    table->setItem(z, 1, new QTableWidgetItem(QString(event->description())));
    table->setItem(z, 2, new QTableWidgetItem(stime));
    QPushButtonExtended *edit = new QPushButtonExtended();
    QPixmap pixmapE(EDIT_PATH);
    QIcon EditIcon(pixmapE);
    edit->setIcon(EditIcon);
    edit->setText("EDIT");
    edit->setEvent(label_event->getEvent());
    table->setCellWidget(z, 3, edit);
    connect(edit, &QPushButtonExtended::on_click_edit, this,
            &MainCalendar::on_button_edit_click);
    QPushButtonExtended *deleteButton = new QPushButtonExtended("DELETE");
    QPixmap pixmapD(DELETE_PATH);
    QIcon DeleteIcon(pixmapD);
    deleteButton->setIcon(DeleteIcon);
    deleteButton->setText("DELETE");
    connect(deleteButton, &QPushButtonExtended::on_click_delete, this,
            &MainCalendar::on_button_delete_click);
    deleteButton->setEvent(label_event->getEvent());
    table->setCellWidget(z, 4, deleteButton);
    z++;
  }

  QVBoxLayout *main_layout = new QVBoxLayout;
  main_layout->addWidget(label_day);
  main_layout->addWidget(table);
  CustomDialog *custom_dialog = new CustomDialog(main_layout);
  custom_dialog->setFixedWidth(600);
  custom_dialog->setWindowTitle("Event of the Day");
  custom_dialog->show();
}

QLabelEvent *MainCalendar::createLabelEvent(CalendarEvent *event) {
  // Make a copy
  CalendarEvent *newEvent = new CalendarEvent(*event);
  QLabelEvent *label_event = new QLabelEvent;
  label_event->setEvent(newEvent);
  label_event->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  connect(label_event, &QLabelEvent::clicked, this,
          &MainCalendar::on_event_click);
  return label_event;
}

void MainCalendar::on_button_delete_click(QPushButtonExtended *d) {
  CalendarEvent *e = nullptr;
  if (d != nullptr)
    e = d->getEvent();
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Delete Event",
                                "Are you sure to delete event?",
                                QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    // QApplication::quit();
    emit delete_event(e);
  }
}

void MainCalendar::on_button_edit_click(QPushButtonExtended *d) {
  CalendarEvent *e = nullptr;
  if (d != nullptr) {
    e = d->getEvent();
    emit modifyEvent(e);
  }
}

void MainCalendar::on_event_click(QLabelEvent *label_event,
                                  Qt::MouseButton button) {
  CalendarEvent *e = nullptr;
  if (label_event != nullptr) {
    e = label_event->getEvent();
    emit modifyEvent(e);
  }
}

void MainCalendar::display_days(Date date) {
  // Update the label that contains month and year
  this->labelDate->setText(
      QString(DateUtil::get_literal_month(date.getMonth()).c_str()) +
      QString("    ") + QString::number(date.getYear()));
  // The current time is needed to highlight the current day
  Date current_date = DateUtil::get_current_date();
  int tot_days = DateUtil::get_days_in_month(date.getMonth(), date.getYear());
  int i, x, start_wday;

  // first week day of the current month
  start_wday = DateUtil::get_first_day_of_month(date).getWeekDay();
  x = 1;
  for (i = 0; i < 42; i++) {
    // Set an invalid date
    this->frames[i]->setDate(NULL);

    // Delete all the event labels inside the frame
    remove_events_from_frame(i);

    QLabel *day = static_cast<QLabel *>(this->frames[i]->children().at(1));
    day->setMaximumWidth(40);
    day->setObjectName("");
    day->setText("");
    // Checks right cells that will contain the days
    if ((i > 6 || // if I'm after the first week or
         (i % 7 >=
          start_wday -
              1)) &&       // if I'm in the first week and I'm in the right days
        (x <= tot_days)) { // and I'm not out of bound
      this->frames[i]->setDate(new Date(x, (start_wday + (x - 1)) % 7,
                                        date.getMonth(), date.getYear()));
      // insert into the label the zero-padded number of the day
      day->setText(QString("%1").arg(x, 2, 10));
      // Checks current day
      if ((x == current_date.getMonthDay()) &&
          (date.getMonth() == current_date.getMonth()) &&
          (date.getYear() == current_date.getYear()))
        day->setObjectName("today");
      this->frames[i]->layout()->addWidget(day);
      // check if the current frame refers to an holiday (i.e. saturday or
      // sunday)
      if ((i % 7) > 4) { // frame in the last two columns
        // mark the cell as an holiday
        this->frames[i]->setObjectName("holiday");
      } else { // is a generic day
        // mark the cell as enabled
        this->frames[i]->setObjectName("enabled");
      }
      x++;
    } else {
      // mark the cell as disabled
      this->frames[i]->setObjectName("disabled");
    }
    // Refresh the css rules
    this->frames[i]->setStyleSheet(_colorStyle.getCellStyle());
  }
}

QFrameExtended *MainCalendar::createQFrameExtended(Date *date) {
  QFrameExtended *frame = new QFrameExtended;
  QVBoxLayout *vl = new QVBoxLayout;
  frame->setDate(date);
  frame->setObjectName("day");
  vl->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  // vl->setMargin(0);
  vl->setSpacing(1);
  vl->addWidget(new QLabel); // qui scrivo in cella
  frame->setMinimumWidth(120);
  frame->setMinimumHeight(100);
  frame->setMaximumWidth(120);
  frame->setMaximumHeight(100);
  frame->setLayout(vl);
  frame->setStyleSheet(_colorStyle.getCellStyle());
  frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  return frame;
}

MainCalendar::~MainCalendar() {
  // delete this->previewLayout;
  /* this->selection_start and this->selection_end are pointers to dates wrapped
   * inside some QFrameExtended widgets. When Qt frees QFrameExtended widgets
   * they will free their dates.
   */
}