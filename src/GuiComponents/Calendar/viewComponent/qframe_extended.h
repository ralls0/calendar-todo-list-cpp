#ifndef QFRAMEEXTENDED_H
#define QFRAMEEXTENDED_H

#include <QObject>
#include <QFrame>
#include <QDebug>
#include "util/dateutil.h"

class QFrameExtended : public QFrame
{
    Q_OBJECT

private:
    Date *date;

public:
    explicit QFrameExtended(QWidget *parent = 0) : QFrame(parent), date(NULL) { }
    ~QFrameExtended() { delete this->date; }
    Date* getDate() { return date; }
    void setDate(Date *d) {
        if (this->date != NULL) delete this->date;
        this->date = d;
    }

};

#endif // QFRAMEEXTENDED_H
