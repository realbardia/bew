#include "adddialog.h"
#include "bewappitem.h"
#include "bewsharedialog.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.uninstallBtn->hide();
    ui.saveBtn->hide();

    connect(ui.title, &QLineEdit::textChanged, this, &AddDialog::checkSaveBtn);
    connect(ui.icon, &QLineEdit::textChanged, this, &AddDialog::checkSaveBtn);
    connect(ui.website, &QLineEdit::textChanged, this, &AddDialog::checkSaveBtn);
    connect(ui.iconBtn, &QToolButton::clicked, this, &AddDialog::selectIcon);

    checkSaveBtn();
}

AddDialog::~AddDialog()
{
}

void AddDialog::accept()
{
}

void AddDialog::restoreBEW(BEWAppItemPtr item)
{
    ui.website->setReadOnly(true);
    ui.title->setReadOnly(true);

    const auto installed = item->isInstalled();
    ui.installBtn->setVisible(!installed);
    ui.uninstallBtn->setVisible(installed);
    ui.saveBtn->setVisible(installed);

    ui.website->setText(item->url());
    ui.title->setText(item->title());
    ui.icon->setText(item->iconUrl());
    ui.scrollbar->setChecked(item->scrollBar());
    ui.singleInstance->setChecked(item->singleInstance());
    ui.systemTray->setChecked(item->systemTray());
    ui.useragent->setText(item->userAgent());
    ui.font->setCurrentText(item->fontFamily());

    if (item->proxy().has_value())
    {
        ui.proxyGroup->setChecked(true);
        const auto proxy = item->proxy().value();

        ui.proxyType->setCurrentText(proxy.type);
        ui.proxyHost->setText(proxy.host);
        ui.proxyPort->setValue(proxy.port);
        ui.proxyUser->setText(proxy.user);
        ui.proxyPass->setText(proxy.pass);
    }

    mRestoreBEW = item;
}

void AddDialog::checkSaveBtn()
{
    ui.installBtn->setEnabled(
        ui.title->text().length() &&
        ui.icon->text().length() &&
        ui.website->text().length()
    );
    ui.uninstallBtn->setEnabled(ui.installBtn->isEnabled());
    ui.saveBtn->setEnabled(ui.installBtn->isEnabled());
}

void AddDialog::selectIcon()
{
    auto path = QFileDialog::getOpenFileName(this, "Please select icon", "", "Images (*.png *.svg)");
    if (path.isEmpty())
        return;

    ui.icon->setText(path);
}

void AddDialog::on_cancelBtn_clicked()
{
    QDialog::reject();
}

void AddDialog::on_uninstallBtn_clicked()
{
    if (!mRestoreBEW)
        return;

    auto resp = QMessageBox::warning(this, "Uninstall", QString("Are you sure about uninstall \"%1\" bew?").arg(ui.title->text()), QMessageBox::Button::Yes|QMessageBox::Button::Cancel);
    if (resp == QMessageBox::Cancel)
        return;

    mRestoreBEW->uninstall();
    Q_EMIT refreshRequest(mRestoreBEW);
    close();
}

void AddDialog::on_installBtn_clicked()
{
    auto item = generateBEW();
    if (item->install())
    {
        Q_EMIT refreshRequest(item);
        QMessageBox::information(this, "Done", QString("Your BEW installed successfully at \"%1\".\nYou can find it at application menu.").arg(BEWAppItem::applicationsPath()));
        close();
    }
}

BEWAppItemPtr AddDialog::generateBEW() const
{
    auto item = BEWAppItemPtr::create();
    item->setUrl(ui.website->text());
    item->setTitle(ui.title->text());
    item->setIconUrl(ui.icon->text());
    item->setScrollBar(ui.scrollbar->isChecked());
    item->setSingleInstance(ui.singleInstance->isChecked());
    item->setSystemTray(ui.systemTray->isChecked());
    item->setFontFamily(ui.font->currentText());
    item->setUserAgent(ui.useragent->text());

    if (ui.proxyGroup->isChecked())
    {
        BEWAppItem::Proxy proxy;
        proxy.type = ui.proxyType->currentText();

        if (ui.proxyHost->text().length())
            proxy.host = ui.proxyHost->text();
        if (ui.proxyPort->value())
            proxy.port = ui.proxyPort->value();
        if (ui.proxyUser->text().length())
            proxy.user = ui.proxyUser->text();
        if (ui.proxyPass->text().length())
            proxy.pass = ui.proxyPass->text();

        item->setProxy(proxy);
    }

    return item;
}

void AddDialog::on_shareBtn_clicked()
{
    BEWShareDialog share;
    share.setJson( QString::fromUtf8(QJsonDocument(generateBEW()->toJson()).toJson()) );
    share.exec();
}

void AddDialog::on_saveBtn_clicked()
{
    if (!mRestoreBEW)
        return;

    mRestoreBEW->uninstall();
    Q_EMIT refreshRequest(mRestoreBEW);

    auto item = generateBEW();
    if (item->install())
    {
        Q_EMIT refreshRequest(item);
        QMessageBox::information(this, "Save", QString("Your changes saved successfully at \"%1\".").arg(BEWAppItem::applicationsPath()));
        close();
    }
}

