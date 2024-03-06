#include "adddialog.h"
#include "bewstore.h"
#include "bewstoreitemwidget.h"
#include "ui_bewstore.h"
#include "settingsdialog.h"

#include <QPalette>

BewStore::BewStore(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BewStore)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->no_installed_bew->hide();
    ui->no_store_bew->hide();

    mEngine = new BewStoreEngine(this);

    mRefreshTimer = new QTimer(this);
    mRefreshTimer->setInterval(300);
    mRefreshTimer->setSingleShot(true);

    connect(mRefreshTimer, &QTimer::timeout, this, &BewStore::reload);

    mSearchLine = new QLineEdit;
    mSearchLine->setFixedWidth(350);
    mSearchLine->setPlaceholderText(tr("Search"));
    mSearchLine->setClearButtonEnabled(true);

    connect(mSearchLine, &QLineEdit::textEdited, this, [this](){
        mRefreshTimer->stop();
        mRefreshTimer->start();
    });

    auto toolbarWidget = new QWidget;
    auto toolbarLayout = new QHBoxLayout(toolbarWidget);
    toolbarLayout->setContentsMargins(0,0,0,0);
    toolbarLayout->addSpacing(110);
    toolbarLayout->addWidget(mSearchLine);

    ui->toolBar->addWidget(toolbarWidget);
    ui->toolBar->addAction(ui->actionSettings);
    ui->toolBar->addAction(ui->actionDonate);
}

BewStore::~BewStore()
{
    delete ui;
}

void BewStore::reload()
{
    QLayoutItem *child;
    while ((child = ui->storeGrid->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }
    while ((child = ui->installedsGrid->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    const auto keyword = mSearchLine->text().toLower().remove(' ').trimmed();
    const int columnsCount = 3;

    int installedIdx = 0;
    for (const auto &bew: mInstalledBews)
    {
        if (keyword.length() && !bew->title().toLower().remove(' ').contains(keyword))
            continue;
        if (!bew->isInstalled())
            continue;

        auto item = new BewStoreItemWidget;
        item->setBew(bew);
        connect(item, &BewStoreItemWidget::refreshRequest, this, &BewStore::refreshRequest);

        ui->installedsGrid->addWidget(item, installedIdx/columnsCount, installedIdx%columnsCount);
        installedIdx++;
    }
    ui->no_installed_bew->setVisible(mInstalledBews.isEmpty());

    int storeIdx = 0;
    for (const auto &bew: mStoreBews)
    {
        if (keyword.length() && !bew->title().toLower().remove(' ').contains(keyword))
            continue;
        if (bew->isInstalled())
            continue;

        auto item = new BewStoreItemWidget;
        item->setBew(bew);
        connect(item, &BewStoreItemWidget::refreshRequest, this, &BewStore::refreshRequest);

        ui->storeGrid->addWidget(item, storeIdx/columnsCount, storeIdx%columnsCount);
        storeIdx++;
    }
    ui->no_store_bew->setVisible(mStoreBews.isEmpty());
}

void BewStore::refresh()
{
    mInstalledBews.clear();
    mStoreBews.clear();
    reload();

    mLoadingsCount = 2;
    ui->progressBar->show();
    ui->store->hide();

    mEngine->refreshInstalleds([this](const QList<BewAppItemPtr> &items){
        mInstalledBews = items;
        reload();

        mLoadingsCount--;
        if (mLoadingsCount <= 0)
        {
            ui->progressBar->hide();
            ui->store->show();
            mLoadingsCount = 0;
        }
    });

    mEngine->refreshStore([this](const QList<BewAppItemPtr> &items){
        mStoreBews = items;
        reload();

        mLoadingsCount--;
        if (mLoadingsCount <= 0)
        {
            ui->progressBar->hide();
            ui->store->show();
            mLoadingsCount = 0;
        }
    });
}

void BewStore::on_actionAdd_triggered()
{
    AddDialog add;
    connect(&add, &AddDialog::refreshRequest, this, &BewStore::refreshRequest);
    add.exec();
}

void BewStore::refreshRequest(const BewAppItemPtr &from)
{
    mStoreBews.removeAll(from);
    mInstalledBews.removeAll(from);

    if (from->isInstalled())
        mInstalledBews.append(from);
    else
        mStoreBews.append(from);

    QMetaObject::invokeMethod(this, &BewStore::reload, Qt::QueuedConnection);
}

void BewStore::on_actionSettings_triggered()
{
    SettingsDialog settings;
    settings.exec();
}

