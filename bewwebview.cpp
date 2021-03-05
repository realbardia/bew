#include "bewwebview.h"

#include <QDebug>

BewWebView::BewWebView(QWidget *parent) :
    QWebEngineView(parent)
{
    connect(this, &QWebEngineView::titleChanged, this, &BewWebView::on_webEngineView_urlChanged);
    connect(page(), &QWebEnginePage::featurePermissionRequested, this, [this](const QUrl &securityOrigin, QWebEnginePage::Feature feature){
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
    connect(page, &QWebEnginePage::featurePermissionRequested, this, [page](const QUrl &securityOrigin, QWebEnginePage::Feature feature){
        page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
    });

    return view;
}

BewWebView::~BewWebView()
{

}
