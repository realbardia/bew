#include "bewstoreitemwidget.h"
#include "ui_bewstoreitemwidget.h"
#include "adddialog.h"

#include <QMessageBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

BewStoreItemWidget::BewStoreItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BewStoreItemWidget)
{
    ui->setupUi(this);
}

BewStoreItemWidget::~BewStoreItemWidget()
{
    delete ui;
}

BewAppItemPtr BewStoreItemWidget::bew() const
{
    return mBew;
}

void BewStoreItemWidget::setBew(BewAppItemPtr newBew)
{
    mBew = newBew;
    if (mBew)
    {
        auto url = mBew->url();
        if (url.length() > 33)
            url = url.left(30) + "...";

        QImage img(mBew->iconUrl());
        ui->title->setText(mBew->title());
        ui->url->setText(url);
        ui->image->setPixmap( QPixmap::fromImage(img.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation)) );

        const auto installed = mBew->isInstalled();
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

void BewStoreItemWidget::paintEvent(QPaintEvent *)
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

void BewStoreItemWidget::enterEvent(QEvent *)
{
    mContainsMouse = true;
    update();
}

void BewStoreItemWidget::leaveEvent(QEvent *)
{
    mContainsMouse = false;
    update();
}

void BewStoreItemWidget::mouseReleaseEvent(QMouseEvent *)
{
    AddDialog dlg;
    dlg.restoreBew(mBew);
    connect(&dlg, &AddDialog::refreshRequest, this, &BewStoreItemWidget::refreshRequest);
    dlg.exec();
}

void BewStoreItemWidget::on_openBtn_clicked()
{
    mBew->launch();
}


void BewStoreItemWidget::on_installBtn_clicked()
{
    if (mBew->install())
    {
        QMessageBox::information(this, "Done", QString("Your Bew installed successfully at \"%1\".\nYou can find it at application menu.").arg(BewAppItem::applicationsPath()));
        Q_EMIT refreshRequest(mBew);
    }
}

