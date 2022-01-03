//
// Created by Davide on 03/01/2022.
//

#ifndef CALENDAR_TODO_LIST_CPP_QCHECKBOXEXTENDED_H
#define CALENDAR_TODO_LIST_CPP_QCHECKBOXEXTENDED_H
#include "../../Task/TaskElement.h"
#include <QCheckBox>

class QCheckBoxExtended : public QCheckBox  {
    Q_OBJECT
private:
    TaskElement *te;
public:
    explicit QCheckBoxExtended(QWidget *parent = 0);
    void checkbox_done_clicked(int x);
    void setTask(TaskElement *ted) { te = ted; };
    TaskElement *getTask() { return te; };

    signals:
            void on_click_done(QCheckBoxExtended *d);
};


#endif //CALENDAR_TODO_LIST_CPP_QCHECKBOXEXTENDED_H
