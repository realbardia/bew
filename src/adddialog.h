#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QWidget>

#include "bewappitem.h"
#include "ui_adddialog.h"

class AddDialog : public QDialog
{
    Q_OBJECT
public:
    AddDialog(QWidget *parent = Q_NULLPTR);
    virtual ~AddDialog();

public Q_SLOTS:
    virtual void accept();
    void restoreBew(BewAppItemPtr item);

Q_SIGNALS:
    void refreshRequest(const BewAppItemPtr &from);

private Q_SLOTS:
    void on_cancelBtn_clicked();
    void on_uninstallBtn_clicked();
    void on_installBtn_clicked();
    void on_shareBtn_clicked();

    void on_saveBtn_clicked();

protected:
    BewAppItemPtr generateBew() const;

private:
    void checkSaveBtn();
    void selectIcon();

private:
    Ui::AddDialog ui;
    BewAppItemPtr mRestoreBew;
};

#endif // ADDDIALOG_H
