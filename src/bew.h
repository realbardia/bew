#ifndef BEW_H
#define BEW_H

#include "bewwebview.h"

#include <QLocalServer>
#include <QMainWindow>
#include <QWebEngineContextMenuData>
#include <QWebEngineProfile>
#include <QSystemTrayIcon>

class Bew : public QMainWindow
{
    Q_OBJECT
public:
    Bew(QWidget *parent = nullptr);
    ~Bew();

    void setFonts(const QString &font);
    void downloadRequested(QWebEngineDownloadItem *download);
    void setScrollBar(bool state);
    void setSystemTray(bool state);
    void setSingleInstance(bool state);

    static QString homePath();

    static QString userAgent();
    static QString instancePath();
    static void setUserAgent(const QString &userAgent);
    static bool showInstance();

public Q_SLOTS:
    void load(const QString &url);

private:
    void webViewUrlChanged(const QUrl &arg1);
    void webViewIconChanged(const QIcon &arg1);
    void webViewCustomContextMenuRequested(const QPoint &pos);

protected:
    void restore();
    void save();
    void keyPressEvent(QKeyEvent *event);

private:
    BewWebView *mWeb;
    QLocalServer *mInstanceServer;
    QSystemTrayIcon *mSysTray;

    static QString mUserAgent;
};
#endif // BEW_H
