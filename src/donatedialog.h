#ifndef DONATEDIALOG_H
#define DONATEDIALOG_H

#include <QDialog>

namespace Ui {
class DonateDialog;
}

class DonateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DonateDialog(QWidget *parent = nullptr);
    ~DonateDialog();

private slots:
    void on_bitcoinCopy_clicked();
    void on_ethereumCopy_clicked();
    void on_tonCopy_clicked();
    void on_fantomCopy_clicked();

private:
    Ui::DonateDialog *ui;
};

#endif // DONATEDIALOG_H
