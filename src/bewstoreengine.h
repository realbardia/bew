#ifndef BEWSTOREENGINE_H
#define BEWSTOREENGINE_H

#include <QObject>

#include "bewappitem.h"

class BEWStoreEngine : public QObject
{
    Q_OBJECT
public:
    BEWStoreEngine(QObject *parent = nullptr);
    virtual ~BEWStoreEngine();

    void refreshInstalleds(std::function<void(const QList<BEWAppItemPtr> &)> callback);
    void refreshStore(std::function<void(const QList<BEWAppItemPtr> &)> callback);
    static void sort(QList<BEWAppItemPtr> &list);
};

#endif // BEWSTOREENGINE_H
