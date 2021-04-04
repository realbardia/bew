#include "bew.h"

#include <QUrl>
#include <QWebEngineSettings>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QMenu>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QMimeDatabase>

QString Bew::mUserAgent;

Bew::Bew(QWidget *parent)
    : QMainWindow(parent)
{
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

    mWeb = new BewWebView;
    mWeb->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mWeb->page()->profile(), &QWebEngineProfile::downloadRequested, this, &Bew::downloadRequested);

    connect(mWeb, &QWebEngineView::urlChanged, this, &Bew::webViewUrlChanged);
    connect(mWeb, &QWebEngineView::iconChanged, this, &Bew::webViewIconChanged);
    connect(mWeb, &QWebEngineView::customContextMenuRequested, this, &Bew::webViewCustomContextMenuRequested);

    setCentralWidget(mWeb);
    resize(1000, 600);
    restore();
}

Bew::~Bew()
{
    save();
}

void Bew::setFonts(const QString &font)
{
    mWeb->settings()->setFontFamily(QWebEngineSettings::StandardFont, font);
}

void Bew::load(const QString &url)
{
    mWeb->load(QUrl(url));
}

void Bew::webViewUrlChanged(const QUrl &)
{
}

void Bew::webViewIconChanged(const QIcon &arg1)
{
    setWindowIcon(arg1);
}

void Bew::webViewCustomContextMenuRequested(const QPoint &)
{
    auto page = mWeb->page();
    if (!page)
        return;

    auto menu = page->createStandardContextMenu();
    if (!menu)
        return;

    const auto &data = page->contextMenuData();
    auto url = data.linkUrl();
    auto media = data.mediaUrl();

    menu->clear();
    if (url.isValid())
    {
        menu->addAction( tr("Open in new window"), this, [page, url](){ page->triggerAction(QWebEnginePage::OpenLinkInNewWindow); } );
        menu->addAction( tr("Open external"), this, [url](){ QDesktopServices::openUrl(url); } );
        menu->addAction( tr("Copy link"), this, [page, url](){ page->triggerAction(QWebEnginePage::CopyLinkToClipboard); } );
        menu->addSeparator();
    }

    menu->addAction( tr("Copy"), this, [page](){ page->triggerAction(QWebEnginePage::Copy); } )->setEnabled(data.editFlags() & QWebEngineContextMenuData::CanCopy);
    menu->addAction( tr("Cut"), this, [page](){ page->triggerAction(QWebEnginePage::Cut); } )->setEnabled(data.editFlags() & QWebEngineContextMenuData::CanCut);
    menu->addAction( tr("Paste"), this, [page](){ page->triggerAction(QWebEnginePage::Paste); } )->setEnabled(data.editFlags() & QWebEngineContextMenuData::CanPaste);

    menu->addSeparator();
    menu->addAction( tr("Select all"), this, [page](){ page->triggerAction(QWebEnginePage::SelectAll); } );

    if (media.isValid())
    {
        menu->addSeparator();
        switch (static_cast<int>(data.mediaType()))
        {
        case QWebEngineContextMenuData::MediaTypeImage:
            menu->addAction( tr("Download image"), this, [page, url](){ page->triggerAction(QWebEnginePage::DownloadImageToDisk); } );
            menu->addAction( tr("Copy image link"), this, [page, url](){ page->triggerAction(QWebEnginePage::CopyImageUrlToClipboard); } );
            menu->addAction( tr("Copy image"), this, [page, url](){ page->triggerAction(QWebEnginePage::CopyImageToClipboard); } );
            break;

        default:
            menu->addAction( tr("Download media"), this, [page, url](){ page->triggerAction(QWebEnginePage::DownloadMediaToDisk); } );
            menu->addAction( tr("Copy media link"), this, [page, url](){ page->triggerAction(QWebEnginePage::CopyMediaUrlToClipboard); } );
            break;
        }
        menu->addAction( tr("Open media external"), this, [url](){ QDesktopServices::openUrl(url); } );
    }

    menu->addSeparator();
    menu->addAction( tr("Back"), this, [page](){ page->triggerAction(QWebEnginePage::Back); } );
    menu->addAction( tr("Forward"), this, [page](){ page->triggerAction(QWebEnginePage::Forward); } );
    menu->addAction( tr("Reload"), this, [page](){ page->triggerAction(QWebEnginePage::Reload); } );

    menu->move( QCursor::pos() );
    menu->show();
}

QString Bew::homePath()
{
    QString path;
#ifdef Q_OS_WIN
    path = QDir::homePath() + "/AppData/Local/bew/" + QCoreApplication::applicationName();
#else
    path = QDir::homePath() + "/.local/share/bew/" + QCoreApplication::applicationName();
#endif

    QDir().mkpath(path);
    return path;
}

void Bew::restore()
{
    QFile file(Bew::homePath() + "/window-geometry.dat");
    if (!file.open(QFile::ReadOnly))
        return;

    restoreGeometry(file.readAll());
    file.close();
}

void Bew::save()
{
    QFile file(Bew::homePath() + "/window-geometry.dat");
    if (!file.open(QFile::WriteOnly))
        return;

    file.write(saveGeometry());
    file.close();
}

QString Bew::userAgent()
{
    return mUserAgent;
}

void Bew::setUserAgent(const QString &userAgent)
{
    mUserAgent = userAgent;
}

void Bew::downloadRequested(QWebEngineDownloadItem *download)
{
    QString url = download->url().toString() + download->downloadFileName();

    QMimeDatabase mdb;
    auto suffixes = mdb.mimeTypeForName(download->mimeType()).suffixes();
    if (suffixes.count())
        url += "." + suffixes.first();

    QString path = QFileDialog::getSaveFileName(this, tr("Save file"), QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + url);
    if (path.isEmpty())
    {
        download->cancel();
        return;
    }

    download->setPath(path);
    download->accept();
}

void Bew::setScrollBar(bool state)
{
    mWeb->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, state);
}
