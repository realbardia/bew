#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "bew_globals.h"

#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QSettings s;
    ui->proxyGroup->setChecked( s.value("Proxy/active").toBool() );
    ui->proxyType->setCurrentText( s.value("Proxy/type", QStringLiteral("noproxy")).toString() );
    ui->proxyHost->setText( s.value("Proxy/host").toString() );
    ui->proxyPort->setValue( s.value("Proxy/port").toInt() );
    ui->proxyUser->setText( s.value("Proxy/user").toString() );
    ui->proxyPass->setText( s.value("Proxy/pass").toString() );

    ui->useragent->setText( s.value("userAgent").toString() );
    ui->font->setCurrentText( s.value("font").toString() );
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    QSettings s;

    s.setValue("Proxy/active", ui->proxyGroup->isChecked());
    s.setValue("Proxy/type", ui->proxyType->currentText());
    s.setValue("Proxy/host", ui->proxyHost->text());
    s.setValue("Proxy/port", ui->proxyPort->value());
    s.setValue("Proxy/user", ui->proxyUser->text());
    s.setValue("Proxy/pass", ui->proxyPass->text());

    s.setValue("userAgent", ui->useragent->text());
    s.setValue("font", ui->font->currentText());

    QDialog::accept();
}
