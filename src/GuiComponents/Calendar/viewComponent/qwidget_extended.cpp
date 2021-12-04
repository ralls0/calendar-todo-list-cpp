#include "qwidget_extended.h"
#include "qframe_extended.h"
#include <QDebug>

QWidgetExtended::QWidgetExtended(QWidget *parent) : QWidget(parent)
{

}

void QWidgetExtended::signalEvent(QMouseEvent *event, int code) {
    QFrameExtended frame;
    QWidget *widget = this->childAt(event->pos());
    if (widget != NULL) {
        QString widgetClassName(widget->metaObject()->className());
        //I don't use explicitly the string because if one day someone changes the name of the class, the compiler will output an error
        QString className(frame.metaObject()->className());
        if (widgetClassName == className) {
            switch(code) {
                case PRESSED:
                    emit mousePress(dynamic_cast<QFrameExtended*> (widget));
                    break;
                case RELEASED:
                    emit mouseRelease(dynamic_cast<QFrameExtended*> (widget));
                    break;
                case MOVED:
                    emit mouseMove(dynamic_cast<QFrameExtended*> (widget));
                    break;
            }
        }
    }
}

void QWidgetExtended::mousePressEvent(QMouseEvent *event) {
    signalEvent(event, PRESSED);
}

void QWidgetExtended::mouseReleaseEvent(QMouseEvent *event) {
    signalEvent(event, RELEASED);
}

void QWidgetExtended::mouseMoveEvent(QMouseEvent *event) {
    signalEvent(event, MOVED);
}

/* This method needs to be overrided to allow CSS styling also for QWidgetExtended */
void QWidgetExtended::paintEvent(QPaintEvent *pe) {
    QStyleOption opt;
    opt.initFrom(this);
    QStylePainter p(this);
    p.drawPrimitive(QStyle::PE_Widget, opt);
}
