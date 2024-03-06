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

        for (const auto &d: QDir(root).entryList({"*.desktop"}, QDir::Files))
        {
            auto ptr = BewAppItemPtr::create();
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

void BewStoreEngine::refreshStore(std::function<void (const QList<BewAppItemPtr> &)> callback)
{
    auto am = new QNetworkAccessManager(this);

    QUrl url(BEW_STORE_URL);
    QNetworkRequest req;
    req.setUrl(url);

    auto reply = am->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, am, callback](){
        QList<BewAppItemPtr> res;
        auto array = QJsonDocument::fromJson(reply->readAll()).array();
        for (const auto &obj: array)
        {
            auto item = BewAppItemPtr::create();
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

bool sort_BewAppItemPtr(const BewAppItemPtr &s1, const BewAppItemPtr &s2)
{
    return s1->title().toLower() < s2->title().toLower();
}

void BewStoreEngine::sort(QList<BewAppItemPtr> &list)
{
    std::sort(list.begin(), list.end(), sort_BewAppItemPtr);
}
