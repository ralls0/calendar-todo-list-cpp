/**
 *
 * @author  Marco Manco
 * @date    29/11/21.
 * @file    NewEventDialog.cpp
 * @brief
 *
 */

#include "NewEventDialog.h"

#define DEBUG_OAUTH 1
#if DEBUG_OAUTH
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

NewEventDialog::NewEventDialog(QWidget *parent) : QDialog(parent) {

  bg_rgb = QColor(QWidget::palette().color(QWidget::backgroundRole()));
  fg_rgb = QColor(QWidget::palette().color(QWidget::foregroundRole()));

  createBaseInfoLayout();
  createButtonGroupBox();

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(gb_baseInfo, 0, 0);
  layout->addWidget(_buttonBox, 2, 0, Qt::AlignRight);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  setWindowTitle(tr("New Event"));
}

void NewEventDialog::createBaseInfoLayout() {
  gb_baseInfo = new QGroupBox;
  gb_baseInfo->setFlat(true);
  gb_baseInfo->setStyleSheet("border:0;");

  QString t_styleSheet =
      QString("QLineEdit {font-size: 24px; font-weight: bold;  border-bottom: "
              "3px solid #232323; border-color: rgb(%1,%2,%3); "
              "background-color: rgb(%4,%5,%6);}")
          .arg(fg_rgb.red())
          .arg(fg_rgb.green())
          .arg(fg_rgb.blue())
          .arg(bg_rgb.red())
          .arg(bg_rgb.green())
          .arg(bg_rgb.blue());
  le_title = new QLineEdit;
  le_title->setPlaceholderText(tr("Title"));
  le_title->setObjectName("Title");
  le_title->setStyleSheet(t_styleSheet);

  _groupBox = new QGroupBox();

  rb_event = new QRadioButton("Event");
  rb_activity = new QRadioButton("Activity");

  rb_event->setChecked(true);

  _vbox = new QHBoxLayout;
  _vbox->addWidget(rb_event);
  _vbox->addWidget(rb_activity);
  _vbox->addStretch(1);

  _groupBox->setLayout(_vbox);

  createEventLayout();
  createActivityLayout();

  connect(rb_event, &QRadioButton::toggled, e_event, &QWidget::setVisible);
  connect(rb_event, &QRadioButton::toggled, e_activity, &QWidget::setHidden);
  connect(rb_activity, &QRadioButton::toggled, e_activity,
          &QWidget::setVisible);
  connect(rb_activity, &QRadioButton::toggled, e_event, &QWidget::setHidden);

  _baseInfoLayout = new QGridLayout;
  _baseInfoLayout->addWidget(le_title, 0, 0);
  _baseInfoLayout->addWidget(_groupBox, 1, 0);
  _baseInfoLayout->addWidget(e_event, 2, 0);
  _baseInfoLayout->addWidget(e_activity, 3, 0);
  gb_baseInfo->setLayout(_baseInfoLayout);
}

void NewEventDialog::createEventLayout() {
  e_event = new QWidget;

  QString dte_styleSheet =
      QString("QDateTimeEdit {border-bottom: "
              "3px solid #232323; border-color: rgb(%1,%2,%3); "
              "background-color: rgb(%4,%5,%6);}")
          .arg(fg_rgb.red())
          .arg(fg_rgb.green())
          .arg(fg_rgb.blue())
          .arg(bg_rgb.red())
          .arg(bg_rgb.green())
          .arg(bg_rgb.blue());
  dte_startDateE = new QDateTimeEdit(QDate::currentDate());
  dte_startDateE->setMinimumDate(QDate::currentDate().addDays(-365));
  dte_startDateE->setMaximumDate(QDate::currentDate().addDays(365));
  dte_startDateE->setDisplayFormat("yyyy/MM/dd");
  dte_startDateE->setStyleSheet(dte_styleSheet);

  dte_endDateE = new QDateTimeEdit(QDate::currentDate());
  dte_endDateE->setMinimumDate(QDate::currentDate().addDays(-365));
  dte_endDateE->setMaximumDate(QDate::currentDate().addDays(365));
  dte_endDateE->setDisplayFormat("yyyy/MM/dd");
  dte_endDateE->setStyleSheet(dte_styleSheet);

  QString cb_styleSheet =
      QString("QComboBox {border-bottom: "
              "3px solid #232323; border-color: rgb(%1,%2,%3); "
              "background-color: rgb(%4,%5,%6);}")
          .arg(fg_rgb.red())
          .arg(fg_rgb.green())
          .arg(fg_rgb.blue())
          .arg(bg_rgb.red())
          .arg(bg_rgb.green())
          .arg(bg_rgb.blue());
  cb_rrule = new QComboBox;
  cb_rrule->addItem("Non si ripete");
  cb_rrule->addItem("Ogni giorno");
  cb_rrule->addItem("Ogni settimana");
  cb_rrule->addItem("Ogni mese");
  cb_rrule->addItem("Ogni anno");
  cb_rrule->setStyleSheet(cb_styleSheet);

  QString le_styleSheet =
      QString("QLineEdit {border-bottom: "
              "3px solid #232323; border-color: rgb(%1,%2,%3); "
              "background-color: rgb(%4,%5,%6);}")
          .arg(fg_rgb.red())
          .arg(fg_rgb.green())
          .arg(fg_rgb.blue())
          .arg(bg_rgb.red())
          .arg(bg_rgb.green())
          .arg(bg_rgb.blue());
  le_location = new QLineEdit;
  le_location->setPlaceholderText("Location");
  le_location->setStyleSheet(le_styleSheet);
  _pixmap = QPixmap("../img/place.png");
  _pixmap = _pixmap.scaled(QSize(18, 18), Qt::KeepAspectRatio);
  lbl_location = new QLabel;
  lbl_location->setPixmap(_pixmap);
  lbl_location->setMask(_pixmap.mask());

  te_descriptionE = new QTextEdit;
  te_descriptionE->setMaximumHeight(100);
  te_descriptionE->setPlaceholderText("Description");

  cb_calendar = new QComboBox;
  cb_calendar->addItem("Google");
  cb_calendar->addItem("Sabre");
  cb_calendar->addItem("NextCloud");
  cb_calendar->setStyleSheet(cb_styleSheet);

  _eventLayout = new QGridLayout;
  _eventLayout->addWidget(dte_startDateE, 0, 0);
  _eventLayout->addWidget(dte_endDateE, 0, 1);
  _eventLayout->addWidget(cb_rrule, 1, 0);
  _eventLayout->addWidget(le_location, 2, 0);
  _eventLayout->addWidget(lbl_location, 2, 1);
  _eventLayout->addWidget(cb_calendar, 3, 0, 3, 2);
  _eventLayout->addWidget(te_descriptionE, 6, 0, 4, 2);
  e_event->setLayout(_eventLayout);
  e_event->show();
}

void NewEventDialog::createActivityLayout() {
  e_activity = new QWidget;

  dte_startDateA = new QDateTimeEdit(QDate::currentDate());
  dte_startDateA->setMinimumDate(QDate::currentDate().addDays(-365));
  dte_startDateA->setMaximumDate(QDate::currentDate().addDays(365));
  dte_startDateA->setDisplayFormat("yyyy.MM.dd");

  dte_endDateA = new QDateTimeEdit(QDate::currentDate());
  dte_endDateA->setMinimumDate(QDate::currentDate().addDays(-365));
  dte_endDateA->setMaximumDate(QDate::currentDate().addDays(365));
  dte_endDateA->setDisplayFormat("yyyy.MM.dd");

  te_descriptionA = new QTextEdit;
  te_descriptionA->setPlaceholderText("Description");

  cb_activity = new QComboBox;
  cb_activity->addItem("Google A");
  cb_activity->addItem("Sabre A");
  cb_activity->addItem("NextCloud A");

  _eventLayout = new QGridLayout;
  _eventLayout->addWidget(dte_startDateA, 0, 0);
  _eventLayout->addWidget(dte_endDateA, 0, 1);
  _eventLayout->addWidget(cb_activity, 1, 0, 1, 2);
  _eventLayout->addWidget(te_descriptionA, 2, 0, 2, 2);
  e_activity->setLayout(_eventLayout);
  e_activity->hide();
}

void NewEventDialog::createButtonGroupBox() {
  btn_cancel = new QPushButton(tr("Cancel"));
  btn_cancel->setCheckable(true);
  btn_save = new QPushButton(tr("Add"));
  btn_save->setCheckable(true);

  connect(btn_save, &QPushButton::clicked, this, &NewEventDialog::onSaveClick);
  connect(btn_cancel, &QPushButton::clicked, this, &QWidget::close);

  _buttonBox = new QDialogButtonBox(Qt::Horizontal);
  _buttonBox->addButton(btn_cancel, QDialogButtonBox::RejectRole);
  _buttonBox->addButton(btn_save, QDialogButtonBox::AcceptRole);
}

void NewEventDialog::onSaveClick(void) {
  if (rb_event->isChecked()) {
    QDEBUG << "[i] Adding new event"
           << " with title:" << le_title->text()
           << "start date:" << dte_startDateE->dateTime().toString()
           << "end date:" << dte_endDateE->dateTime().toString()
           << "rrule:" << cb_rrule->currentText()
           << "location:" << le_location->text()
           << "description:" << te_descriptionE->toPlainText() << "\n";
    QString uid = "";
    QString filename = "";
    QString exdate = "";
    QString summary = le_title->text();
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
    emit newEvent(uid, filename, summary, location, description, rrule,
                  startDateTime, endDateTime);
  } else {
    // FIXME
    emit newAction();
  }
  this->close();
}
