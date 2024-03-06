#ifndef BEWSTORE_H
#define BEWSTORE_H

#include "bewappitem.h"
#include "bewstoreengine.h"

#include <QLineEdit>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QTimer>

namespace Ui {
class BewStore;
}

class BewStore : public QMainWindow
{
    Q_OBJECT

public:
    explicit BewStore(QWidget *parent = nullptr);
    ~BewStore();

public Q_SLOTS:
    void reload();
    void refresh();

private Q_SLOTS:
    void on_actionAdd_triggered();
    void on_actionSettings_triggered();

protected:
    void refreshRequest(const BewAppItemPtr &from);

private:
    Ui::BewStore *ui;
    BewStoreEngine *mEngine;
    QLineEdit *mSearchLine;

    QList<BewAppItemPtr> mInstalledBews;
    QList<BewAppItemPtr> mStoreBews;

    int mLoadingsCount = 0;
    QTimer *mRefreshTimer = nullptr;
};

#endif // BEWSTORE_H
