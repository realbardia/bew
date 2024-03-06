#include "donatedialog.h"
#include "ui_donatedialog.h"

#include <QClipboard>
#include <QApplication>

DonateDialog::DonateDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DonateDialog)
{
    ui->setupUi(this);
}

DonateDialog::~DonateDialog()
{
    delete ui;
}

void DonateDialog::on_bitcoinCopy_clicked()
{
    qApp->clipboard()->setText( ui->bitcoin->text() );
}

void DonateDialog::on_ethereumCopy_clicked()
{
    qApp->clipboard()->setText( ui->ethereum->text() );
}

void DonateDialog::on_tonCopy_clicked()
{
    qApp->clipboard()->setText( ui->ton->text() );
}

void DonateDialog::on_fantomCopy_clicked()
{
    qApp->clipboard()->setText( ui->fantom->text() );
}

