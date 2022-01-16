/**
 *
 * @author  Marco Manco
 * @date    27/12/21.
 * @file    ShareCalendarDialog.cpp
 * @brief
 *
 */

#include "ShareCalendarDialog.h"

ShareCalendarDialog::ShareCalendarDialog(QList<QString> cals, QWidget *parent)
    : QDialog(parent) {
  createBaseLayout(cals);
  createButtonGroupBox();

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(gb_base, 0, 0);
  layout->addWidget(_buttonBox, 2, 0, Qt::AlignRight);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  setWindowTitle(tr("Create"));
}

void ShareCalendarDialog::createBaseLayout(QList<QString> cals) {

  gb_base = new QGroupBox(this);
  gb_base->setFlat(true);
  gb_base->setStyleSheet("border:0;");

  lbl_email = new QLabel(tr("Email"), gb_base);
  le_email = new QLineEdit(gb_base);
  le_email->setPlaceholderText(tr("email"));

  cb_calendar = new QComboBox(gb_base);
  if (!cals.isEmpty()) {
    cb_calendar->addItems(cals);
  } else {
    cb_calendar->addItem(QString("Nessun Calendario"));
  }

  te_comment = new QTextEdit(gb_base);
  te_comment->setMaximumHeight(100);
  te_comment->setPlaceholderText("Comment");

  _baseLayout = new QGridLayout(gb_base);
  _baseLayout->addWidget(lbl_email, 0, 0);
  _baseLayout->addWidget(le_email, 0, 1);
  _baseLayout->addWidget(cb_calendar, 1, 0, 1, 2);
  _baseLayout->addWidget(te_comment, 2, 0, 2, 2);
  gb_base->setLayout(_baseLayout);
}

void ShareCalendarDialog::createButtonGroupBox() {
  btn_cancel = new QPushButton(tr("Close"), gb_base);
  btn_cancel->setCheckable(true);
  QPixmap pixmapC(CLOSE_PATH);
  QIcon CloseIcon(pixmapC);
  btn_cancel->setIcon(CloseIcon);
  btn_share = new QPushButton(tr("Share"), gb_base);
  btn_share->setCheckable(true);
  QPixmap pixmapA(ADD_PATH);
  QIcon AddIcon(pixmapA);
  btn_share->setIcon(AddIcon);

  connect(btn_share, &QPushButton::clicked, this,
          &ShareCalendarDialog::onShareClick);
  connect(btn_cancel, &QPushButton::clicked, this, &QWidget::close);

  _buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  _buttonBox->addButton(btn_share, QDialogButtonBox::AcceptRole);

  _buttonBox->addButton(btn_cancel, QDialogButtonBox::RejectRole);
}

void ShareCalendarDialog::onShareClick(void) {
  QDEBUG << "[i] Sharing calendar"
         << "in: " << cb_calendar->currentText()
         << "with email:" << le_email->text()
         << "and comment:" << te_comment->toPlainText() << "\n";

  QString email = le_email->text();
  QString displayName = cb_calendar->currentText();
  QString comment = te_comment->toPlainText();

  emit shareCalendar(displayName, email, comment);

  this->close();
}
