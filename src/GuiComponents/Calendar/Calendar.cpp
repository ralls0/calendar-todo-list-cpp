/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Calendar.cpp
 * @brief
 *
 */

#include <QPainter>
#include <ctime>

#include "Calendar.h"

#define DEBUG_OAUTH 1
#if DEBUG_OAUTH
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

MainCalendar::MainCalendar(QWidget *parent) : QWidget(parent) {

    /* connect(_newCalendarDialog, &NewCalendarDialog::newCalendar, this,
             &WindowNewCalendar::createNewCalendar);*/
    Date current_date = DateUtil::get_current_date();
    this->API = new APImain;
    this->label_date = new QLabel;
    this->label_date->setMaximumHeight(40);
    this->label_date->setFixedWidth(400);
    this->label_date->setStyleSheet(_colorStyle.getLabelDateStyle());
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
    hl->addWidget(label_date, 1);
    hl->addWidget(wbutton, 1, Qt::AlignRight);
    this->layout =
            new QVBoxLayout; // layout verticale dove sopra bottoni sotto calendario
    this->layout->addLayout(hl);
    //setCalendarList();
    _calList = new QHBoxLayout;
    QLabel *ql = new QLabel("I tuoi calendari:" );
    _calList->addWidget(ql);
    _checkList = new QList<QCheckBox>;
    this->layout->addLayout(_calList);
    // Create 6x7 grid
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
        // hl->setMargin(0);
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

    // altra roba

    // grid_layout->setMargin(5);
    this->layout->addLayout(grid_layout);

    // Set layout in QWidget
    /*QWidgetExtended *window = new QWidgetExtended;
    window->setObjectName("mainwindow");
    window->setStyleSheet(MAINWINDOW_STYLE);
    window->setLayout(this->layout);
    window->setMinimumHeight(600);
    window->setMinimumWidth(1100);*/
    // Fill the grid with the days of the default month (i.e. the current month)
    display_days(current_date);
    display_events(current_date);
    setLayout(this->layout);

    // Set QWidget as the central layout of the main window

    /*createPreviewGroupBox();
     QGridLayout *layout = new QGridLayout;
     layout->addWidget(previewGroupBox, 0, 0);
     //layout->setSizeConstraint(QLayout::SetFixedSize);
     setLayout(layout);
     previewLayout->setRowMinimumHeight(0, calendar->sizeHint().height());
     previewLayout->setColumnMinimumWidth(0, calendar->sizeHint().width());
     setWindowTitle(tr("Calendar Widget"));*/
}

void MainCalendar::setCalendarList( QList<QString> t ){
    _checkList = new QList<QCheckBox>;
    for(QString x: t){
        QCheckBox *box1 = new QCheckBox(x);
        _calList->addWidget(box1);

    }
}

void MainCalendar::updateListOfEvents(QList<QObject *> t){
    std::cout<<"CIAONE";
    for(QObject* x: t){
        std::cout<<"ciaone";
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
    display_events(newDate);
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
    display_events(newDate);
}

void MainCalendar::display_events(Date date) {
    // list<Event*> event_list = this->pm->get_events_of_month(date.getMonth(),
    // date.getYear());
    /*struct tm *tinfo;
    time_t rawtime;
    time ( &rawtime );
    tinfo = localtime ( &rawtime );
    tinfo->tm_year=2021;
    tinfo->tm_mon=12;
    tinfo->tm_mday=3;
    time_t endd = mktime(tinfo);
    struct tm *tinfo2;
    time ( &rawtime );
    tinfo2 = localtime ( &rawtime );
    tinfo->tm_year=2021;
    tinfo->tm_mon=12;
    tinfo->tm_mday=2;
    time_t starta = mktime(tinfo2);*/
    time_t today, bday;
    struct tm *birthday;
    time(&today);
    birthday = localtime(&today);
    birthday->tm_mon = 12 - 1;
    birthday->tm_mday = 3;
    birthday->tm_year = 2021 - 1900;

    bday = mktime(birthday);
    std::list<Event *> event_list =
            this->API->getEventByMonth(date.getMonth(), date.getYear());

    // Event *b = new Event(3, "Festune2","" ,"casaMia", new
    // Category(3,"enigma","green"), time(0), time(0));
    /* Event *c = new Event(4, "Festune3","" ,"casaMia", new
     Category(3,"enigma","green"), time(0), time(0)); Event *d = new Event(5,
     "Festune4","" ,"casaMia", new Category(3,"enigma","green"), time(0),
     time(0)); Event *e = new Event(6, "Festune5","" ,"casaMia", new
     Category(3,"enigma","green"), time(0), time(0)); Event *f = new Event(7,
     "Festune6","" ,"casaMia", new Category(3,"enigma","green"), time(0),
     time(0));*/

    // event_list.push_front(b);
    /* event_list.push_front(c);
     event_list.push_front(d);
     event_list.push_front(e);
     event_list.push_front(f);*/
    int start_offset;
    Event *selected_event = NULL;
    // Check if there was a selected event
    /* if (this->selected_event != NULL) {
         selected_event = this->selected_event->getEvent();
     }*/

    // Remove all displayed events
    remove_events_from_all_frames();

    // Find at which cell the month starts
    for (start_offset = 0; start_offset < 42; start_offset++) {
        // Looks where is the first day of the month
        if (this->frames[start_offset]->getDate() != NULL)
            break;
    }
    // Add events to the gui
    for (Event *event : event_list) {

        // if ((category != NULL) && (!event->getCategory()->equals(*category)))
        // continue; //Don't add this event to the view
        Date start = DateUtil::date_from_timestamp(event->getStart());
        Date end = DateUtil::date_from_timestamp(event->getEnd());
        if (((start.getMonth() < date.getMonth()) &&
             (start.getYear() == date.getYear())) ||
            (start.getYear() < date.getYear()))
            start = DateUtil::get_first_day_of_month(date);
        if (((end.getMonth() > start.getMonth()) &&
             (end.getYear() == start.getYear())) ||
            (end.getYear() > start.getYear()))
            end = DateUtil::get_last_day_of_month(start);
        // sovrascrive tutto xk gli eventi vanno presi mese mese non c'è controllo
        // annoframe==annoevento && meseframe==meseevento
        for (int i = start_offset + start.getMonthDay() - 1;
             i < (start_offset + end.getMonthDay()); i++) {
            QLabelEvent *label_event = createLabelEvent(event);

            /*if ((selected_event != NULL) && (selected_event->equals(*event))) {
                label_event->markSelection(true);
                this->selected_event = label_event;
            }*/
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
            // if(this->frames[i]->getDate()->compareTo(event->get))
            (static_cast<QVBoxLayout *>(this->frames[i]->layout()))
                    ->insertWidget(1, label_event);
        }
        delete event;
    }
    /*if (this->selected_event != NULL)
        this->selected_event->setFocus();*/
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

QTime MainCalendar::timeToQTime(time_t datax) {
    int m = (datax / 60) % 60;
    int s = datax % 60;
    int h = (datax / 3600) % 24;
    QTime *dat = new QTime(h, m, s);
    return *dat;
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
            "background:linear-gradient(to " // box-shadow: 0px 1px 0px 0px #fff6af;
            "bottom, #ffec64 5%, #ffab23 100%);"
            " background-color:#ffec64; border-radius:6px; border:1px solid #ffaa22; "
            //"display:inline-block;cursor:pointer;"
            " color:#333333; font-family:Arial; font-size:15px; "
            "font-weight:bold;padding:6px 24px;text-decoration:none;"
            "  text-align: center;"); // text-shadow:0px 1px 0px #ffee66;
    // label_day->setText(text);
    // label_day->setStyleSheet("QLabel { width: 100%; background-color: #ffffb3;
    // border-bottom: 1px solid #000000; margin-bottom: 2px; }");
    char stime[14];
    int z = 0;
    QTableWidget *table =
            new QTableWidget(this->frames[index]->children().size() - 2, 5);
    table->setHorizontalHeaderItem(0,new QTableWidgetItem("NAME"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("DESCRIPTION"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("HOUR"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("EDIT"));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("DELETE"));
    table->setStyleSheet(
            "QHeaderView {background-color: transparent;} "
            "QHeaderView::section {background-color: #646464;"
            "padding: 4px;font-size: 10pt; "
            "border-style: none;  border-bottom: 1px solid #fffff8; "
            " border-right: 1px solid #fffff8;  }"
            " QHeaderView::section:horizontal{ border-top: 1px solid #fffff8;  }"
            "QHeaderView::section:vertical  { border-left: 1px solid #fffff8;   }");
    // QPushButton *edit = new QPushButton("EDIT");
    //  connect(edit, &QPushButton::clicked, this, &MainCalendar::on_event_click);
    for (QLabelEvent *label_event :
            this->frames[index]->findChildren<QLabelEvent *>()) {
        Event *event = new Event(*label_event->getEvent());
        // QTime start = QDateTime::fromTime_t(event->getStart()).time();
        QTime start = timeToQTime(event->getStart());
        QTime end = timeToQTime(event->getEnd());
        // QString descr = QString::fromStdString(event->getDescription());
        // QLabel *des = new QLabel(descr);
        snprintf(stime, 14, "%02d:%02d - %02d:%02d", start.hour(), start.minute(),
                 end.hour(), end.minute());
        QLabel *time = new QLabel(stime);
        table->setCellWidget(z, 0, createLabelEvent(event));
        table->setItem(
                z, 1,
                new QTableWidgetItem(QString::fromStdString(event->getDescription())));
        table->setItem(z, 2, new QTableWidgetItem(stime));
        QPushButtonExtended *edit = new QPushButtonExtended();
        edit->setText("EDIT");
        edit->setEvent(label_event->getEvent());
        table->setCellWidget(z, 3, edit);
        // CONNECTDASISTEMARE
        connect(edit, &QPushButtonExtended::on_click_edit, this,
                &MainCalendar::on_button_edit_click);
        table->setCellWidget(z, 4, new QPushButton("DELETE"));

        // hl->addWidget(createLabelEvent(event));
        // hl->addWidget(des);

        z++;
    }
    // QHBoxLayout *hl = new QHBoxLayout;
    // hl->addWidget(table);
    //(static_cast <QVBoxLayout*> (frame->layout()))->addLayout(hl); //TODO: add
    // the entries ordered by timestamp
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(label_day);
    main_layout->addWidget(table);
    CustomDialog *custom_dialog = new CustomDialog(main_layout);
    custom_dialog->setFixedWidth(600);
    custom_dialog->setWindowTitle("Event of the Day");
    custom_dialog->show();
}

QLabelEvent *MainCalendar::createLabelEvent(Event *event) {
    // Make a copy
    Event *newEvent = new Event(*event);
    QLabelEvent *label_event = new QLabelEvent;
    label_event->setEvent(newEvent);
    label_event->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(label_event, &QLabelEvent::clicked, this,
            &MainCalendar::on_event_click);
    // connect(label_event, &QLabelEvent::keyPressed, this,
    // &MainCalendar::on_event_key_press);
    return label_event;
}

void MainCalendar::on_button_edit_click(QPushButtonExtended *d) {
    Event *e = nullptr;
    if (d != nullptr)
        e = d->getEvent();
    NewEventDialog *eventDialog = new NewEventDialog(e);
    eventDialog->show();
}

void MainCalendar::on_event_click(QLabelEvent *label_event,
                                  Qt::MouseButton button) {
    Event *e = nullptr;
    if (label_event != nullptr)
        e = label_event->getEvent();
    NewEventDialog *eventDialog = new NewEventDialog(e);
    // eventDialog->setEvent(label_event->getEvent());
    eventDialog->show();
}

void MainCalendar::display_days(Date date) {
    // Update the label that contains month and year
    this->label_date->setText(
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
    frame->setMinimumWidth(100);
    frame->setMinimumHeight(80);
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

void MainCalendar::createPreviewGroupBox() {
    previewGroupBox = new QGroupBox(tr("Preview"));

    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setGridVisible(true);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    /*connect(calendar, &QCalendarWidget::currentPageChanged,
            this, &Window::reformatCalendarPage);*/
    calendar->adjustSize();
    previewLayout = new QGridLayout;
    previewLayout->addWidget(calendar, 0, 0, Qt::AlignCenter);
    previewGroupBox->setLayout(previewLayout);
}

/*void WindowNewCalendar::createNewCalendar(const QString &displayName,
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
}*/
/*
void WindowNewCalendar::createPreviewGroupBox() {
  _previewGroupBox = new QGroupBox(tr("Main"));
  QPushButton *btn_addCalendar = new QPushButton("Add Calendar", nullptr);
  connect(btn_addCalendar, &QPushButton::clicked, _newCalendarDialog,
          &QWidget::show);
  _previewLayout = new QGridLayout;
  _previewLayout->addWidget(btn_addCalendar, 0, 0, Qt::AlignCenter);
  _previewGroupBox->setLayout(_previewLayout);
}*/