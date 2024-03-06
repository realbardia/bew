#ifndef BEWSTOREITEMWIDGET_H
#define BEWSTOREITEMWIDGET_H

#include <QWidget>
#include "bewappitem.h"

namespace Ui {
class BewStoreItemWidget;
}

class BewStoreItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BewStoreItemWidget(QWidget *parent = nullptr);
    ~BewStoreItemWidget();

    BewAppItemPtr bew() const;
    void setBew(BewAppItemPtr newBew);

Q_SIGNALS:
    void refreshRequest(const BewAppItemPtr &from);

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void on_openBtn_clicked();
    void on_installBtn_clicked();

private:
    Ui::BewStoreItemWidget *ui;
    BewAppItemPtr mBew;
    bool mContainsMouse = false;
};

#endif // BEWSTOREITEMWIDGET_H
