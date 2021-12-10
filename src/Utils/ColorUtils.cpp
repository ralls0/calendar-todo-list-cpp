/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    ColorUtils.cpp
 * @brief
 *
 */

#include "ColorUtils.h"

#define DEBUG_ 1
#if DEBUG_
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

WindowStyle::WindowStyle(QWidget *parent) {
  _bg = QColor(QWidget::palette().color(QPalette::Window));
  bg_hex = _bg.name();
  _fg = QColor(QWidget::palette().color(QPalette::WindowText));
  fg_hex = _fg.name();
  _dark = QString("#302A32");
  _red = QString("#FD5545");
  _selected = QString("#FD5545");
}

QString WindowStyle::getMainWindowStyle() {
  return QString("#mainwindow { background-color: %1; border: 2px solid %2; }")
      .arg(bg_hex)
      .arg(fg_hex);
}

QString WindowStyle::getLabelDateStyle() {
  return QString("QLabel { color: %1; qproperty-alignment: AlignLeft; "
                 "padding-right: 100px; font-size: 24px; font-weight: "
                 "bold; }")
      .arg(fg_hex);
}

QString WindowStyle::getCellStyle() {
  return QString("QFrame { background-color: %1; color: %2; border: 1px outset "
                 "%2;}"
                 "QFrame#holiday { background-color: %3; }"
                 "QLabel { border: none; font-size: 16px; padding: 5px; "
                 "background-color:rgba(0,0,0,0); }"
                 "QLabel#today { background-color: %4; color: %1;  border: 1px "
                 "solid %4; border-radius: 15px;}"
                 "QFrame#selected { background-color: #EEEEFF;  }"
                 "QLabel#header { font-weight: bold; margin-right: 0; }"
                 "QFrame#header { background-color: %1; border: 0; }"
                 "QFrame#disabled { background-color: %1; border: 0;}")
      .arg(bg_hex)
      .arg(fg_hex)
      .arg(_dark)
      .arg(_red);
}

QString WindowStyle::getDialogStyle() {
  return QString("QLineEdit, QTextEdit, QDateTimeEdit, QComboBox { "
                 "background-color: %1; border-bottom: 3px solid %2; } "
                 "QLineEdit#title { font-size: 24px; font-weight: bold; "
                 "background-color: %1; border-bottom: 3px solid %2;}")
      .arg(bg_hex)
      .arg(fg_hex);
}