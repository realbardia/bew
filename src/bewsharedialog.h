#ifndef BEWSHAREDIALOG_H
#define BEWSHAREDIALOG_H

#include <QDialog>

namespace Ui {
class BewShareDialog;
}

class BewShareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BewShareDialog(QWidget *parent = nullptr);
    ~BewShareDialog();

    void setJson(const QString &json);

private Q_SLOTS:
    void on_githubBtn_clicked();

private:
    Ui::BewShareDialog *ui;
};

#endif // BEWSHAREDIALOG_H
