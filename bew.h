#ifndef BEW_H
#define BEW_H

#include "bewwebview.h"

#include <QMainWindow>
#include <QWebEngineContextMenuData>
#include <QWebEngineProfile>

class Bew : public QMainWindow
{
    Q_OBJECT
public:
    Bew(QWidget *parent = nullptr);
    ~Bew();

    void setFonts(const QString &font);
    void downloadRequested(QWebEngineDownloadItem *download);
    void setScrollBar(bool state);

    static QString homePath();

    static QString userAgent();
    static void setUserAgent(const QString &userAgent);

public Q_SLOTS:
    void load(const QString &url);

private:
    void webViewUrlChanged(const QUrl &arg1);
    void webViewIconChanged(const QIcon &arg1);
    void webViewCustomContextMenuRequested(const QPoint &pos);

protected:
    void restore();
    void save();

private:
    BewWebView *mWeb;
    static QString mUserAgent;
};
#endif // BEW_H
