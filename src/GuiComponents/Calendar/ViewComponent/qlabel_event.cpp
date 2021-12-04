#include "qlabel_event.h"

#include <QDebug>

QLabelEvent::QLabelEvent(QWidget *parent) : QLabel(parent)
{
    this->event = NULL;
}

QLabelEvent::~QLabelEvent() {
    if (this->event != NULL)
        delete this->event;
}

void QLabelEvent::mousePressEvent(QMouseEvent *e) {
    emit clicked(this, e->button());
}

void QLabelEvent::keyPressEvent(QKeyEvent *e) {
    emit keyPressed(e->key());
}

void QLabelEvent::setEvent(Event *event) {
    if (this->event != NULL) delete this->event;
    this->event = event;
    this->drawUI();
}

Event* QLabelEvent::getEvent() {
   return this->event;
}

bool QLabelEvent::markSelection(bool selected) {
    if (this->event == NULL) return false;
    if (selected)
        this->setObjectName("selected");
    else
        this->setObjectName("");
    //Update the ui
    this->setStyleSheet(this->styleSheet());
    return true;
}

bool QLabelEvent::drawInvalidEvent() {
    if (this->event == NULL) return false;
    this->setText(QString("<s>") + QString(this->event->getName().c_str()) + QString("</s>"));
    this->setStyleSheet(QString("QLabel { font-size: 14px; background-color : #AAAAAA; color: #000000; };"));
    this->setFixedHeight(26);
    this->setMargin(0);
    this->setEnabled(false);
    return true;
}

bool QLabelEvent::drawUI() {
    if (this->event == NULL) return false;
    if (this->event->isInvalid()) return drawInvalidEvent();

    this->setText(this->event->getName().c_str());
    QString textColor("#000000");
    if (is_color_dark(this->event->getCategory()->getColor()))
        textColor = "#FFFFFF";
    this->setStyleSheet(QString("QLabel#selected {border-top: 1px solid #FF0000; border-bottom: 1px solid #FF0000; font-weight: bold; font-size: 13px;} QLabel { font-size: 14px; border-radius: 2px; background-color : ") + QString(this->event->getCategory()->getColor().c_str()) + QString("; color: ") + textColor + QString("};"));
    this->setFixedHeight(26);
    this->setMargin(0);
    QString tooltip_text;
    if (this->event->getName().length() > 20)
        tooltip_text = QString("<b>Name: </b>") + this->event->getName().c_str() + QString("\n");
    if (this->event->getPlace() != "")
        tooltip_text = tooltip_text + QString("<b>PlaceGialo: </b>") + this->event->getPlace().c_str() + QString("\n");
    if (this->event->getDescription() != "")
        tooltip_text = tooltip_text + QString("<b>Description: </b>") + this->event->getDescription().c_str();
    this->setToolTip(tooltip_text);
    return true;
}

/* This is an utility function and should be moved in another class, but for the moment no one else is using it. */
bool QLabelEvent::is_color_dark(string colorName) {
    QColor color(colorName.c_str());
    //Formula to calculate luminance from ITU-R BT.709
    int l = 0.2126 * color.red() + 0.7152 * color.green() + 0.0722 * color.blue();
    if (l < 50)
        return true;
    else
        return false;
}

