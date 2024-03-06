#include "adddialog.h"
#include "bewstore.h"
#include "bewstoreitemwidget.h"
#include "ui_bewstore.h"
#include "settingsdialog.h"
#include "donatedialog.h"

#include <QPalette>

BEWStore::BEWStore(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BEWStore)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->no_installed_bew->hide();
    ui->no_store_bew->hide();

    mEngine = new BEWStoreEngine(this);

    mRefreshTimer = new QTimer(this);
    mRefreshTimer->setInterval(300);
    mRefreshTimer->setSingleShot(true);

    connect(mRefreshTimer, &QTimer::timeout, this, &BEWStore::reload);

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

BEWStore::~BEWStore()
{
    delete ui;
}

void BEWStore::reload()
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
    for (const auto &bew: mInstalledBEWs)
    {
        if (keyword.length() && !bew->title().toLower().remove(' ').contains(keyword))
            continue;
        if (!bew->isInstalled())
            continue;

        auto item = new BEWStoreItemWidget;
        item->setBEW(bew);
        connect(item, &BEWStoreItemWidget::refreshRequest, this, &BEWStore::refreshRequest);

        ui->installedsGrid->addWidget(item, installedIdx/columnsCount, installedIdx%columnsCount);
        installedIdx++;
    }
    ui->no_installed_bew->setVisible(ui->installedsGrid->count() == 0);

    int storeIdx = 0;
    for (const auto &bew: mStoreBEWs)
    {
        if (keyword.length() && !bew->title().toLower().remove(' ').contains(keyword))
            continue;
        if (bew->isInstalled())
            continue;

        auto item = new BEWStoreItemWidget;
        item->setBEW(bew);
        connect(item, &BEWStoreItemWidget::refreshRequest, this, &BEWStore::refreshRequest);

        ui->storeGrid->addWidget(item, storeIdx/columnsCount, storeIdx%columnsCount);
        storeIdx++;
    }
    ui->no_store_bew->setVisible(ui->storeGrid->count() == 0);
}

void BEWStore::refresh()
{
    mInstalledBEWs.clear();
    mStoreBEWs.clear();
    reload();

    mLoadingsCount = 2;
    ui->progressBar->show();
    ui->store->hide();

    mEngine->refreshInstalleds([this](const QList<BEWAppItemPtr> &items){
        mInstalledBEWs = items;
        reload();

        mLoadingsCount--;
        if (mLoadingsCount <= 0)
        {
            ui->progressBar->hide();
            ui->store->show();
            mLoadingsCount = 0;
        }
    });

    mEngine->refreshStore([this](const QList<BEWAppItemPtr> &items){
        mStoreBEWs = items;
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

void BEWStore::on_actionAdd_triggered()
{
    AddDialog add;
    connect(&add, &AddDialog::refreshRequest, this, &BEWStore::refreshRequest);
    add.exec();
}

void BEWStore::refreshRequest(const BEWAppItemPtr &from)
{
    mStoreBEWs.removeAll(from);
    mInstalledBEWs.removeAll(from);

    if (from->isInstalled())
    {
        mInstalledBEWs.append(from);
        BEWStoreEngine::sort(mInstalledBEWs);
    }
    else
    {
        mStoreBEWs.append(from);
        BEWStoreEngine::sort(mStoreBEWs);
    }

    QMetaObject::invokeMethod(this, &BEWStore::reload, Qt::QueuedConnection);
}

void BEWStore::on_actionSettings_triggered()
{
    SettingsDialog settings;
    settings.exec();
}

void BEWStore::on_actionDonate_triggered()
{
    DonateDialog donate;
    donate.exec();
}

