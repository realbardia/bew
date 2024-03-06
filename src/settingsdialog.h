#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

public Q_SLOTS:
    void accept() Q_DECL_OVERRIDE;

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
