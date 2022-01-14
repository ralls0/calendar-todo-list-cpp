/**
 *
 * @author  Marco Manco Davide Manco
 * @date    29/11/21.
 * @file    NewEventDialog.cpp
 * @brief
 *
 */

#include "NewEventDialog.h"

NewEventDialog::NewEventDialog(CalendarEvent *event, TaskElement *te,
                               QWidget *parent)
    : QDialog(parent) {

  if (te) {
    _te = te;
    _event = nullptr;
    setWindowTitle(tr("Modify Task"));
  }

  if (event) {
    _event = event;
    _te = nullptr;
    setWindowTitle(tr("Modify Event"));
  }

  createBaseInfoLayout(QList<QString>(), _event, _te);
  createButtonGroupBox(_event, _te);

  _layout = new QGridLayout(this);
  _layout->addWidget(gb_baseInfo, 0, 0);
  _layout->addWidget(_buttonBox, 2, 0, Qt::AlignRight);
  _layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(_layout);

  this->setStyleSheet(_colorStyle.getDialogStyle());
}

NewEventDialog::NewEventDialog(QList<QString> cals, QWidget *parent)
    : QDialog(parent) {

  _event = nullptr;
  _te = nullptr;

  createBaseInfoLayout(cals);
  createButtonGroupBox();

  _layout = new QGridLayout(this);
  _layout->addWidget(gb_baseInfo, 0, 0);
  _layout->addWidget(_buttonBox, 2, 0, Qt::AlignRight);
  _layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(_layout);

  setWindowTitle(tr("Create"));
  this->setStyleSheet(_colorStyle.getDialogStyle());
}

NewEventDialog::~NewEventDialog() {
  delete _layout;
  delete gb_baseInfo;
}

void NewEventDialog::createBaseInfoLayout(QList<QString> cals,
                                          CalendarEvent *event,
                                          TaskElement *te) {
  gb_baseInfo = new QGroupBox(this);
  gb_baseInfo->setFlat(true);
  gb_baseInfo->setStyleSheet("border:0;");

  le_title = new QLineEdit(gb_baseInfo);
  le_title->setPlaceholderText(tr("Title"));
  le_title->setObjectName("title");
  if (event != nullptr)
    le_title->setText(event->name());
  if (te != nullptr)
    le_title->setText(te->getName());
  _groupBox = new QGroupBox(gb_baseInfo);

  rb_event = new QRadioButton("Event", _groupBox);
  rb_activity = new QRadioButton("Activity", _groupBox);

  createEventLayout(cals, event);
  createActivityLayout(cals, te);

  if (event == nullptr && te == nullptr) {
    rb_event->setChecked(true);
    e_activity->hide();
    e_event->show();
  }
  if (event != nullptr) {
    rb_event->setChecked(true);
    rb_activity->setDisabled(true);
    e_activity->hide();
    e_event->show();
  }
  if (te != nullptr) {
    rb_activity->setChecked(true);
    rb_event->setDisabled(true);
    e_event->hide();
    e_activity->show();
  }

  _vbox = new QHBoxLayout(_groupBox);
  _vbox->addWidget(rb_event);
  _vbox->addWidget(rb_activity);
  _vbox->addStretch(1);

  _groupBox->setLayout(_vbox);

  connect(rb_event, &QRadioButton::toggled, e_event, &QWidget::setVisible);
  connect(rb_event, &QRadioButton::toggled, e_activity, &QWidget::setHidden);
  connect(rb_activity, &QRadioButton::toggled, e_activity,
          &QWidget::setVisible);
  connect(rb_activity, &QRadioButton::toggled, e_event, &QWidget::setHidden);

  _baseInfoLayout = new QGridLayout(gb_baseInfo);
  _baseInfoLayout->addWidget(le_title, 0, 0);
  _baseInfoLayout->addWidget(_groupBox, 1, 0);
  _baseInfoLayout->addWidget(e_event, 2, 0);
  _baseInfoLayout->addWidget(e_activity, 3, 0);
  gb_baseInfo->setLayout(_baseInfoLayout);
}

void NewEventDialog::createEventLayout(QList<QString> cals,
                                       CalendarEvent *event) {
  e_event = new QWidget(gb_baseInfo);

  QDateTime startD, endD;
  if (event == nullptr) {
    startD = QDateTime::currentDateTime();
    endD = QDateTime::currentDateTime();
  } else {
    startD = event->getStartDateTime();
    endD = event->getEndDateTime();
  }
  dte_startDateE = new QDateTimeEdit(startD, e_event);
  dte_startDateE->setMinimumDate(QDate::currentDate().addDays(-365));
  dte_startDateE->setMaximumDate(QDate::currentDate().addDays(365));
  dte_startDateE->setDisplayFormat("yyyy/MM/dd hh:mm");

  dte_endDateE = new QDateTimeEdit(endD, e_event);
  dte_endDateE->setMinimumDate(QDate::currentDate().addDays(-365));
  dte_endDateE->setMaximumDate(QDate::currentDate().addDays(365));
  dte_endDateE->setDisplayFormat("yyyy/MM/dd hh:mm");

  cb_rrule = new QComboBox(e_event);
  cb_rrule->addItem("Non si ripete");
  cb_rrule->addItem("Ogni giorno");
  cb_rrule->addItem("Ogni settimana");
  cb_rrule->addItem("Ogni mese");
  cb_rrule->addItem("Ogni anno");

  le_location = new QLineEdit(e_event);
  le_location->setPlaceholderText("Location");
  if (event != nullptr)
    le_location->setText(event->location());
  _pixmap = QPixmap("../img/place.png");
  _pixmap = _pixmap.scaled(QSize(18, 18), Qt::KeepAspectRatio);
  lbl_location = new QLabel(e_event);
  lbl_location->setPixmap(_pixmap);
  lbl_location->setMask(_pixmap.mask());

  te_descriptionE = new QTextEdit(e_event);
  te_descriptionE->setMaximumHeight(100);
  te_descriptionE->setPlaceholderText("Description");
  if (event != nullptr)
    te_descriptionE->setText(event->description());

  cb_calendar = new QComboBox(e_event);
  if (!cals.isEmpty()) {
    cb_calendar->addItems(cals);
  } else {
    if (event != nullptr) {
      cb_calendar->addItem(QString(event->calendarName()));
    } else {
      cb_calendar->addItem(QString("Nessun Calendario"));
    }
  }

  _eventLayout = new QGridLayout(e_event);
  _eventLayout->addWidget(dte_startDateE, 0, 0);
  _eventLayout->addWidget(dte_endDateE, 0, 1);
  _eventLayout->addWidget(cb_rrule, 1, 0);
  _eventLayout->addWidget(le_location, 2, 0);
  _eventLayout->addWidget(lbl_location, 2, 1);
  _eventLayout->addWidget(cb_calendar, 3, 0, 3, 2);
  _eventLayout->addWidget(te_descriptionE, 6, 0, 4, 2);
  e_event->setLayout(_eventLayout);
}

void NewEventDialog::createActivityLayout(const QList<QString> &cals,
                                          TaskElement *te) {
  e_activity = new QWidget(gb_baseInfo);

  _deadLine = new QLabel("Deadline:", e_activity);

  if (te != nullptr)
    dte_deadline = new QDateTimeEdit(te->getDeadline(), e_activity);
  else
    dte_deadline = new QDateTimeEdit(QDate::currentDate(), e_activity);
  dte_deadline->setMinimumDate(QDate::currentDate().addDays(-365));
  dte_deadline->setMaximumDate(QDate::currentDate().addDays(365));
  dte_deadline->setDisplayFormat("yyyy.MM.dd");


  if (!cals.isEmpty()) {
      cb_activity = new QComboBox(e_activity);
    cb_activity->addItems(cals);
  }

  _activityLayout = new QGridLayout(e_activity);
  _activityLayout->addWidget(_deadLine, 0, 0);
  _activityLayout->addWidget(dte_deadline, 0, 1);
    if (!cals.isEmpty()) {
  _activityLayout->addWidget(cb_activity, 1, 0, 1, 2);
    }
  e_activity->setLayout(_activityLayout);
}

void NewEventDialog::createButtonGroupBox(CalendarEvent *event,
                                          TaskElement *te) {
  _buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  btn_cancel = new QPushButton(tr("Close"), _buttonBox);
  btn_cancel->setCheckable(true);
  QPixmap pixmapC(CLOSE_PATH);
  QIcon CloseIcon(pixmapC);
  btn_cancel->setIcon(CloseIcon);

  if (event != nullptr || te != nullptr)
    btn_save = new QPushButton(tr("Modify"), this);
  else
    btn_save = new QPushButton(tr("Add"), this);

  btn_save->setCheckable(true);
  if (!event && !te) {
    QPixmap pixmapA(ADD_PATH);
    QIcon AddIcon(pixmapA);
    btn_save->setIcon(AddIcon);
  } else {
    QPixmap pixmapE(EDIT_PATH);
    QIcon EditIcon(pixmapE);
    btn_save->setIcon(EditIcon);
  }
  if (event || te) {
    btn_delete = new QPushButton(tr("Delete"), _buttonBox);
    btn_delete->setCheckable(true);
    QPixmap pixmapD(DELETE_PATH);
    QIcon DeleteIcon(pixmapD);
    btn_delete->setIcon(DeleteIcon);
  }
  connect(btn_save, &QPushButton::clicked, this, &NewEventDialog::onSaveClick);
  connect(btn_cancel, &QPushButton::clicked, this, &QWidget::close);

  _buttonBox->addButton(btn_save, QDialogButtonBox::AcceptRole);

  if (event || te) {
    connect(btn_delete, &QPushButton::clicked, this,
            &NewEventDialog::onDeleteClick);
    _buttonBox->addButton(btn_delete, QDialogButtonBox::RejectRole);
  }
  _buttonBox->addButton(btn_cancel, QDialogButtonBox::RejectRole);
}

void NewEventDialog::onDeleteClick(void) { // TODO: add te
  if (rb_event->isChecked())
    emit deleteEvent(_event);
  if (rb_activity->isChecked())
    emit deleteTask();
  this->close();
};

void NewEventDialog::onSaveClick(void) {
  if (rb_event->isChecked()) {
    QString uid;
    if (_event) {
      QDEBUG << "[i] Modify event with UID: " << _event->getUID();
      uid = _event->getUID();
    } else {
      QDEBUG << "[i] Adding new event";
      uid = "";
    }
    QDEBUG << "in: " << cb_calendar->currentText()
           << "with title:" << le_title->text()
           << "start date:" << dte_startDateE->dateTime().toString()
           << "end date:" << dte_endDateE->dateTime().toString()
           << "rrule:" << cb_rrule->currentText()
           << "location:" << le_location->text()
           << "description:" << te_descriptionE->toPlainText() << "\n";

    QString filename = "";
    QString title = le_title->text();
    QString location = le_location->text();
    QString description = te_descriptionE->toPlainText();
    QString rrule = "";
    QString rule = cb_rrule->currentText();
    if (rule == "Non si ripete")
      rrule = "";
    if (rule == "Ogni giorno")
      rrule = "FREQ=DAILY;INTERVAL=1";
    if (rule == "Ogni settimana")
      rrule = "FREQ=WEEKLY;INTERVAL=1";
    if (rule == "Ogni mese")
      rrule = "FREQ=MONTHLY;INTERVAL=1";
    if (rule == "Ogni anno")
      rrule = "FREQ=YEARLY;INTERVAL=1";
    QDateTime startDateTime = dte_startDateE->dateTime();
    QDateTime endDateTime = dte_endDateE->dateTime();
    QString calendar = cb_calendar->currentText();
    emit newEvent(uid, filename, title, location, description, rrule,
                  startDateTime, endDateTime, calendar);
  } else if (rb_activity->isChecked()) {
    if (btn_save->text() == "Modify") {
      QDateTime endDateTime = dte_deadline->dateTime();
      QString title = le_title->text();
      if (_te->getType() == 0)
        emit modifyTask(title, endDateTime, _te->getId(), _te);
    } else if (btn_save->text() == "Add") {
      QString title = le_title->text();
      QDateTime endDateTime = dte_deadline->dateTime();
      emit newTask(title,endDateTime);
    }
  }
  this->close();
}
