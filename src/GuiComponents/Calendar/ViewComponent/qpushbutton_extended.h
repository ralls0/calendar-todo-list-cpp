#ifndef QPUSHBUTTONEXTENDED_H
#define QPUSHBUTTONEXTENDED_H

#include <QPushButton>
#include "Model/event.h"

class QPushButtonExtended : public QPushButton
{
    Q_OBJECT
private:
    int index;
    Event* ev;

public:
    explicit QPushButtonExtended(const char *text, QWidget *parent = 0);
    QPushButtonExtended(QWidget *parent = 0);
    void setIndex(int index);
    void button_clicked();
    void button_edit_clicked();
    void setEvent(Event *event){
        ev = event;
    };
    Event* getEvent(){return ev;};

signals:
    void on_click(int index);;
    void on_click_edit(QPushButtonExtended *d);
public slots:
};

#endif // QPUSHBUTTONEXTENDED_H
