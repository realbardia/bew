#ifndef BEWSTOREITEMWIDGET_H
#define BEWSTOREITEMWIDGET_H

#include <QWidget>
#include "bewappitem.h"

namespace Ui {
class BEWStoreItemWidget;
}

class BEWStoreItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BEWStoreItemWidget(QWidget *parent = nullptr);
    ~BEWStoreItemWidget();

    BEWAppItemPtr bew() const;
    void setBEW(BEWAppItemPtr newBEW);

Q_SIGNALS:
    void refreshRequest(const BEWAppItemPtr &from);

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void on_openBtn_clicked();
    void on_installBtn_clicked();

private:
    Ui::BEWStoreItemWidget *ui;
    BEWAppItemPtr mBEW;
    bool mContainsMouse = false;
};

#endif // BEWSTOREITEMWIDGET_H
