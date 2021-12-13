/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    QLabelEvent.cpp
 * @brief
 *
 */

#include "QLabelEvent.h"

#include <QDebug>

QLabelEvent::QLabelEvent(QWidget *parent) : QLabel(parent) {
  this->event = NULL;
}

QLabelEvent::~QLabelEvent() {
  if (this->event != NULL)
    delete this->event;
}

void QLabelEvent::mousePressEvent(QMouseEvent *e) {
  emit clicked(this, e->button());
}

void QLabelEvent::keyPressEvent(QKeyEvent *e) { emit keyPressed(e->key()); }

void QLabelEvent::setEvent(CalendarEvent *event) {
  if (this->event != NULL)
    delete this->event;
  this->event = event;
  this->drawUI();
}

CalendarEvent *QLabelEvent::getEvent() { return this->event; }

bool QLabelEvent::markSelection(bool selected) {
  if (this->event == NULL)
    return false;
  if (selected)
    this->setObjectName("selected");
  else
    this->setObjectName("");
  // Update the ui
  this->setStyleSheet(this->styleSheet());
  return true;
}

bool QLabelEvent::drawInvalidEvent() {
  if (this->event == NULL)
    return false;
  this->setText(QString("<s>") + QString(this->event->name()) +
                QString("</s>"));
  this->setStyleSheet(QString("QLabel { font-size: 14px; background-color : "
                              "#AAAAAA; color: #000000; };"));
  this->setFixedHeight(26);
  this->setMargin(0);
  this->setEnabled(false);
  return true;
}

bool QLabelEvent::drawUI() {
  if (this->event == NULL)
    return false;
  /*if (this->event->isInvalid())
    return drawInvalidEvent();*/

  this->setText(this->event->name());
  QString textColor("#000000");
  if (is_color_dark(this->event->getColor()))
    textColor = "#FFFFFF";
  this->setStyleSheet(
      QString("QLabel#selected {border-top: 1px solid #FF0000; border-bottom: "
              "1px solid #FF0000; font-weight: bold; font-size: 13px;} QLabel "
              "{ font-size: 14px; border-radius: 2px; background-color : ") +
      QString(this->event->getColor()) +
      QString("; color: ") + textColor + QString("};"));
  this->setFixedHeight(26);
  this->setMargin(0);
  QString tooltip_text;
  if (this->event->name().length() > 20)
    tooltip_text = QString("<b>Name: </b>") + this->event->name() +
                   QString("\n");
  if (this->event->location() != "")
    tooltip_text = tooltip_text + QString("<b>PlaceGialo: </b>") +
                   this->event->location() + QString("\n");
  if (this->event->description() != "")
    tooltip_text = tooltip_text + QString("<b>Description: </b>") +
                   this->event->description();
  this->setToolTip(tooltip_text);
  return true;
}

/* This is an utility function and should be moved in another class, but for the
 * moment no one else is using it. */
bool QLabelEvent::is_color_dark(QString colorName) {
  QColor color(colorName);
  // Formula to calculate luminance from ITU-R BT.709
  int l = 0.2126 * color.red() + 0.7152 * color.green() + 0.0722 * color.blue();
  if (l < 50)
    return true;
  else
    return false;
}
