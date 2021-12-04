#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QLabel>



class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDialog(QLayout *layout, QWidget *parent = 0);
    ~CustomDialog();

protected:
    virtual void keyPressEvent(QKeyEvent *e);


};

#endif
