/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    NewCalendarDialog.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_NEWCALENDARDIALOG_H
#define CALENDAR_TODO_LIST_CPP_NEWCALENDARDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>

class NewCalendarDialog : public QDialog {
  Q_OBJECT

public:
  NewCalendarDialog(QWidget *parent = nullptr);

public slots:
  void browseFile(void);
  void fileClientSecretChanged(const QString &path);
  void onAddClick(void);

signals:
  void newCalendar(const QString &displayName, const QString &hostURL,
                   bool isBasicAuth, const QString &username,
                   const QString &password, const QString &clientSecret);

private:
  void createBaseInfoGroupBox();
  void createAuthGroupBox();
  void createButtonGroupBox();
  QGroupBox *_baseInfoGroupBox;
  QGridLayout *_baseInfoLayout;
  QGroupBox *_authGroupBox;
  QGridLayout *_authLayout;

  QLabel *lbl_displayName;
  QLineEdit *le_displayName;
  QLabel *lbl_hostURL;
  QLineEdit *le_hostURL;
  QLabel *lbl_username;
  QLineEdit *le_username;
  QLabel *lbl_password;
  QLineEdit *le_password;
  QLabel *lbl_clientSecret;
  QLineEdit *le_clientSecret;
  QPushButton *btn_clientSecret;
  QFileDialog *fd_clientSecret;

  QGroupBox *_groupBox;
  QHBoxLayout *_vbox;
  QRadioButton *rb_basicAuth;
  QRadioButton *rb_oAuth;

  QDialogButtonBox *_buttonBox;
  QPushButton *btn_cancel;
  QPushButton *btn_add;

  QWidget *e_basic;
  QWidget *e_oauth;
};

#endif // CALENDAR_TODO_LIST_CPP_NEWCALENDARDIALOG_H
