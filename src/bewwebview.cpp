#include "bewwebview.h"
#include "bew.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QCryptographicHash>
#include <QWebEngineSettings>

BEWWebView::BEWWebView(QWidget *parent) :
    QWebEngineView(parent)
{
    page()->profile()->setHttpUserAgent(BEW::userAgent());
    connect(page(), &QWebEnginePage::featurePermissionRequested, this, [this](const QUrl &securityOrigin, QWebEnginePage::Feature feature){
        checkPermission(this, page(), securityOrigin, feature);
    });
}

QString BEWWebView::featureToString(QWebEnginePage::Feature feature)
{
    switch(static_cast<int>(feature))
    {
    case QWebEnginePage::Notifications:
        return QString("%1 want to send notification. Allow access?").arg(QCoreApplication::applicationName());
        break;
    case QWebEnginePage::Geolocation:
        return QString("%1 want to access your location. Allow access?").arg(QCoreApplication::applicationName());
        break;
    case QWebEnginePage::MediaAudioCapture:
        return QString("%1 want to record audio. Allow access?").arg(QCoreApplication::applicationName());
        break;
    case QWebEnginePage::MediaVideoCapture:
        return QString("%1 want to record video. Allow access?").arg(QCoreApplication::applicationName());
        break;
    case QWebEnginePage::MediaAudioVideoCapture:
        return QString("%1 want to record audio and video. Allow access?").arg(QCoreApplication::applicationName());
        break;
    case QWebEnginePage::MouseLock:
        return QString("%1 want to lock mouse in the window area. Allow access?").arg(QCoreApplication::applicationName());
        break;
    case QWebEnginePage::DesktopVideoCapture:
        return QString("%1 want to capture your desktop video. Allow access?").arg(QCoreApplication::applicationName());
        break;
    case QWebEnginePage::DesktopAudioVideoCapture:
        return QString("%1 want to capture your desktop video and audio. Allow access?").arg(QCoreApplication::applicationName());
        break;
    }

    return QString();
}

QString BEWWebView::featureToKey(QWebEnginePage::Feature feature)
{
    switch(static_cast<int>(feature))
    {
    case QWebEnginePage::Notifications:
        return "Notifications";
        break;
    case QWebEnginePage::Geolocation:
        return "Geolocation";
        break;
    case QWebEnginePage::MediaAudioCapture:
        return "MediaAudioCapture";
        break;
    case QWebEnginePage::MediaVideoCapture:
        return "MediaVideoCapture";
        break;
    case QWebEnginePage::MediaAudioVideoCapture:
        return "MediaAudioVideoCapture";
        break;
    case QWebEnginePage::MouseLock:
        return "MouseLock";
        break;
    case QWebEnginePage::DesktopVideoCapture:
        return "DesktopVideoCapture";
        break;
    case QWebEnginePage::DesktopAudioVideoCapture:
        return "DesktopAudioVideoCapture";
        break;
    }

    return QString();
}

void BEWWebView::checkPermission(QWebEngineView *view, QWebEnginePage *page, const QUrl &securityOrigin, QWebEnginePage::Feature feature)
{
    QString hash = QCryptographicHash::hash(securityOrigin.toString().toUtf8(), QCryptographicHash::Md5).toHex();
    QSettings settings(BEW::homePath() + "/permissions.ini", QSettings::IniFormat);
    settings.setValue(hash + "/origin", securityOrigin.toString());

    auto featureKey = hash + "/" + featureToKey(feature);
    switch (settings.value(featureKey, 0).toInt())
    {
    case -1:
        page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
        break;
    case 0:
    {
        auto res = QMessageBox::warning(view, "Permission", featureToString(feature),
                                        QMessageBox::Yes, QMessageBox::No);
        if (res == QMessageBox::Yes)
        {
            page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
            settings.setValue(featureKey, 1);
        }
        else
        {
            page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
            settings.setValue(featureKey, -1);
        }
    }
        break;
    case 1:
        page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
        break;
    }
}

QWebEngineView *BEWWebView::createWindow(QWebEnginePage::WebWindowType)
{
    QWebEngineView *view = new QWebEngineView(this);
    view->setWindowFlag(Qt::Dialog);
    view->resize(1024, 600);
    view->show();
    view->setAttribute(Qt::WA_DeleteOnClose);
    QWebEnginePage *page = view->page();

    connect(view, &QWebEngineView::iconChanged, this, [view](const QIcon &icon){ view->setWindowIcon(icon); });
    connect(view, &QWebEngineView::titleChanged, this, [view](const QString &title){ view->setWindowTitle(title); });
    connect(page, &QWebEnginePage::featurePermissionRequested, this, [page, view, this](const QUrl &securityOrigin, QWebEnginePage::Feature feature){
        checkPermission(view, page, securityOrigin, feature);
    });

    return view;
}

BEWWebView::~BEWWebView()
{

}
