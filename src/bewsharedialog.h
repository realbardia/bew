#ifndef BEWSHAREDIALOG_H
#define BEWSHAREDIALOG_H

#include <QDialog>

namespace Ui {
class BEWShareDialog;
}

class BEWShareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BEWShareDialog(QWidget *parent = nullptr);
    ~BEWShareDialog();

    void setJson(const QString &json);

private Q_SLOTS:
    void on_githubBtn_clicked();

private:
    Ui::BEWShareDialog *ui;
};

#endif // BEWSHAREDIALOG_H
