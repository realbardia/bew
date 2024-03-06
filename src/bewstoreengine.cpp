#include "bewstoreengine.h"
#include "bew_globals.h"

#include <QNetworkReply>
#include <QRunnable>
#include <QThreadPool>
#include <QDir>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QJsonArray>

BewStoreEngine::BewStoreEngine(QObject *parent)
    : QObject{parent}
{
}

BewStoreEngine::~BewStoreEngine()
{
}

void BewStoreEngine::refreshInstalleds(std::function<void (const QList<BewAppItemPtr> &)> callback)
{
    const auto root = BewAppItem::applicationsPath();
    auto runnable = QRunnable::create([this, callback, root](){
        QList<BewAppItemPtr> res;

        QThread::msleep(500);

        for (const auto &d: QDir(root).entryList({"*.desktop"}, QDir::Files))
        {
            auto ptr = BewAppItemPtr::create();
            if (!ptr->restoreUsingFile(root + d))
                continue;

            res << ptr;
        }

        QMetaObject::invokeMethod(this, [callback, res](){
                callback(res);
        }, Qt::QueuedConnection);
    });

    QThreadPool::globalInstance()->start(runnable);
}

void BewStoreEngine::refreshStore(std::function<void (const QList<BewAppItemPtr> &)> callback)
{
    auto am = new QNetworkAccessManager(this);

    QUrl url(BEW_STORE_URL);
    QNetworkRequest req;
    req.setUrl(url);

    auto reply = am->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, am, callback](){
        QList<BewAppItemPtr> list;
        auto array = QJsonDocument::fromJson(reply->readAll()).array();
        for (const auto &obj: array)
        {
            auto item = BewAppItemPtr::create();
            if (!item->fromJson(obj.toObject()))
                continue;

            list << item;
        }

        callback(list);
        reply->deleteLater();
        am->deleteLater();
    });
}
