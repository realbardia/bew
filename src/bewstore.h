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
class BEWStore;
}

class BEWStore : public QMainWindow
{
    Q_OBJECT

public:
    explicit BEWStore(QWidget *parent = nullptr);
    ~BEWStore();

public Q_SLOTS:
    void reload();
    void refresh();

private Q_SLOTS:
    void on_actionAdd_triggered();
    void on_actionSettings_triggered();
    void on_actionDonate_triggered();

protected:
    void refreshRequest(const BEWAppItemPtr &from);

private:
    Ui::BEWStore *ui;
    BEWStoreEngine *mEngine;
    QLineEdit *mSearchLine;

    QList<BEWAppItemPtr> mInstalledBEWs;
    QList<BEWAppItemPtr> mStoreBEWs;

    int mLoadingsCount = 0;
    QTimer *mRefreshTimer = nullptr;
};

#endif // BEWSTORE_H
