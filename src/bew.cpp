#include "bew.h"
#include "bew_globals.h"

#include <QUrl>
#include <QWebEngineSettings>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QMenu>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QMimeDatabase>
#include <QLocalSocket>
#include <QBuffer>
#include <QKeyEvent>

QString BEW::mUserAgent = BEW_DEFAULT_USER_AGENT;

BEW::BEW(QWidget *parent)
    : QMainWindow(parent),
      mInstanceServer(Q_NULLPTR),
      mSysTray(Q_NULLPTR)
{
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

    mWeb = new BEWWebView;
    mWeb->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mWeb->page()->profile(), &QWebEngineProfile::downloadRequested, this, &BEW::downloadRequested);

    connect(mWeb, &QWebEngineView::urlChanged, this, &BEW::webViewUrlChanged);
    connect(mWeb, &QWebEngineView::iconChanged, this, &BEW::webViewIconChanged);
    connect(mWeb, &QWebEngineView::customContextMenuRequested, this, &BEW::webViewCustomContextMenuRequested);

    mMessage = new BewMessageWidget;
    mMessage->hide();

    connect(mMessage, &BewMessageWidget::activatedChanged, this, [this](bool mActivated){
        mMessage->setVisible(mActivated);
        mWeb->setVisible(!mActivated);
    });

    auto widget = new QWidget;
    auto layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(mWeb);
    layout->addWidget(mMessage);

    setCentralWidget(widget);
    resize(1000, 600);
    restore();
}

BEW::~BEW()
{
    save();
}

void BEW::setFonts(const QString &font)
{
    mWeb->settings()->setFontFamily(QWebEngineSettings::StandardFont, font);
}

void BEW::load(const QString &url)
{
    mWeb->load(QUrl(url));
}

void BEW::webViewUrlChanged(const QUrl &)
{
}

void BEW::webViewIconChanged(const QIcon &arg1)
{
    setWindowIcon(arg1);
}

void BEW::webViewCustomContextMenuRequested(const QPoint &)
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

QString BEW::homePath()
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

void BEW::restore()
{
    QFile file(BEW::homePath() + "/window-geometry.dat");
    if (!file.open(QFile::ReadOnly))
        return;

    restoreGeometry(file.readAll());
    file.close();
}

void BEW::save()
{
    QFile file(BEW::homePath() + "/window-geometry.dat");
    if (!file.open(QFile::WriteOnly))
        return;

    file.write(saveGeometry());
    file.close();
}

void BEW::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier)
    {
        if (e->key() == Qt::Key_Q)
            QCoreApplication::quit();
    }
}

void BEW::setMessageIdleMinutes(qint32 newMessageIdleMinutes)
{
    mMessage->setMessageIdleMinutes(newMessageIdleMinutes);
}

void BEW::setMessage(const QString &newMessage)
{
    mMessage->setMessage(newMessage);
}

QString BEW::userAgent()
{
    return mUserAgent;
}

QString BEW::instancePath()
{
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream << QDir::homePath();
    stream << QCoreApplication::applicationFilePath();
    stream << QCoreApplication::arguments();

    auto res = QDir::tempPath() + "/bew-" + QCryptographicHash::hash(bytes, QCryptographicHash::Md5).toHex() + ".socket";
    return res;
}

void BEW::setUserAgent(const QString &userAgent)
{
    mUserAgent = userAgent;
}

bool BEW::showInstance()
{
    qRegisterMetaType<QLocalSocket::LocalSocketError>("QLocalSocket::LocalSocketError");

    auto path = BEW::instancePath();
    auto socket = new QLocalSocket;

    QEventLoop loop;
    bool error = false;

    socket->connect(socket, &QLocalSocket::connected, socket, [&loop, &error, socket](){
        socket->write("show");
        socket->flush();
        socket->close();
        error = false;
        loop.exit();
    }, Qt::QueuedConnection);
    socket->connect(socket, &QLocalSocket::errorOccurred, socket, [&loop, &error, path](){
        error = true;
        QLocalServer::removeServer(path);
        loop.exit();
    }, Qt::QueuedConnection);

    socket->connectToServer(path);
    loop.exec();

    return !error;
}

void BEW::downloadRequested(QWebEngineDownloadItem *download)
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

    download->setDownloadFileName(path);
    download->accept();
}

void BEW::setScrollBar(bool state)
{
    mWeb->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, state);
}

void BEW::setSystemTray(bool state)
{
    if ((mSysTray != Q_NULLPTR) == state)
        return;

    if (mSysTray) {
        delete mSysTray;
        mSysTray = Q_NULLPTR;
    } else {

        auto menu = new QMenu();
        menu->addAction(tr("Show"), this, [this](){
            show();
            activateWindow();
        });
        menu->addAction(tr("Quit"), QCoreApplication::instance(), &QCoreApplication::quit);

        mSysTray = new QSystemTrayIcon(this);
        mSysTray->setIcon(windowIcon());
        mSysTray->setContextMenu(menu);
        mSysTray->show();

        connect(mSysTray, &QSystemTrayIcon::activated, this, [this](){
            setVisible(!isVisible());
            activateWindow();
        });
    }
}

void BEW::setSingleInstance(bool state)
{
    if ((mInstanceServer != Q_NULLPTR) == state)
        return;

    if (mInstanceServer) {
        delete mInstanceServer;
        mInstanceServer = Q_NULLPTR;
    } else {
        auto path = BEW::instancePath();
        QLocalServer::removeServer(path);

        mInstanceServer = new QLocalServer(this);
        mInstanceServer->listen(path);

        connect(mInstanceServer, &QLocalServer::newConnection, this, [this](){
            auto socket = mInstanceServer->nextPendingConnection();
            connect(socket, &QLocalSocket::readyRead, this, [socket, this](){
                auto data = socket->readAll();
                if (data.toLower() == "show")
                {
                    show();
                    activateWindow();
                }
            });
        });
    }
}
