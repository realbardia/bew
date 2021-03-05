#include "bewwebview.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>

BewWebView::BewWebView(QWidget *parent) :
    QWebEngineView(parent)
{
    connect(this, &QWebEngineView::titleChanged, this, &BewWebView::on_webEngineView_urlChanged);
    connect(page(), &QWebEnginePage::featurePermissionRequested, this, [this](const QUrl &securityOrigin, QWebEnginePage::Feature feature){
        QVariant var(feature);
        var.convert(QVariant::String);

        auto res = QMessageBox::warning(this, "Permission", QString("%1 needs below permission:\n%2").arg(QCoreApplication::applicationName()).arg(var.toString()),
                                        QMessageBox::Ok, QMessageBox::Ok | QMessageBox::Discard);
        if (res == QMessageBox::Ok)
            page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
    });
}

void BewWebView::on_webEngineView_urlChanged(const QString &arg1)
{
}

QWebEngineView *BewWebView::createWindow(QWebEnginePage::WebWindowType)
{
    QWebEngineView *view = new QWebEngineView(this);
    view->setWindowFlag(Qt::Dialog);
    view->resize(1024, 600);
    view->show();
    view->setAttribute(Qt::WA_DeleteOnClose);
    QWebEnginePage *page = view->page();

    connect(view, &QWebEngineView::iconChanged, this, [view](const QIcon &icon){ view->setWindowIcon(icon); });
    connect(view, &QWebEngineView::titleChanged, this, [view](const QString &title){ view->setWindowTitle(title); });
    connect(page, &QWebEnginePage::featurePermissionRequested, this, [page, view](const QUrl &securityOrigin, QWebEnginePage::Feature feature){
        QVariant var(feature);
        var.convert(QVariant::String);

        auto res = QMessageBox::warning(view, "Permission", QString("%1 needs below permission:\n%2").arg(QCoreApplication::applicationName()).arg(var.toString()),
                                        QMessageBox::Ok, QMessageBox::Ok | QMessageBox::Discard);
        if (res == QMessageBox::Ok)
            page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
    });

    return view;
}

BewWebView::~BewWebView()
{

}
