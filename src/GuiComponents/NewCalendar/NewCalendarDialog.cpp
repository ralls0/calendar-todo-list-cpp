/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    NewCalendarDialog.cpp
 * @brief
 *
 */

#include "NewCalendarDialog.h"

NewCalendarDialog::NewCalendarDialog(QWidget *parent) : QDialog(parent) {

  createBaseInfoGroupBox();
  createAuthGroupBox();
  createButtonGroupBox();

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(_baseInfoGroupBox, 0, 0);
  layout->addWidget(_authGroupBox, 1, 0);
  layout->addWidget(_buttonBox, 2, 0, Qt::AlignRight);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  setWindowTitle(tr("Adding Calendar"));
}

NewCalendarDialog::~NewCalendarDialog() {
  delete _baseInfoGroupBox;
  delete _baseInfoLayout;
  delete _authGroupBox;
  delete _authLayout;
}

void NewCalendarDialog::createBaseInfoGroupBox() {
  _baseInfoGroupBox = new QGroupBox(tr("Base Information"));

  lbl_displayName = new QLabel(tr("&Display Name:"));
  le_displayName = new QLineEdit;
  lbl_displayName->setBuddy(le_displayName);

  lbl_hostURL = new QLabel(tr("&Host URL:"));
  le_hostURL = new QLineEdit;
  lbl_hostURL->setBuddy(le_hostURL);

  _baseInfoLayout = new QGridLayout;
  _baseInfoLayout->addWidget(lbl_displayName, 0, 0, Qt::AlignLeft);
  _baseInfoLayout->addWidget(le_displayName, 0, 1, Qt::AlignCenter);
  _baseInfoLayout->addWidget(lbl_hostURL, 1, 0, Qt::AlignLeft);
  _baseInfoLayout->addWidget(le_hostURL, 1, 1, Qt::AlignCenter);
  _baseInfoGroupBox->setLayout(_baseInfoLayout);
}

void NewCalendarDialog::createAuthGroupBox() {
  _authGroupBox = new QGroupBox(tr("Authentication"));

  _groupBox = new QGroupBox();
  rb_basicAuth = new QRadioButton("Basic Authentication");
  rb_oAuth = new QRadioButton("OAuth 2 Authentication");
  rb_basicAuth->setChecked(true);
  _vbox = new QHBoxLayout;
  _vbox->addWidget(rb_basicAuth);
  _vbox->addWidget(rb_oAuth);
  _vbox->addStretch(1);
  _groupBox->setLayout(_vbox);

  e_basic = new QWidget;
  e_oauth = new QWidget;

  lbl_username = new QLabel(tr("Username:"));
  le_username = new QLineEdit;
  lbl_username->setBuddy(le_username);

  lbl_password = new QLabel(tr("Password:"));
  le_password = new QLineEdit;
  lbl_password->setBuddy(le_password);

  lbl_clientSecret = new QLabel(tr("Client Secret:"));
  le_clientSecret = new QLineEdit;
  le_clientSecret->setDisabled(true);
  btn_clientSecret = new QPushButton(tr("Browse..."));
  connect(btn_clientSecret, &QPushButton::clicked, this,
          &NewCalendarDialog::browseFile);
  lbl_clientSecret->setBuddy(le_clientSecret);

  connect(rb_basicAuth, &QRadioButton::toggled, e_basic, &QWidget::setVisible);
  connect(rb_basicAuth, &QRadioButton::toggled, e_oauth, &QWidget::setHidden);
  connect(rb_oAuth, &QRadioButton::toggled, e_oauth, &QWidget::setVisible);
  connect(rb_oAuth, &QRadioButton::toggled, e_basic, &QWidget::setHidden);

  QGridLayout *e_basicLayout = new QGridLayout;
  e_basicLayout->addWidget(lbl_username, 0, 0, Qt::AlignLeft);
  e_basicLayout->addWidget(le_username, 0, 1, Qt::AlignCenter);
  e_basicLayout->addWidget(lbl_password, 1, 0, Qt::AlignLeft);
  e_basicLayout->addWidget(le_password, 1, 1, Qt::AlignCenter);
  e_basic->setLayout(e_basicLayout);
  e_basic->show();

  QGridLayout *e_oauthLayout = new QGridLayout;
  e_oauthLayout->addWidget(lbl_clientSecret, 0, 0, Qt::AlignLeft);
  e_oauthLayout->addWidget(le_clientSecret, 0, 1, Qt::AlignCenter);
  e_oauthLayout->addWidget(btn_clientSecret, 0, 2, Qt::AlignCenter);
  e_oauth->setLayout(e_oauthLayout);
  e_oauth->hide();

  _authLayout = new QGridLayout;
  _authLayout->addWidget(_groupBox, 0, 0, Qt::AlignLeft);
  _authLayout->addWidget(e_basic, 1, 0, Qt::AlignLeft);
  _authLayout->addWidget(e_oauth, 2, 0, Qt::AlignLeft);
  _authGroupBox->setLayout(_authLayout);
}

void NewCalendarDialog::createButtonGroupBox() {
  btn_cancel = new QPushButton(tr("Cancel"));
  btn_cancel->setCheckable(true);
  btn_add = new QPushButton(tr("Add"));
  btn_add->setCheckable(true);

  connect(btn_add, &QPushButton::clicked, this, &NewCalendarDialog::onAddClick);
  connect(btn_cancel, &QPushButton::clicked, this, &QWidget::close);

  _buttonBox = new QDialogButtonBox(Qt::Horizontal);
  _buttonBox->addButton(btn_cancel, QDialogButtonBox::RejectRole);
  _buttonBox->addButton(btn_add, QDialogButtonBox::AcceptRole);
}

void NewCalendarDialog::browseFile(void) {
  fd_clientSecret = new QFileDialog;
  connect(fd_clientSecret, &QFileDialog::currentChanged, this,
          &NewCalendarDialog::fileClientSecretChanged);
  fd_clientSecret->show();
}

void NewCalendarDialog::fileClientSecretChanged(const QString &path) {
  le_clientSecret->setText(path);
}

void NewCalendarDialog::onAddClick(void) {
  QDEBUG << "[i] Adding new calendar with displayName:"
         << le_displayName->text() << "hostURL:" << le_hostURL->text()
         << "basicAuth:" << rb_basicAuth->isChecked()
         << "username:" << le_username->text()
         << "password:" << le_password->text()
         << "clientSecret:" << le_clientSecret->text() << "\n";
  emit newCalendar(le_displayName->text(), le_hostURL->text(),
                   rb_basicAuth->isChecked(), le_username->text(),
                   le_password->text(), le_clientSecret->text());
  this->close();
}
