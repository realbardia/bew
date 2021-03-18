#include "adddialog.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

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
    QFile file(":/bew/templates/template.desktop");
    if (!file.open(QFile::ReadOnly))
        return;

    QString data = QString::fromUtf8(file.readAll());
    file.close();

    QString command = QStringLiteral("bew \"%1\" --app-name \"%2\" --app-icon \"%3\"")
            .arg(ui.website->text(), ui.title->text(), ui.icon->text());

    if (!ui.scrollbar->isChecked())
        command += " --no-scrollbar";
    if (ui.useragent->text().length())
        command += " --user-agent \"" + ui.useragent->text() + "\"";
    if (ui.font->currentText().length())
        command += " --font \"" + ui.font->currentText() + "\"";

    if (ui.proxyGroup->isChecked())
    {
        command += "--proxy-type \"" + ui.proxyType->currentText().toLower() + "\"";
        if (ui.proxyHost->text().length())
            command += "--proxy-host \"" + ui.proxyHost->text() + "\"";
        if (ui.proxyPort->value())
            command += "--proxy-port \"" + QString::number(ui.proxyPort->value()) + "\"";
        if (ui.proxyUser->text().length())
            command += "--proxy-username \"" + ui.proxyUser->text() + "\"";
        if (ui.proxyPass->text().length())
            command += "--proxy-password \"" + ui.proxyPass->text() + "\"";
    }

    data.replace("%command%", command);
    data.replace("%icon%", ui.icon->text());
    data.replace("%name%", ui.title->text());

    file.setFileName(QDir::homePath() + "/.local/share/applications/" + ui.title->text() + ".desktop");
    file.open(QFile::WriteOnly);
    file.write(data.toUtf8());
    file.close();

    QProcess::startDetached("update-mime-database", QStringList());
    QMessageBox::information(this, "Done", QString("Your Bew created successfully at \"%1\".\nYou can find it at application menu.").arg(file.fileName()));

    if (qgetenv("DESKTOP_SESSION").toLower().contains("plasma"))
        QProcess::startDetached("kioclient5", {"exec", file.fileName()});
    else
        QProcess::startDetached("gtk-launch", {file.fileName()});

    QDialog::accept();
}

void AddDialog::checkSaveBtn()
{
    ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(
                    ui.title->text().length() &&
                    ui.icon->text().length() &&
                    ui.website->text().length()
                );
}

void AddDialog::selectIcon()
{
    auto path = QFileDialog::getOpenFileName(this, "Please select icon", "", "Images (*.png *.svg)");
    if (path.isEmpty())
        return;

    ui.icon->setText(path);
}
