#ifndef QLABELEVENT_H
#define QLABELEVENT_H

#include <QLabel>
#include <QMouseEvent>
#include "model/event.h"

class QLabelEvent : public QLabel
{
    Q_OBJECT
private:
    Event *event;

public:
    explicit QLabelEvent(QWidget *parent = 0);
    ~QLabelEvent();
    bool markSelection(bool selected);
    void setEvent(Event *event);
    Event* getEvent();
    bool drawUI();
    bool is_color_dark(string colorName);

private:
    bool drawInvalidEvent();

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);

signals:
    void clicked(QLabelEvent *event, Qt::MouseButton button);
    void keyPressed(int key);

public slots:

};

#endif // QLABELEVENT_H
