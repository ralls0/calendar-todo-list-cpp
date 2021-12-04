/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    ColorUtils.cpp
 * @brief
 *
 */

#include "ColorUtils.h"

WindowStyle::WindowStyle(QWidget *parent) {
  _bg = QColor(QWidget::palette().color(QWidget::backgroundRole()));
  bg_rgb =
      QString("rgb(%1,%2,%3)").arg(_bg.red()).arg(_bg.green()).arg(_bg.blue());
  _fg = QColor(QWidget::palette().color(QWidget::foregroundRole()));
  fg_rgb =
      QString("rgb(%1,%2,%3)").arg(_fg.red()).arg(_fg.green()).arg(_fg.blue());
  _holiday = QString("#302A32");
  _today = QString("#FD5545");
  _selected = QString("#FD5545");
}

QString WindowStyle::getMainWindowStyle() {
  return QString("#mainwindow { background-color: %1; border: 2px solid %2; }")
      .arg(bg_rgb)
      .arg(fg_rgb);
}

QString WindowStyle::getLabelDateStyle() {
  return QString("QLabel { color: %1; qproperty-alignment: AlignCenter; "
                 "padding-left: "
                 "100px; padding-right: 100px; font-size: 24px; font-weight: "
                 "bold; }")
      .arg(fg_rgb);
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
                 "QFrame#disabled { background-color: %1; }")
      .arg(bg_rgb)
      .arg(fg_rgb)
      .arg(_holiday)
      .arg(_today);
}