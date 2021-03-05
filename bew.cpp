#include "bew.h"

#include <QUrl>
#include <QWebEngineSettings>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QMenu>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>

Bew::Bew(QWidget *parent)
    : QMainWindow(parent)
{
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
        menu->addAction( tr("Open in new Window"), this, [page, url](){ page->triggerAction(QWebEnginePage::OpenLinkInNewWindow); } );
        menu->addAction( tr("Open External"), this, [url](){ QDesktopServices::openUrl(url); } );
        menu->addSeparator();
    }

    menu->addAction( tr("Copy"), this, [page](){ page->triggerAction(QWebEnginePage::Copy); } )->setEnabled(data.editFlags() & QWebEngineContextMenuData::CanCopy);
    menu->addAction( tr("Cut"), this, [page](){ page->triggerAction(QWebEnginePage::Cut); } )->setEnabled(data.editFlags() & QWebEngineContextMenuData::CanCut);
    menu->addAction( tr("Paste"), this, [page](){ page->triggerAction(QWebEnginePage::Paste); } )->setEnabled(data.editFlags() & QWebEngineContextMenuData::CanPaste);

    menu->addSeparator();
    menu->addAction( tr("Select All"), this, [page](){ page->triggerAction(QWebEnginePage::SelectAll); } );
    if (media.isValid())
    {
        menu->addSeparator();
        menu->addAction( tr("Download"), this, [page, url](){ page->triggerAction(QWebEnginePage::DownloadLinkToDisk); } );
        menu->addAction( tr("Open External"), this, [url](){ QDesktopServices::openUrl(url); } );
    }

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

void Bew::downloadRequested(QWebEngineDownloadItem *download)
{
    QString url = download->url().toString();
    int idx = url.indexOf("?");
    if (idx > 0)
        url = url.left(idx);

    idx = url.lastIndexOf("/");
    if (idx > 0)
        url = url.mid(idx+1);

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
