#ifndef BEWSTOREENGINE_H
#define BEWSTOREENGINE_H

#include <QObject>

#include "bewappitem.h"

class BewStoreEngine : public QObject
{
    Q_OBJECT
public:
    BewStoreEngine(QObject *parent = nullptr);
    virtual ~BewStoreEngine();

    void refreshInstalleds(std::function<void(const QList<BewAppItemPtr> &)> callback);
    void refreshStore(std::function<void(const QList<BewAppItemPtr> &)> callback);
};

#endif // BEWSTOREENGINE_H
