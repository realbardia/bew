#include "bewsharedialog.h"
#include "ui_bewsharedialog.h"

#include <QDesktopServices>
#include <QUrl>

BewShareDialog::BewShareDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BewShareDialog)
{
    ui->setupUi(this);
}

BewShareDialog::~BewShareDialog()
{
    delete ui;
}

void BewShareDialog::setJson(const QString &json)
{
    ui->json->setPlainText(json);
}

void BewShareDialog::on_githubBtn_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/realbardia/bew"));
}
