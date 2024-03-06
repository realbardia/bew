#include "bewstoreengine.h"
#include "bew_globals.h"

#include <QNetworkReply>
#include <QRunnable>
#include <QThreadPool>
#include <QDir>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QJsonArray>

BEWStoreEngine::BEWStoreEngine(QObject *parent)
    : QObject{parent}
{
}

BEWStoreEngine::~BEWStoreEngine()
{
}

void BEWStoreEngine::refreshInstalleds(std::function<void (const QList<BEWAppItemPtr> &)> callback)
{
    const auto root = BEWAppItem::applicationsPath();
    auto runnable = QRunnable::create([this, callback, root](){
        QList<BEWAppItemPtr> res;

        for (const auto &d: QDir(root).entryList({"*.desktop"}, QDir::Files))
        {
            auto ptr = BEWAppItemPtr::create();
            if (!ptr->restoreUsingFile(root + d))
                continue;

            res << ptr;
        }

        sort(res);
        QMetaObject::invokeMethod(this, [callback, res](){
            callback(res);
        }, Qt::QueuedConnection);
    });

    QThreadPool::globalInstance()->start(runnable);
}

void BEWStoreEngine::refreshStore(std::function<void (const QList<BEWAppItemPtr> &)> callback)
{
    auto am = new QNetworkAccessManager(this);

    QUrl url(BEW_STORE_URL);
    QNetworkRequest req;
    req.setUrl(url);

    auto reply = am->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, am, callback](){
        QList<BEWAppItemPtr> res;
        auto array = QJsonDocument::fromJson(reply->readAll()).array();
        for (const auto &obj: array)
        {
            auto item = BEWAppItemPtr::create();
            if (!item->fromJson(obj.toObject()))
                continue;

            res << item;
        }

        sort(res);
        callback(res);
        reply->deleteLater();
        am->deleteLater();
    });
}

bool sort_BEWAppItemPtr(const BEWAppItemPtr &s1, const BEWAppItemPtr &s2)
{
    return s1->title().toLower() < s2->title().toLower();
}

void BEWStoreEngine::sort(QList<BEWAppItemPtr> &list)
{
    std::sort(list.begin(), list.end(), sort_BEWAppItemPtr);
}
