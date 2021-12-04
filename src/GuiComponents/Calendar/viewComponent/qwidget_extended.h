#ifndef QWIDGETEXTENDED_H
#define QWIDGETEXTENDED_H

#include <QWidget>
#include <QString>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>
#include "qframe_extended.h"

enum { PRESSED, RELEASED, MOVED };

class QWidgetExtended : public QWidget
{
    Q_OBJECT

private:
    void signalEvent(QMouseEvent *, int code);

public:
    explicit QWidgetExtended(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *pe);

signals:
    void mousePress(QFrameExtended *);
    void mouseRelease(QFrameExtended *);
    void mouseMove(QFrameExtended *);

};

#endif

