#include "bewsharedialog.h"
#include "ui_bewsharedialog.h"

#include <QDesktopServices>
#include <QUrl>

BEWShareDialog::BEWShareDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BEWShareDialog)
{
    ui->setupUi(this);
}

BEWShareDialog::~BEWShareDialog()
{
    delete ui;
}

void BEWShareDialog::setJson(const QString &json)
{
    ui->json->setPlainText(json);
}

void BEWShareDialog::on_githubBtn_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/realbardia/bew"));
}
