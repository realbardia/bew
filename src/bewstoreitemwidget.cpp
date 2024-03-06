#include "bewstoreitemwidget.h"
#include "ui_bewstoreitemwidget.h"
#include "adddialog.h"

#include <QMessageBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

BEWStoreItemWidget::BEWStoreItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BEWStoreItemWidget)
{
    ui->setupUi(this);
}

BEWStoreItemWidget::~BEWStoreItemWidget()
{
    delete ui;
}

BEWAppItemPtr BEWStoreItemWidget::bew() const
{
    return mBEW;
}

void BEWStoreItemWidget::setBEW(BEWAppItemPtr newBEW)
{
    mBEW = newBEW;
    if (mBEW)
    {
        auto url = mBEW->url();
        if (url.length() > 33)
            url = url.left(30) + "...";

        QImage img(mBEW->iconUrl());
        ui->title->setText(mBEW->title());
        ui->url->setText(url);
        ui->image->setPixmap( QPixmap::fromImage(img.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation)) );

        const auto installed = mBEW->isInstalled();
        ui->installBtn->setVisible(!installed);
        ui->openBtn->setVisible(installed);
    }
    else
    {
        ui->title->clear();
        ui->url->clear();
        ui->image->clear();
    }
}

void BEWStoreItemWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (mContainsMouse)
    {
        auto color = palette().highlight().color();
        color.setAlphaF(0.1);

        QPainterPath path;
        path.addRoundedRect(rect(), 10, 10);

        painter.fillPath(path, color);
    }
    else
        painter.fillRect(rect(), QColor(0,0,0,0));
}

void BEWStoreItemWidget::enterEvent(QEvent *)
{
    mContainsMouse = true;
    update();
}

void BEWStoreItemWidget::leaveEvent(QEvent *)
{
    mContainsMouse = false;
    update();
}

void BEWStoreItemWidget::mouseReleaseEvent(QMouseEvent *)
{
    AddDialog dlg;
    dlg.restoreBEW(mBEW);
    connect(&dlg, &AddDialog::refreshRequest, this, &BEWStoreItemWidget::refreshRequest);
    dlg.exec();
}

void BEWStoreItemWidget::on_openBtn_clicked()
{
    mBEW->launch();
}


void BEWStoreItemWidget::on_installBtn_clicked()
{
    if (mBEW->install())
    {
        QMessageBox::information(this, "Done", QString("Your BEW installed successfully at \"%1\".\nYou can find it at application menu.").arg(BEWAppItem::applicationsPath()));
        Q_EMIT refreshRequest(mBEW);
    }
}

