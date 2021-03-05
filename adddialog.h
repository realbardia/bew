#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QWidget>

#include "ui_adddialog.h"

class AddDialog : public QDialog
{
    Q_OBJECT
public:
    AddDialog(QWidget *parent = Q_NULLPTR);
    virtual ~AddDialog();

public Q_SLOTS:
    virtual void accept();

Q_SIGNALS:

private:
    void checkSaveBtn();
    void selectIcon();

private:
    Ui::AddDialog ui;
};

#endif // ADDDIALOG_H
